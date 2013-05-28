<?php

abstract class dCycleMockableShell extends dCycleMockableBase {
  abstract public function shell_exec($command);

  public static function getConcreteClass() {
    $type = self::getType();

    switch ($type) {
      case 'real':
        return 'dCycleRealShell';
        break;
      case 'mock':
        return 'dCycleMockShell';
        break;
      default:
        return NULL;
        break;
    }
  }
}

class dCycleMockShell extends dCycleMockableShell {
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

class dCycleRealShell extends dCycleMockableShell {
  public function shell_exec($command) {
    return shell_exec($command);
  }
}
