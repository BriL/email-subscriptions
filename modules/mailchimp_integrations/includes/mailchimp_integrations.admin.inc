<?php

/**
 * @file
 *  Mailchimp Integration Settings Form.
 */

/**
 * Implements hook_form().
 */
function mailchimp_integrations_settings_form($form) {
  // Retrieves saved variable from database.
  $mailchimp_integrations_settings = variable_get('mailchimp_integrations_settings');

  $form['api_name'] = array(
    '#title' => 'Mailchimp API Name',
    '#type' => 'textfield',
    '#default_value' => $mailchimp_integrations_settings['api_name'],
    '#description' => 'Mailchimp API Name used in Oauth.',
    '#required' => 1,
    '#maxlength' => 255,
  );

  $form['api_key'] = array(
    '#title' => 'Mailchimp API Key',
    '#type' => 'textfield',
    '#default_value' => $mailchimp_integrations_settings['api_key'],
    '#description' => 'Mailchimp API Key used from the site.',
    '#required' => 1,
    '#maxlength' => 255,
  );

  $form['subscription_letter_id'] = array(
    '#title' => 'The Subscription Letter List ID',
    '#type' => 'textfield',
    '#default_value' => $mailchimp_integrations_settings['subscription_letter_id'],
    '#description' => 'Mailchimp List Id found on the mailchimp site.',
    '#required' => 1,
    '#maxlength' => 255,
  );

  $form['contact_us_id'] = array(
    '#title' => 'The Contact Us List ID',
    '#type' => 'textfield',
    '#default_value' => $mailchimp_integrations_settings['contact_us_id'],
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

/**
 * Implements hook_form_submit().
 */
function mailchimp_integrations_settings_form_submit($form, &$form_state) {
  // Save the form values as a databse variable.
  variable_set('mailchimp_integrations_settings', $form_state['values']);

  drupal_set_message('Configurations Saved');
}
