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
 * Run 'dcycle-test'.
 *
 * For each test which passes the preflight, run the test.
 */
function drush_dcycle_test() {
  $args = drush_parse_args();
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
    $request = drupal_http_request($base_url . '/admin/dcycle/identifier');
    if (isset($request->data)) {
      $decoded = drupal_json_decode($request->data);
      if ($decoded['identifier'] == dcycle_menu_identifier()) {
        drush_log($base_url . ' and this site are one and the same', 'ok');
      }
      else {
        drush_log('Cannot communicate with ' . $base_url . '; are you sure it is the same site as ' . getcwd() . '? ' . $base_url . '/admin/dcycle/identifier is expected to return ' . dcycle_menu_identifier(), 'error');
      }
    }
  }
  else {
    drush_log('please set base_url in the settings file', 'error');
  }

  foreach (dcycle_module_simpletests($module) as $file) {
    dcycle_exec('cd ' . getcwd() . ';php scripts/run-tests.sh --verbose --color --file "' . $file . '"');
  }
}

function dcycle_exec($command) {
  $output = array();
  $durations = variable_get('dcycle_command_durations', array());
  drush_log('About to call ' . $command . ' with ' . $durations[md5($command)]['time'], 'ok');
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
   // drush_log($line, 'ok');
  }
}


function dcycle_drush_get_tests($modules) {
  foreach ($modules as $module) {
    drush_log('grabbing all tests for ' . $module, 'ok');
  }
}

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
