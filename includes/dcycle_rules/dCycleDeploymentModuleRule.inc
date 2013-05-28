<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dCycleBaseRule');

class dCycleDeploymentModuleRule extends dCycleBaseRule {
  function getName() {
    return 'A deployment module must exist and define the website\'s namespace, in the form sites/*/modules/custom/[namespace]_deploy.';
  }
  function _requirements() {
    $deployment_module = dcycle_deployment_module();
    $requirements['deployment_module'] = array(
      'title' => t('dCycle Deployment module'),
      'value' => $deployment_module?$deployment_module:'Please create and enable a deployment module at sites/*/modules/custom/[namespace]_deploy',
      'severity' => $deployment_module?REQUIREMENT_OK:REQUIREMENT_ERROR,
    );
    $namespace = dcycle_namespace();
    $requirements['namespace'] = array(
      'title' => t('dCycle namespace'),
      'description' => t('The namespace is derived from the deployment module\'s name by stripping off _deploy.'),
      'value' => $namespace?$namespace:'Please create a deployment and enable module at sites/*/modules/custom/[namespace]_deploy to set your namespace',
      'severity' => $namespace?REQUIREMENT_OK:REQUIREMENT_ERROR,
      'weight' => -1,
    );
    return $requirements;
  }
}
