<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dcycleModuleRule');

class dcycleCoderReviewRule extends dcycleModuleRule {
  function _getModules() {
    return array('coder_review');
  }
  function getDescription() {
    return 'it allows Jenkins to test reviews at each commit.';
  }
  function getEnvironmentsOn() {
    return array('dev');
  }
  function getEnvironmentsOff() {
    return array('prod');
  }

  /**
   * Overrides dcycleBaseRule::test().
   */
  function test() {
    $policy_attr = $this->getPolicyAttribute('simpletest_module_description', array('type' => 'string'));
    dcycle_exec('drush coder-review --minor --comment --security ' . $policy_attr);
  }
}
