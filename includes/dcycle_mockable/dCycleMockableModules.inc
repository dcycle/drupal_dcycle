<?php

abstract class dCycleMockableModules extends dCycleMockableBase {
  abstract public function deployment_module();

  public static function getConcreteClass() {
    $type = self::getType();

    switch ($type) {
      case 'real':
        return 'dCycleRealModules';
        break;
      case 'mock':
        return 'dCycleMockModules';
        break;
      default:
        return NULL;
        break;
    }
  }
}

class dCycleMockModules extends dCycleMockableModules {
  private static $deployment_modules;
  public static function setDeploymentModules($modules) {
    self::$deployment_modules = $modules;
  }
  public function deployment_module() {
    $return = array();
    if (self::$deployment_modules) {
      $return[] = self::$deployment_modules;
    }
    return $return;
  }
}

class dCycleRealShell extends dCycleMockableShell {
  public function deployment_module() {
    $candidates = array();
    // find all candidate modules which adhere to
    // sites/*/modules/custom/*_deploy
    foreach (module_list() as $module) {
      if (drupal_substr($module, drupal_strlen($module) - drupal_strlen('_deploy')) == '_deploy' && drupal_substr(drupal_get_path('module', $module), drupal_strlen(drupal_get_path('module', $module)) - drupal_strlen('/custom/' . $module)) == '/custom/' . $module) {
        $candidates[] = $module;
      }
    }
    return $candidates;
  }
}
