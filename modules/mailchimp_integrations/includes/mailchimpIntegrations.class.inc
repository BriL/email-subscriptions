<?php

require 'vendor/autoload.php';
use GuzzleHttp\Client;
use GuzzleHttp\Exception\RequestException;

/**
 * Class used to interact with Mailchimp api.
 */
class mailchimpIntegrations {

  /**
   * This is the basr url for the mailchimp API.
   */
  private $config = [
    'base_url' => 'https://us18.api.mailchimp.com/3.0/',
  ];
  private $name;
  private $email;

  private $fname;
  private $lname;
  private $contact_reason;
  private $contact_description;

  /**
   * Sets the name value to be saved to mailchimp.
   */
  public function set_name($value) {
    $this->name = $value;
  }

  /**
   * Gets the objects name.
   */
  public function get_name() {
    return $this->name;
  }

  /**
   * Sets the users email address.
   */
  public function set_email($value) {
    $this->email = $value;
  }

  /**
   * Gets the users email address.
   */
  public function get_email() {
    return $this->email;
  }

  /**
   * Sets the users first name.
   */
  public function set_fname($value) {
    $this->fname = $value;
  }

  /**
   * Gets the users first name.
   */
  public function get_fname() {
    return $this->fname;
  }

  /**
   * Sets the users last name.
   */
  public function set_lname($value) {
    $this->lname = $value;
  }

  /**
   * Gets the users last name.
   */
  public function get_lname() {
    return $this->lname;
  }

  /**
   * Sets the users reason for contacting.
   */
  public function set_contact_reason($value) {
    $this->contact_reason = $value;
  }

  /**
   * Gets the user reason fron contacting.
   */
  public function get_contact_reason() {
    return $this->contact_reason;
  }

  /**
   * Sets the contact description.
   */
  public function set_contact_desctiption($value) {
    $this->contact_description = $value;
  }

  /**
   * Gets the contact description.
   */
  public function get_contact_description() {
    return $this->contact_description;
  }

  /**
   * Contructer method that sets the auth and list id configurations saved in the admin settings.
   *
   * @param string $type
   *   The subsection data that is post the mailchimp database.
   */
  public function __construct($type) {
    // Retrieve the varibale setttings from the database.
    $grandera_mailchimp_integrations_settings = variable_get('grandera_mailchimp_integrations_settings');

    switch ($type) {
      case 'subscribe':
        // Configurations assigned by mailchimp API.
        $this->config['list_id'] = $grandera_mailchimp_integrations_settings['subscription_letter_id'];
        break;

      case 'contact':
        // Configurations assigned by mailchimp API.
        $this->config['list_id'] = $grandera_mailchimp_integrations_settings['contact_us_id'];
        break;

      default:
        watchdog(__CLASS__, 'Class type is not defined in class instance', array(), WATCHDOG_ERROR);;
        break;
    }

    // Set the auth and subsection configurations to be used in the endpoint call.
    $this->config['subsection'] = $type;
    $this->config['auth'] = [
      $grandera_mailchimp_integrations_settings['api_name'], $grandera_mailchimp_integrations_settings['api_key'],
    ];
  }

  /**
   * Goes to the specific url.
   *
   * @param string $url_type
   *   The case which we know which url to use.
   *
   * @return array(response body) || bool
   *   The response data from GET or false if fails.
   */
  public function save() {
    // Get the apprioprate save method.
    $save_method = 'save_' . $this->config['subsection'];
    $this->config['data'] = $this->$save_method();
    $this->config['url'] = 'lists/' . $this->config['list_id'] . '/members';
    // Set the url string used in the guzzle call.
    return $this->post();
  }

  /**
   * Perform a restful get call.
   *
   * @param string $url
   *   The target URL.
   *
   * @return array(response body) || bool
   *   The response data from POST or false if fails.
   */
  protected function post() {

    try {
      // Initialize guzzle client.
      $client = new Client();
      // Attempt to post the email through mailchimp API.
      $result = $client->post($this->config['base_url'] . $this->config['url'],
        [
          'auth' => $this->config['auth'],
          'json' => $this->config['data'],
          'http_errors' => FALSE,
        ]
      );

      if ($result->getStatusCode() == '200') {
        return TRUE;
      }
      else {
        // Log the guzzle error so it can be displayed to the user.
        $error = json_decode($result->getBody()->getContents());
        $this->setGuzzleError($error);
        return FALSE;
      }
    }
    catch (RequestException $e) {
      // Catch any unexpected errros and log them to the database.
      watchdog(__CLASS__, '<pre>' . print_r($e->getMessage(), TRUE) . '</pre>', array(), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  /**
   * Prepares the subscription data to be sent to mailchimp.
   *
   * @return arrayJSON
   *   The json data used to save the subscritption letter fields.
   */
  protected function save_contact() {
    return array(
      'email_address' => $this->get_email(),
      'merge_fields' => array(
        'FNAME' => $this->get_fname(),
        'LNAME' => $this->get_lname(),
        'REASON' => $this->get_contact_reason(),
        'DESCR' => $this->get_contact_description(),
      ),
      'status' => 'subscribed',
    );
  }

  /**
   * Prepares the subscription data to be sent to mailchimp.
   *
   * @return arrayJSON
   *   The json data used to save the subscritption letter fields.
   */
  protected function save_subscribe() {
    return array(
      'email_address' => $this->get_email(),
      'merge_fields' => array(
        'NAME' => $this->get_name(),
      ),
      'status' => 'subscribed',
    );
  }

  /**
   * Sets the guzzle error that is returned.
   *
   * @param string $error
   *   error body returned from guzzle.
   */
  protected function setGuzzleError($error) {
    // Custom messages to display to the user based on respose from mailchimp API.
    if ($error->title == 'Member Exists') {
      $this->errors = t('This email is already registered.');
    }
    elseif ($error->title == 'Invalid Resource') {
      $this->errors = $error->detail;
    }
    else {
      $this->errors = t('We are currently experiencing some technical difficulties. Please try agian later.');
    }
    watchdog(__CLASS__, '<pre>' . print_r($error, TRUE) . '</pre>', array(), WATCHDOG_ERROR);
  }

}
