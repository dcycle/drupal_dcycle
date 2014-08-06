<?php

/**
 * @file
 * Drush integration for Dcycle
 */

/**
 * Implements hook_drush_command().
 *
 * These are the Dcycle commands available when you type "drush" on the
 * terminal.
 */
function dcycle_drush_command() {
  $items['dcycle-test'] = array(
    'description' => dt('Runs tests as defined in module.drush.inc.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'module' => 'A Drupal module.',
    ),
  );
  $items['dcycle-check'] = array(
    'description' => dt('Run a Check, and keep metrics and artifacts.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'module' => 'A Dcycle Check class.',
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function dcycle_drush_help($section) {
  switch ($section) {
    case 'drush:dcycle-test':
      $help = dt('Runs tests as defined in module.drush.inc.');
      return $help;
  }
}

// We must call interface_exists here because during the call
// to drush en dcycle, the interface is not present yet.
if (interface_exists('DcycleLog')) {
  class DcycleDrushLog implements DcycleLog {
    public function log($text, $vars = array()) {
      drush_log(dt($text, $vars), 'ok');
    }
  
    public function error($text, $vars = array()) {
      $this->log('DCYCLE_FAILURE');
      drush_set_error('DCYCLE_ERROR', dt($text, $vars));
      drush_set_context('DRUSH_ERROR_CODE', 1);
      // we need this for jenkins to get 1 to show up in $? With drush_die(1)
      // $? returns 0 in the command line.
      die(1);
    }
  }
}

/**
 * Run 'dcycle-test'.
 */
function drush_dcycle_test() {
  $args = func_get_args();
  $module = $args[0];

  dcycle_test($module, new DcycleDrushLog);
}

/**
 * Run 'dcycle-test'.
 */
function drush_dcycle_check() {
  $args = func_get_args();
  $class = $args[0];

  dcycle_check($class, new DcycleDrushLog);
}
