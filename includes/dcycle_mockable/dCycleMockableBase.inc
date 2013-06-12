<?php
/**
 * @file
 */

/**
 * For testing, never use random data in subclasses
 */
abstract class dcycleMockableBase {
  /**
   * return an object which is an instance of a subclass
   * of the abstract $class, depending on the context,
   * 'real' or 'mock'
   *
   * @param $class
   *   Base class. The class should already be loaded before this is called.
   *
   * @throws DcycleException
   */
  static function getMockable($base_class) {
    $class = $base_class::getConcreteClass();
    if (class_exists($class)) {
      return new $class;
    }
    else {
      throw new DcycleException(t('@base_class::getConcreteClass() is expected to return a concrete class, but returned "@return" which does not pass class_exists(); please see the API for dcycle_mockable() for details on how to use and extend the mock object system.', array('@base_class' => $base_class, '@return' => serialize($class))));
    }
  }

  static private $type;

  /**
   * Get the type, normally either real or mock
   */
  static function getType() {
    return variable_get('dcycle_mockable_base_type', array());
  }

  /**
   * Set the type, normally either real or mock
   */
  static function setType($type, $more = array()) {
    variable_set('dcycle_mockable_base_type', $type);
    variable_set('dcycle_mockable_base_more', $more);
  }
}
