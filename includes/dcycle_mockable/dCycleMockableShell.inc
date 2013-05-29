<?php

dcycle_load_include('inc', 'dcycle', 'includes/dcycle_mockable/dcycleMockableBase');

abstract class dcycleMockableShell extends dcycleMockableBase {
  abstract public function shell_exec($command);

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

class dcycleMockShell extends dcycleMockableShell {
  private static $drush;
  public static function setDrush($drush) {
    self::$drush = $drush;
  }
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

class dcycleRealShell extends dcycleMockableShell {
  public function shell_exec($command) {
    return shell_exec($command);
  }
}
