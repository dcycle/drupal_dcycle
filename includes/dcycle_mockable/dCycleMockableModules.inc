<?php

abstract class dcycleMockableModules extends dcycleMockableBase {
  abstract public function deployment_module();

  public static function getConcreteClass() {
    $type = self::getType();

    switch ($type) {
      case 'real':
        return 'dcycleRealModules';
        break;
      case 'mock':
        return 'dcycleMockModules';
        break;
      default:
        return NULL;
        break;
    }
  }
}

class dcycleMockModules extends dcycleMockableModules {
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

class dcycleRealShell extends dcycleMockableShell {
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
