<?php

class DcycleEnvironment {
  static function Exec($command, &$lines = array(), &$result = NULL) {
    if (self::ExecAvailable()) {
      return exec($command, $lines, $result);
    }
    else {
      throw new Exception(t('exec() function is not available.'));
    }
  }

  static function ExecResult($command) {
    $lines = array();
    $result = NULL;
    self::Exec($command, $lines, $result);
    return $result;
  }

  static function DrushExists() {
    return (self::ExecResult(self::DrushPath()) == 0);
  }

  static function DrushCommand($command, &$lines = array(), &$result = NULL) {
    return self::Exec(self::DrushPath() . ' ' . $command, $lines, $result);
  }

  static function DrushCommandExists($command) {
    return self::DrushCommand('help ' . $command);
  }

  static function DrushPath() {
    return variable_get('dcycle_drush_path', 'drush');
  }

  static function ExecAvailable() {
    return function_exists('exec');
  }
}
