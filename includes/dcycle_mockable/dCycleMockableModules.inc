<?php

module_load_include('inc', 'dcycle', 'includes/dcycle_mockable/dcycleMockableBase');

abstract class dcycleMockableModules extends dcycleMockableBase {
  abstract public function deployment_modules();

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
        throw new DcycleException('Only the types real and mock are supported.');
        break;
    }
  }
}

class dcycleMockModules extends dcycleMockableModules {
  private static $deployment_modules;
  public static function setDeploymentModules($modules) {
    self::$deployment_modules = $modules;
  }
  public function deployment_modules() {
    $return = array();
    if (self::$deployment_modules) {
      $return[] = self::$deployment_modules;
    }
    return $return;
  }
}

class dcycleRealModules extends dcycleMockableModules {
  public function deployment_modules() {
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
