<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dCycleBaseRule');

class dCycleSiteVersionRule extends dCycleBaseRule {
  function getName() {
    return 'The site version corresponds to the deployment module version, and the suffix -dev in the development environment.';
  }
  function _requirements() {
    $requirements = array();
    $site_version = dcycle_site_version();
    $requirements['site_version'] = array(
      'title' => t('Site version', array()),
      'value' => $site_version?$site_version:'Not yet set, please set it',
      'severity' => $site_version?REQUIREMENT_INFO:REQUIREMENT_ERROR,
      'weight' => -1,
    );
    return $requirements;
  }
}
