<?php

/**
 * @file
 * Mock object definition for dealing with the shell.
 */

// Include parent class file.
dcycle_load_include('inc', 'dcycle', 'includes/dcycle_mockable/dcycleMockableBase');

/**
 * Abstract class for dealing with the shell
 */
abstract class dcycleMockableShell extends dcycleMockableBase {
  /**
   * Abstract shell_exec() function.
   */
  abstract public function shell_exec($command);

  /**
   * Get the concrete (real or mock) class.
   */
  public static function getConcreteClass() {
    $type = self::getType();

    switch ($type) {
      case 'real':
        return 'dcycleRealShell';
        break;
      case 'mock':
        return 'dcycleMockShell';
        break;
      default:
        return NULL;
        break;
    }
  }
}

/**
 * Mock object for dealing with the shell.
 */
class dcycleMockShell extends dcycleMockableShell {
  /**
   * The location of drush, as in which drush.
   */
  private static $drush;

  /**
   * Settre for self::$drush, called during testing and development.
   */
  public static function setDrush($drush) {
    self::$drush = $drush;
  }

  /**
   * Mock shell_exec() function.
   */
  public function shell_exec($command) {
    $return = array();
    switch ($command) {
      case 'which drush':
        if (self::$drush) {
          $return[] = self::$drush;
        }
        break;
      default:
        $return[] = '[error] mock shell does not recognize command.';
        break;
    }
    return $return;
  }
  
}

/**
 * Real object for dealing with the shell.
 */
class dcycleRealShell extends dcycleMockableShell {
  /**
   * Real shell_exec() function.
   */
  public function shell_exec($command) {
    return shell_exec($command);
  }

}
