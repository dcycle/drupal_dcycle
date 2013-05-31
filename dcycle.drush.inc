<?php

/**
 * @file
 * Drush integration for dcycle
 */

/**
 * Implements hook_drush_command().
 *
 * These are the dcycle commands available when you type "drush" on the
 * terminal.
 */
function dcycle_drush_command() {
  $items['dcycle-test'] = array(
    'description' => dt('Checks to see which tests can be run.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'module' => 'A Drupal module.',
    ),
  );
  $items['dcycle-mock'] = array(
    'description' => dt('dcycle-mock mock starts using mock object; dcycle-mock real stops using mock objects.'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'mock-type' => 'Mock type should be real or mock.',
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
      $help = dt('Checks to see which tests can be run, then runs them.');
      return $help;
  }
}

/**
 * Run 'dcycle-mock'.
 *
 * Toggles using mock objects. Usage is "dcycle-mock mock" or "dcycle-mock 
 * real".
 */
function drush_dcycle_mock() {
  dcycle_mockable_set_type($args[1]);
}

/**
 * Run 'dcycle-test'.
 *
 * For each test which passes the preflight, run the test.
 */
function drush_dcycle_test() {
  $module = $args[1];

  // start by preflighting simpletest
  _dcycle_check_command_exists('drush', 'drush is required, and should be in the PATH for this user (not called by its full path, or from an alias in .bash_profile)', 'http://drupal.org/project/drush');
  if (module_exists('simpletest')) {
    drush_log('Simpletest module is active', 'ok');
  }
  else {
    drush_log('Simpletest module is inactive; please activate it', 'error');
  }
  global $base_url;
  if ($base_url && $base_url != 'http://default') {
    drush_log('base_url is set in the settings file to ' . $base_url, 'ok');
    $expected_identifier = dcycle_menu_identifier();
    $request = drupal_http_request($base_url . '/admin/dcycle/identifier');
    if (isset($request->data)) {
      $decoded = drupal_json_decode($request->data);
      if ($decoded['identifier'] == $expected_identifier) {
        drush_log($base_url . ' and this site are one and the same', 'ok');
      }
      else {
        drush_log('Cannot communicate with ' . $base_url . '; are you sure it is the same site as ' . getcwd() . '? ' . $base_url . '/admin/dcycle/identifier is expected to return ' . drupal_json_encode(array('identifier' => $expected_identifier)) . ' but returns ' . $request->data . ' instead.', 'error');
      }
    }
    else {
      drush_log('Cannot communicate with ' . $base_url . '/admin/dcycle/identifier.', 'error');
    }
  }
  else {
    drush_log('please set base_url in the settings file', 'error');
  }

  dcycle_policy_object()->testAll(new drushObject);
}

/**
 * Object representing drush
 */
class drushObject {
  /**
   * Return TRUE if simulation module (--simulate called on command line)
   */
  function simulate() {
    return drush_get_context('DRUSH_SIMULATE');
  }
}

/**
 * Execute a command.
 *
 * This function also maintains a history of how long this normally takes.
 */
function dcycle_exec($command, $drushObject = NULL) {
  if ($drushObject && $drushObject->simulate()) {
    drush_log('Simulating command call', 'ok');
    drush_log('Command: ' . $command, 'ok');
  }
  else {
    $output = array();
    $durations = variable_get('dcycle_command_durations', array());
    drush_log('About to call ' . $command, 'ok');
    if (!isset($durations[md5($command)]['time'])) {
      drush_log('Not sure how long this will take.');
      $durations[md5($command)] = array(
        'time' => 0,
        'count' => 0,
      );
    }
    else {
      drush_log('This generally takes about ' . $durations[md5($command)]['time'] . ' seconds', 'ok');
    }
    $start = time();
    exec($command, $output);
    $durations[md5($command)]['time'] = time() - $start;
    $durations[md5($command)]['count']++;
    variable_set('dcycle_command_durations', $durations);
    
    foreach ($output as $line) {
      drush_log($line, 'ok');
    }
  }
}

/**
 * Make sure a command exists in the PATH.
 */
function _dcycle_check_command_exists($cmd, $why, $where) {
  if (_dcycle_command_exist($cmd)) {
    drush_log(dt('@cmd seems to be available for user @user.', array('@cmd' => $cmd, '@user' => get_current_user())), 'ok');
  }
  else {
    drush_log(dt('@cmd needs to be available for the PATH of user @user. @why. You can get @cmd at @where', array('@cmd' => $cmd, '@user' => get_current_user(), '@why' => $why, '@where' => $where)), 'error');
  }
}

/**
 * Checks whether a command exists in the command line
 */
function _dcycle_command_exist($cmd) {
  $return_val = dcycle_mockable('shell_exec', 'which ' . $cmd);
  return (empty($return_val) ? FALSE : TRUE);
}
