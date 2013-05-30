<?php

/**
 * @file
 * Rule stating that dcycle should be aware of its (dev, prod) environment.
 */

// get base class
module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dcycleBaseRule');

/**
 * Class for rule stating that dcycle should be aware of its environment.
 */
class dcycleEnvironmentRule extends dcycleBaseRule {
  /**
   * Overrides getName().
   */
  function getName() {
    return 'Every environment must be of type dev or prod';
  }

  /**
   * Overrides _requirements().
   */
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
