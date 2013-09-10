<?php

/**
 * @file
 * deployment module must exist
 */

// load the parent class
module_load_include('inc', 'Dcycle', 'includes/dcycle_rules/dcycleBaseRule');

/**
 * deployment module must exist rule class
 */
class dcycleDeploymentModuleRule extends dcycleBaseRule {
  /**
   * Overrides getName().
   */
  function getName() {
    return 'A deployment module must exist and define the website\'s namespace, in the form sites/*/modules/custom/[namespace]_deploy.';
  }

  /**
   * Overrides dcycleBaseRule::usedForPolicy().
   *
   * Use this rule only for the dev environment.
   */
  function usedForPolicy() {
    return in_array($this->getPolicyAttribute('project_type', array('equals' => array('website', 'module'))), array('website'));
  }

  /**
   * Tries to find a deployment module.
   */
  function deploymentModule() {
    return dcycle_deployment_module();
  }

  /**
   * Overrides _requirements().
   */
  function _requirements() {
    $deployment_module = dcycle_deployment_module();
    $requirements['deployment_module'] = array(
      'title' => t('dcycle Deployment module'),
      'value' => $deployment_module?t('Your deployment module is @module', array('@module' => $deployment_module)):'Please create and enable a deployment module at sites/*/modules/custom/[namespace]_deploy',
      'severity' => $deployment_module?REQUIREMENT_OK:REQUIREMENT_ERROR,
    );
    $namespace = dcycle_namespace();
    $requirements['namespace'] = array(
      'title' => t('dcycle namespace'),
      'description' => t('The namespace is derived from the deployment module\'s name by stripping off _deploy.'),
      'value' => $namespace?$namespace:'Please create a deployment and enable module at sites/*/modules/custom/[namespace]_deploy to set your namespace',
      'severity' => $namespace?REQUIREMENT_OK:REQUIREMENT_ERROR,
      'weight' => -1,
    );
    return $requirements;
  }

}
