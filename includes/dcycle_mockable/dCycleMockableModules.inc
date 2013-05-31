<?php

/**
 * @file
 * Defines a mock object for getting a list of modules.
 */

// load the base class
module_load_include('inc', 'dcycle', 'includes/dcycle_mockable/dcycleMockableBase');

/**
 * @file
 * Defines a mock object for getting a list of modules.
 */

/**
 * Mockable class for a list of active modules.
 */
abstract class dcycleMockableModules extends dcycleMockableBase {
  /**
   * Returns the list of active modules
   *
   * @return
   *   An associative array in the format:
   *   array(
   *     'machine' => 'Human',
   *     ...
   *   )
   */
  abstract public function modules();

  /**
   * Based on the type (real or mock) return the class to use.
   *
   * @return
   *   The name of a class defined in this file.
   */
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

/**
 * Mock object for a list of active modules.
 */
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
  public static function addModule($module) {
    self::$modules = array_merge(self::$modules, $module);
  }

  /**
   * Overrides dcycleMockableModules::modules().
   */
  public function modules() {
    $return = array();
    if (self::$modules) {
      $return = self::$modules;
    }
    return $return;
  }
}

/**
 * Real object for a list of active modules.
 */
class dcycleRealModules extends dcycleMockableModules {
  /**
   * Overrides dcycleMockableModules::modules().
   */
  public function modules() {
    return module_list();
  }
}
