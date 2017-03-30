<?php 

/*
 * Implemnts hook_form().
 */
function trfs_email_subscriptions_form($form, &$form_state){

  $form = array();

  $form['email'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'placeholder' => t('*Email Address')
    ),
    '#size' => 40,
    '#maxlength' => 80,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe')
  );

  return $form;
}

/*
 * Implemnts hook_form_validate().
 */
function trfs_email_subscriptions_form_validate($form, &$form_state){
  // regular expression to validate for a valid email address
  $regex = "/[a-zA-Z0-9.!#$%&'*+\/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$/";
  // return form error if it doesnt match
  if (empty($form_state['values']['email']) || !preg_match($regex, $form_state['values']['email'])) {
    form_set_error('email');
  }
}

/*
 * Implemnts hook_form_submit().
 */
function trfs_email_subscriptions_form_submit($form, &$form_state){

  // Initialize the trfsEmailSubscriptions object and set the user email
  // call the function of that object that post the email
  $trfsEmail = new trfsEmailSubscriptions();
  $trfsEmail->set_email($form_state['values']['email']);
  $result = $trfsEmail->setUrlAndGo('email_subscription');

  if($result){

    drupal_set_message('You are subscribed.');

    try{
      // store the email in our database just to have another source of this data
      $eid = db_insert('trfs_email_subscriptions_emails')
      ->fields(array(
        'email' => $form_state['values']['email'],
        'email_timestamp' => REQUEST_TIME,
      ));
      $eid->execute();

    }
    catch (Exception $e) {
      watchdog("trfs_email_subscriptions_form_submit", '<pre>' . print_r( $e->getMessage(), true) . '</pre>', array(), WATCHDOG_ERROR);
    }

  }
  elseif(isset($trfsEmail->errors)){
    // print any errors that was returned from the mailchimp API
    drupal_set_message($trfsEmail->errors, 'error');
  }
  else{
    // failback message if call fails unexpectedly
    drupal_set_message('We are currenlty experiencing issues.', 'error');
  }

  
}

?>