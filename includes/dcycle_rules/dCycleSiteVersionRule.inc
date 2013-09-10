<?php

/**
 * @file
 * Rule stating that there should be a site version
 */

// load base class
module_load_include('inc', 'Dcycle', 'includes/dcycle_rules/dcycleBaseRule');

/**
 * Rule object for rule stating that there should be a site version
 */
class dcycleSiteVersionRule extends dcycleBaseRule {
  /**
   * Overrides getName().
   */
  function getName() {
    return 'The site version corresponds to the deployment module version, and the suffix -dev in the development environment.';
  }

  /**
   * Overrides _requirements().
   */
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
