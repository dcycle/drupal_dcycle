<?php

/**
 * @file
 * Rule: Simpletest must be defined for the module under dev.
 */

// load parent class
module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dcycleBaseRule');

/**
 * Rule class: Simpletest must be defined for the module under dev.
 */
class dcycleSimpletestRule extends dcycleBaseRule {
  /**
   * Overrides getName().
   */
  function getName() {
    return t('Simpletests must be defined for @modules', array('@modules' => $this->getPolicyAttribute('simpletest_module_description', array('type' => 'string'))));
  }
  
  /**
   * Overrides _requirements().
   */
  function _requirements() {
    return array();
  }

  /**
   * Overrides test().
   */
  function test($drushObject) {
    $policy_attr = $this->getPolicyAttribute('simpletest_module_description', array('type' => 'string'));
    $files = dcycle_module_simpletests($policy_attr);
    foreach ($files as $file) {
      dcycle_exec('cd ' . getcwd() . ';php scripts/run-tests.sh --verbose --color --file "' . $file . '"', $drushObject);
    }
  }

}
