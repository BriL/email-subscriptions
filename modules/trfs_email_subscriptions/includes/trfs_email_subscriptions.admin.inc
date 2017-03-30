<?php

/*
 * Implements hook_form().
 */
function trfs_email_subscriptions_settings_form($form){
  // Retrieves saved variable from database.
  $trfs_email_subscriptions_settings = variable_get('trfs_email_subscriptions_settings');

  $form['api_name'] = array(
    '#title' => 'Mailchimp API Name',
    '#type' => 'textfield',
    '#default_value' => $trfs_email_subscriptions_settings['api_name'],
    '#description' => 'Mailchimp API Name used in Oauth.',
    '#required' => 1,
    '#maxlength' => 255,
  );

  $form['api_key'] = array(
    '#title' => 'Mailchimp API Key',
    '#type' => 'textfield',
    '#default_value' => $trfs_email_subscriptions_settings['api_key'],
    '#description' => 'Mailchimp API Key used from the site.',
    '#required' => 1,
    '#maxlength' => 255,
  );

  $form['list_id'] = array(
    '#title' => 'The Maichimp List ID',
    '#type' => 'textfield',
    '#default_value' => $trfs_email_subscriptions_settings['list_id'],
    '#description' => 'Mailchimp List Id found on the mailchimp site.',
    '#required' => 1,
    '#maxlength' => 255,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/*
 * Implements hook_form_submit().
 */
function trfs_email_subscriptions_settings_form_submit($form, &$form_state){
  // save the form values as a databse variable
  variable_set('trfs_email_subscriptions_settings', $form_state['values']);

  drupal_set_message('Configurations Saved');
}

?>