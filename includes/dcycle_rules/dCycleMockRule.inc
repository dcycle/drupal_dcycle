<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dCycleBaseRule');

class dCycleMockRule extends dCycleBaseRule {
  /**
   * Overrides dCycleBaseRule::getName().
   */
  function getName() {
    return 'Mock objects must be properly used.';
  }

  /**
   * Overrides dCycleBaseRule::peerReviewChecklist().
   */
  function peerReviewChecklist() {
    return array(
      'mock_object_usage' => array(
        'description' => 'Interactions with outside systems must go through mock objects.',
      ),
      'mock_object_no_rand' => array(
        'description' => 'If used, mock objects must not contain random data if they are used for automatic testing.',
      ),
    );
  }
}
