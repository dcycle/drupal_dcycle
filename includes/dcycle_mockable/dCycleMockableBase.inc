<?php
/**
 * @file
 */

/**
 * For testing, never use random data in subclasses
 */
abstract class dCycleMockableBase {
  // return an object which is an instance of a subclass
  // of the abstract $class, depending on the context,
  // 'real' or 'mock'
  static function getMockable($class) {
    $class = $class::getConcreteClass();
    if (class_exists($class) && ($object = new $class)) {
      return $object;
    }
    else {
      throw new Exception('Could not create a valid object from class');
    }
  }

  static private $type;

  static function getType() {
    return self::$type;
  }

  static function setType($type) {
    self::$type = $type;
  }
}
