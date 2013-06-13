<?php

/**
 * @file
 * Defines the dcycle policy
 */

// load the parent class
module_load_include('inc', 'dcycle', 'includes/dcycle_policies/base/dcycleBasePolicy');

/**
 * The dcycle Quickstart policy.
 *
 * This policy is meant to used when doing demos, tutorials, etc.
 * It is meant to be a general-purpose robust policy.
 */
class dcycleQuickstartPolicy extends dcycleBasePolicy {
  /**
   * An info associative array with various policy settings.
   */
  private $info;

  /**
   * Constructor, setting the info array
   *
   * @param $info
   *   An associative array with whatever information you want.
   */
  function __construct($info = array()) {
    $this->info = $info;
  }

  /**
   * Overrides _getRules().
   */
  function _getRules() {
    return array(
      $this->getRuleObject('dcycle', 'dcycleEnvironmentRule'),
      $this->getRuleObject('dcycle', 'dcycleSimpletestRule'),
      $this->getRuleObject('dcycle', 'dcycleSiteVersionRule'),
      $this->getRuleObject('dcycle', 'dcycleDeploymentModuleRule'),
      $this->getRuleObject('dcycle', 'dcycleCoderReviewRule'),
      $this->getRuleObject('dcycle', 'dcycleModuleDevRule'),
    );
  }

  /**
   * Gets the version of this policy.
   *
   * @TODO think more about what to do when there are more than one versions
   * of a given policy.
   */
  function getVersion() {
    return 'dev';
  }

  /**
   * Gets the human name of the policy.
   */
  function getName() {
    return 'dcycle';
  }

  /**
   * Dcycle defines the concept of an environment type: dev or prod
   *
   * @TODO is this really needed? Devops states there should not really
   * be a distinction between environments.
   */
  function getEnvironmentType() {
    if (isset($this->info['environment'])) {
      $return = $this->info['environment'];
    }
    else {
      $return = dcycle_environment_type();
    }
    return $return;
  }

  /**
   * Are we developing a website or a module?
   *
   * @TODO This should be in the rule, not the policy.
   */
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

  /**
   * Overrides _getAttribute().
   */
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

  /**
   * Overrides dcycleBasePolicy::renderedSettings().
   */
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

  /**
   * Overrides dcycleBasePolicy::policyFromForm().
   */
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
    $conf = variable_get('dcycle', array()) {
      if (isset($conf['modules']) && is_array($conf['modules']) && count($conf['modules'])) {
        foreach ($conf['modules'] as $machine => $info) {
          return $machine;
        }
      }
    }
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
