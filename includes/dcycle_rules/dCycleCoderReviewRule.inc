<?php

/**
 * @file
 * A rule stating that all code must go through the coder module for review.
 */

// load the parent class
module_load_include('inc', 'Dcycle', 'includes/dcycle_rules/dcycleModuleRule');

/**
 * Class for the coder review rule
 */
class dcycleCoderReviewRule extends dcycleModuleRule {
  /**
   * Returns modules affected by this rule
   */
  function _getModules() {
    return array('coder_review');
  }

  /**
   * Overrides getDescription().
   */
  function getDescription() {
    return 'it allows Jenkins to test reviews at each commit.';
  }

  /**
   * Override getEnvironmentsOn().
   */
  function getEnvironmentsOn() {
    return array('dev');
  }

  /**
   * Override getEnvironmentsOff().
   */
  function getEnvironmentsOff() {
    return array('prod');
  }

  /**
   * Overrides dcycleBaseRule::test().
   */
  function test($drushObject) {
    $policy_attr = $this->getPolicyAttribute('simpletest_module_description', array('type' => 'string'));
    dcycle_exec('drush coder-review --minor --comment --security ' . $policy_attr, $drushObject);
  }

}
