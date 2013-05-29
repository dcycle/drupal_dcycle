<?php

/**
 * @file
 * Defines the dcycle policy
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_policies/base/dcycleBasePolicy');

class dcycleQuickstartPolicy extends dcycleBasePolicy {
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
      $this->getRuleObject('dcycle', 'dcycleModuleDevRule'),
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

  function getProjectType() {
    return variable_get('dcycle_project_type', $this->discoverProjectType());
  }

  /**
   * Attempt to figure out whether we are developing a module or website
   *
   * Jenkins should be able to determine this automatically based on
   * - if exactly one module is under git, then assume that is the module we're
   *   checking and developing
   * - if not, assume we're developing a website.
   * see http://drupal.org/node/2007096#comment-7473482
   *
   * @return
   *   A project type, 'website' or 'module'
   */
  function discoverProjectType() {
    if ($this->getRuleObject('dcycle', 'dcycleModuleDevRule')->checkReqs()) {
      return 'module';
    }
    else {
      return 'website';
    }
  }

  function _getAttribute($name) {
    switch ($name) {
      case 'environment':
        return $this->getEnvironmentType();
        break;
      case 'project_type':
        return $this->getProjectType();
        break;
      case 'simpletest_module_description':
        return $this->getTargetModule();
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
      '#default_value' => $this->getProjectType(),
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

  /**
   * Return the target module for this environment
   *
   * Dcycle requires that there be exactly one target module:
   * - 
   */
  function getTargetModule() {
    $moduleDevRule = $this->getRuleObject('dcycle', 'dcycleModuleDevRule');
    if ($moduleDevRule->checkReqs()) {
      // we know there is only one because that is the requirement that
      // was just checked
      $modules_under_git = $moduleDevRule->modulesUnderGit();
      foreach ($modules_under_git as $module_under_git => $human) {
        return $module_under_git;
      }
    }
    else {
      return $this->getRuleObject('dcycle', 'dcycleDeploymentModuleRule')->deploymentModule();
    }
  }
  
}
