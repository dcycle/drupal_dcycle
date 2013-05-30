<?php

/**
 * @file
 * Mock objects must be properly used.
 */

// load parent
module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dcycleBaseRule');

/**
 * @file
 * Rule class to ensure that mock objects must be properly used.
 */
class dcycleMockRule extends dcycleBaseRule {
  /**
   * Overrides dcycleBaseRule::getName().
   */
  function getName() {
    return 'Mock objects must be properly used.';
  }

  /**
   * Overrides dcycleBaseRule::peerReviewChecklist().
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
