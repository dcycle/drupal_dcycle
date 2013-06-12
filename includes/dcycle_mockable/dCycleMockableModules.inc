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
   * Returns the list of active modules
   *
   * @param $options
   *   An associative array in the format:
   *   array(
   *     'type' => 'module'|'theme',
   *     'name' => 'machine name',
   *   )
   */
  abstract public function drupal_get_path($options);

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
   * Add a module to the mock list of modules.
   *
   * This can be used during testing or development to see how the system
   * reacts to the existence of a particular module.
   *
   * @param $module
   *   A module in the format array('machine' => 'human')
   */
  public static function addModule($module) {
    $modules = self::staticModules();
    $modules = array_merge($modules, $module);
    variable_set('dcycle_mockable_modules_list', $modules);
  }

  /**
   * Overrides dcycleMockableModules::drupal_get_path().
   */
  public function drupal_get_path($options) {
    foreach (variable_get('dcycle_mockable_modules_list', array()) as $module => $info) {
      if ($options['name'] == $module) {
        if (isset($info['path'])) {
          return $info['path'];
        }
        else {
          return 'sites/all/modules/' . $module;
        }
      }
    }
  }

  public static function staticModules() {
    $return = array();
    foreach (variable_get('dcycle_mockable_modules_list', array()) as $module => $info) {
      $return[$module] = $module;
    }
    return $return;
  }

  /**
   * Overrides dcycleMockableModules::modules().
   */
  public function modules() {
    return self::staticModules();
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

  /**
   * Overrides dcycleMockableModules::drupal_get_path().
   */
  public function drupal_get_path($options) {
    return drupal_get_path($options['type'], $options['name']);
  }
}
