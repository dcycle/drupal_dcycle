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

class DcycleDrushLog implements DcycleLog {
  public function log($text, $vars = array()) {
    drush_log(dt($text, $vars), 'ok');
  }

  public function error($text, $vars) {
    drush_set_error('DCYCLE_ERROR', dt($text, $vars));
    exit (1);
  }
}

/**
 * Run 'dcycle-test'.
 */
function drush_dcycle_test() {
  $args = func_get_args();
  $module = $args[1];

  dcycle_test($module, new DcycleDrushLog);
}
