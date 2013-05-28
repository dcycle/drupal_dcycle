<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dCycleModuleRule');

class dCycleCoderReviewRule extends dCycleModuleRule {
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
}
