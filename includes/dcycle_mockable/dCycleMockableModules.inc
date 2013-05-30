<?php

module_load_include('inc', 'dcycle', 'includes/dcycle_mockable/dcycleMockableBase');

abstract class dcycleMockableModules extends dcycleMockableBase {
  abstract public function modules();

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
  /**
   * List of mock active modules.
   */
  private static $modules;

  /**
   * Add a module to the mock list of modules.
   *
   * This can be used during testing or development to see how the system
   * reacts to the existence of a particular module.
   *
   * @param $module
   *   A module in the format array('machine' => 'human')
   */
  public function addModule($module) {
    self::$modules = array_merge(self::$modules, $module);
  }

  public function modules() {
    $return = array();
    if (self::$modules) {
      $return = self::$modules;
    }
    return $return;
  }
}

class dcycleRealModules extends dcycleMockableModules {
  public function modules() {
    return module_list();
  }
}
