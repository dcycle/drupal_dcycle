<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dCycleBaseRule');

class dCycleSimpletestRule extends dCycleBaseRule {
  function getName() {
    return t('Simpletests must be defined for @modules', array('@modules' => $this->getPolicyAttribute('simpletest_module_description', array('type' => 'string'))));
  }
  function _requirements() {
    return array();
  }
}
