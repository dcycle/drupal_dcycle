<?php

/**
 * @file
 * these functions are never called, they are here for documentation
 * purposes only.
 */

/**
 * Let dcycle know which functions are mockable.
 *
 * Please see the example in the description of dcycle_mockable().
 *
 * @return
 *   An associative array, with the keys being the functions. Each
 *   element should contain the module and the base class. The base class
 *   is the file name which resides in
 *   yourmodule/includes/dcycle_mockable/className.inc. To see an example
 *   of what that file should look like please see
 *   dcycle/includes/dcycle_mockable/dcycleMockableShell.inc
 */
function hook_dcycle_mockables() {
  $mockables = array();

  $mockables['shell_exec'] = array(
    'module' => 'dcycle',
    'base_class' => 'dcycleMockableShell',
  );
  $mockables['modules'] = array(
    'module' => 'dcycle',
    'base_class' => 'dcycleMockableModules',
  );

  return $mockables;
}
