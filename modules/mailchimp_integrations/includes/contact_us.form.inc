<?php

/**
 * @file
 *  Contact us form that utilizes Mailchimp Integrations on the backend.
 */

/**
 * Implemnts hook_form().
 */
function contact_us_form($form, &$form_state) {

  $form = array();

  $form['first_name'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'placeholder' => t('*First Name'),
    ),
    '#size' => 40,
    '#maxlength' => 80,
  );

  $form['last_name'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'placeholder' => t('*Last Name'),
    ),
    '#size' => 40,
    '#maxlength' => 80,
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#attributes' => array(
      'placeholder' => t('*Email Address'),
    ),
    '#size' => 40,
    '#maxlength' => 80,
  );

  $form['reason'] = array(
    '#type' => 'select',
    '#options' => array(
      '' => t('*Reason for Contact'),
      'book' => t('Booking'),
      'beats' => t('Inquire about Beats'),
      'complaints' => t('Complaints'),
      'other' => t('Other'),
    ),
  );

  $form['reason_description'] = array(
    '#type' => 'textarea',
    '#attributes' => array(
      'placeholder' => t('*Description'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

/**
 * Implemnts hook_form_validate().
 */
function contact_us_form_validate($form, &$form_state) {
  if (empty($form_state['values']['first_name'])) {
    form_set_error('first_name', t('First Name is required.'));
    $errors['first_name'] = t('First Name is required.');
  }
  if (empty($form_state['values']['last_name'])) {
    form_set_error('last_name', t('Last Name is required.'));
  }

  if (!preg_match('/^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/', $form_state['values']['email']) || empty($form_state['values']['email'])) {
    form_set_error('email', t('Email is not valid.'));
  }

  if (empty($form_state['values']['reason'])) {
    form_set_error('reason', t('Reason for contacting is required.'));
  }
  if (empty($form_state['values']['reason_description'])) {
    form_set_error('reason_description', t('Description is required.'));
  }
}

/**
 * Implemnts hook_form_submit().
 */
function contact_us_form_submit($form, &$form_state) {
  // Initialize the granderaMailchimpIntegrations object and set the user email.
  $granderaEmail = new mailchimpIntegrations('contact');
  $granderaEmail->set_fname($form_state['values']['first_name']);
  $granderaEmail->set_email($form_state['values']['email']);
  $granderaEmail->set_lname($form_state['values']['last_name']);
  $granderaEmail->set_contact_reason($form_state['values']['reason']);
  $granderaEmail->set_contact_desctiption($form_state['values']['reason_description']);

  $result = $granderaEmail->save();

  if ($result) {
    drupal_set_message('We will contact you as soon as possible', 'success');
  }
  elseif (isset($granderaEmail->errors)) {
    // Print any errors that was returned from the mailchimp API
    drupal_set_message($granderaEmail->errors, 'error');
  }
  else {
    // Failback message if call fails unexpectedly.
    drupal_set_message('We are currenlty experiencing issues.', 'error');
  }
}
