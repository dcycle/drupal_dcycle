<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dcycleBaseRule');

class dcycleSimpletestRule extends dcycleBaseRule {
  function getName() {
    return t('Simpletests must be defined for @modules', array('@modules' => $this->getPolicyAttribute('simpletest_module_description', array('type' => 'string'))));
  }
  function _requirements() {
    return array();
  }
  function test() {
    $policy_attr = $this->getPolicyAttribute('simpletest_module_description', array('type' => 'string'));
    $files = dcycle_module_simpletests($policy_attr);
    foreach ($files as $file) {
      dcycle_exec('cd ' . getcwd() . ';php scripts/run-tests.sh --verbose --color --file "' . $file . '"');
    }
  }
}
