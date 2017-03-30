<?php

require 'vendor/autoload.php';
use GuzzleHttp\Client;
use GuzzleHttp\Exception\RequestException;
use GuzzleHttp\Message\RequestInterface;

/**
* Class used to interact with Mailchimp api
*/

class trfsEmailSubscriptions
{

  /**
  * This is the basr url for the mailchimp API 
  */
  private $config = [
    'base_url' => 'https://us15.api.mailchimp.com/3.0/'
  ];
  private $email;

  public function set_email($value){
    $this->email = $value;
  }

  public function get_email(){
    return $this->email;
  }

  /**
  * Contructer method that sets the auth and list id configurations saved in the admin settings.
  */
  public function __construct()
  {
    // retrieve the varibale setttings from the database
    $trfs_email_subscriptions_settings = variable_get('trfs_email_subscriptions_settings');

    // Configurations assigned by mailchimp API
    $this->config['list_id'] = $trfs_email_subscriptions_settings['list_id'];
    $this->config['auth'] = [
      $trfs_email_subscriptions_settings['api_name'], $trfs_email_subscriptions_settings['api_key']
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
  public function setUrlAndGo($url_type){
    switch ($url_type) {
      case 'email_subscription':
        // set the url string used in the guzzle call
        $mail_url_string = 'lists/' . $this->config['list_id'] . '/members';

        return $this->post($mail_url_string);

        break;
      default:
        watchdog("trfsEmailSubscriptions set URL", 'url type not defined', array(), WATCHDOG_ERROR);;
    }
  }

  /**
   * Perform a restful get call.
   *
   * @param string $url
   *   The target URL.
   *
   * @return array(response body) || bool
   *   The response data from GET or false if fails.
   */
  protected function post($url){

    try{
      // initialize guzzle client
      $client = new GuzzleHttp\Client();
      // attempt to post the email through mailchimp API
      $result = $client->post($this->config['base_url'] . $url, 
        [
          'auth' => $this->config['auth'],
          'json' => [
            'email_address' => $this->get_email(),
            'status' => 'subscribed'
          ],
          'http_errors' => false
        ]
      );

      if($result->getStatusCode() == '200'){
        return TRUE;
      }
      else{
        // log the guzzle error so it can be displayed to the user
        $error = json_decode($result->getBody()->getContents());
        $this->setGuzzleError($error);
        return FALSE;
      }

    }
    catch (RequestException $e){
      // Catch any unexpected errros and log them to the database
      watchdog("trfsEmailSubscriptions post", '<pre>' . print_r( $e->getMessage(), true) . '</pre>', array(), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  /**
   * Sets the guzzle error that is returned.
   *
   * @param string $error
   *   error body returned from guzzle.
   */
  protected function setGuzzleError($error){
    // custom messages to display to the user based on respose from mailchimp API
    if ($error->title == 'Member Exists'){
      $this->errors = 'This email is already registered.';
    }
    elseif($error->title == 'Invalid Resource'){
      $this->errors = $error->detail;
    }
    else{
      $this->errors = t('We are currenlty experiencing issues.').'<br/>'.t('Please try again later.');
    }
  }


}