<?php

/**
 * @file
 * Functions used during Dcycle development.
 * See http://dcycleproject.org/
 */

/**
 * Administrative settings callback.
 */
function dcycle_settings() {
  $form = array();

  if (dcycle_policy_exists(dcycle_policy())) {
    $form = array_merge($form, dcycleBasePolicy::_renderedSettings(dcycle_policy()));
  }
  else {
    drupal_set_message(t('The selected policy could not be found.'), 'error', FALSE);
  }

  $form['dcycle_policy'] = array(
    '#type' => 'select',
    '#title' => t('Policy'),
    '#default_value' => variable_get('dcycle_policy', 'dcycleQuickstartPolicy'),
    '#options' => dcycle_get_available_policies(),
    '#description' => t('Select which policy you will be using for your development cycle. Different policies enforce different sets of rules.'),
  );

  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Environment status'),
  );

  // The environment status is simply the requirements set by
  // Dcycle. These same requirements appear in the admin/reports/status
  // page. The prefix and suffix allow the contents of the requirements to
  // be
  module_load_include('inc', 'Dcycle', 'includes/dcycle_policies/base/dcycleBasePolicy');
  $form['status']['dcycle_status'] = array(
    '#type' => 'markup',
    '#markup' => dcycleBasePolicy::_renderedRequirements(dcycle_policy()),
    '#prefix' => '<div id="_dcycle_ajax_status_wrapper">',
    '#suffix' => '</div>',
  );

  return system_settings_form($form);
}

/**
 * Ajax callback for requirements on the administrative screen.
 */
function _dcycle_ajax_settings($form, $form_state) {
  // figure out the desired environment and project type.
  // The environment can be dev or prod; the project can
  // be module or website.
  return dcycle_policy()->policyFromForm($form)->renderedRequirements();
}
