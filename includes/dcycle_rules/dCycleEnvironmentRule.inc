<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dCycleBaseRule');

class dCycleEnvironmentRule extends dCycleBaseRule {
  function getName() {
    return 'Every environment must be of type dev or prod';
  }
  function _requirements() {
    $requirements = array();
    $environment = $this->getPolicyAttribute('environment', array('equals' => array('dev', 'prod')));
    $requirements['env'] = array(
      'title' => t('Environment'),
      'value' => $environment?$environment:'Not yet set, please set it',
      'severity' => $environment?REQUIREMENT_INFO:REQUIREMENT_ERROR,
      'weight' => -1,
    );
    return $requirements;
  }
}
