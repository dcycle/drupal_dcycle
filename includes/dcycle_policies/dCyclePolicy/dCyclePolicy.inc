<?php

/**
 * @file
 * Defines the dcycle policy
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_policies/base/dcycleBasePolicy');

class dcyclePolicy extends dcycleBasePolicy {
  private $info;
  function __construct($info = array()) {
    $this->info = $info;
  }
  function _getRules() {
    return array(
      $this->getRuleObject('dcycle', 'dcycleEnvironmentRule'),
      $this->getRuleObject('dcycle', 'dcycleSimpletestRule'),
      $this->getRuleObject('dcycle', 'dcycleSiteVersionRule'),
      $this->getRuleObject('dcycle', 'dcycleDeploymentModuleRule'),
      $this->getRuleObject('dcycle', 'dcycleSimpletestRule'),
      $this->getRuleObject('dcycle', 'dcycleCoderReviewRule'),
      $this->getRuleObject('dcycle', 'dcycleBranchesRule'),
    );
  }
  function getVersion() {
    return 'dev';
  }
  function getName() {
    return 'dcycle';
  }
  function getEnvironmentType() {
    if (isset($this->info['environment'])) {
      $return = $this->info['environment'];
    }
    else {
      $return = dcycle_environment_type();
    }
    return $return;
  }
  function _getAttribute($name) {
    switch ($name) {
      case 'environment':
        return $this->getEnvironmentType();
        break;
      case 'simpletest_module_description':
        return t('the deployment module');
        break;
      default:
        return NULL;
        break;
    }
  }
  function renderedSettings() {
    $form['dcycle_environment_type'] = array(
      '#type' => 'select',
      '#title' => t('Environment type'),
      '#default_value' => variable_get('dcycle_environment_type', 'dev'),
      '#options' => array(
        'dev' => t('Development'),
        'prod' => t('Production'),
      ),
      '#description' => t('Select which type of environment this is; different types of environments will behave differently.'),
    );
    $form['dcycle_project_type'] = array(
      '#type' => 'select',
      '#title' => t('Project type'),
      '#default_value' => variable_get('dcycle_project_type', 'website'),
      '#options' => array(
        'website' => t('Website'),
        'module' => t('Module'),
      ),
      '#description' => t('Select which type of project this is; modules and websites will have different requirements.'),
    );
    return $form;
  }
  function policyFromForm($form) {
    if (isset($form['dcycle_environment_type']['#value'])) {
      return new dcyclePolicy(array('environment' => $form['dcycle_environment_type']['#value']));
    }
    else {
      return $this;
    }
  }
}
