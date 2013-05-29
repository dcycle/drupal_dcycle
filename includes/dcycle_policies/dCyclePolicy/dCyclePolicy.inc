<?php

/**
 * @file
 * Defines the dcycle policy
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_policies/base/dcycleQuickstartPolicy');

class dcyclePolicy extends dcycleQuickstartPolicy {
  private $info;
  function __construct($info = array()) {
    $this->info = $info;
  }
  function _getRules() {
    return array_merge(parent::_getRules(), array(
      $this->getRuleObject('dcycle', 'dcycleBranchesRule'),
    ));
  }
}
