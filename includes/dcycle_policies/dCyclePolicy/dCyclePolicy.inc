<?php

/**
 * @file
 * Defines the dcycle policy
 */

// load the base class.
module_load_include('inc', 'dcycle', 'includes/dcycle_policies/base/dcycleQuickstartPolicy');

/**
 * Defines the dcycle policy, meant to be more complete than quickstart.
 */
class dcyclePolicy extends dcycleQuickstartPolicy {
  /**
   * Overrides _getRules().
   */
  function _getRules() {
    return array_merge(parent::_getRules(), array(
      $this->getRuleObject('dcycle', 'dcycleBranchesRule'),
    ));
  }
}
