<?php

/**
 * @file
 * Defines the dcycle policy
 */

/**
 * A base class for all policies.
 *
 * Policies are classes which define which rules and other information is
 * required during development. In turn, Jenkins via drush dcycle-test check
 * the policy of the module or website you are building, and makes sure all
 * rules are respected, before merging your unstable branch to the stable
 * branch.
 */
abstract class dcycleBasePolicy {
  function requirements() {
    include_once DRUPAL_ROOT . '/includes/install.inc';
    $requirements = array();
    $requirements['dcycle_version'] = array(
      'title' => t('!d version', array('!d' => l(t('dcycle'), 'http://dcycleproject.org/'))),
      'value' => dcycle_version(),
      'severity' => REQUIREMENT_INFO,
      'weight' => -2,
    );
    $requirements['policy_version'] = array(
      'title' => t('!name policy version', array('!name' => $this->getLink())),
      'value' => $this->getVersion(),
      'severity' => REQUIREMENT_INFO,
      'weight' => -2,
    );
    $rules = $this->getRules();
    foreach ($rules as $rule) {
      $requirements = array_merge($requirements, $rule->requirements());
    }
    return $requirements;
  }
  function getLink() {
    $url = $this->getUrl();
    $name = $this->getName();
    if ($url) {
      return l($name, $url);
    }
    else {
      return $name;
    }
  }
  abstract function getName();
  abstract function getVersion();
  function getUrl() {
    // Subclasses may override this if they have a public URL describing
    // their policy
    return NULL;
  }
  function getRules() {
    $return = $this->_getRules();
    foreach ($return as $object) {
      if (!is_object($object)) {
        throw new Exception('Expecting _getRules() to return an object');
      }
    }
    return $return;
  }
  abstract function _getRules();
  function getRuleObject($module, $name) {
    module_load_include('inc', $module, 'includes/dcycle_rules/' . $name);
    return new $name($this);
  }
  function getAttribute($name, $expected) {
    $return = $this->_getAttribute($name);
    if (isset($expected['equals'])) {
      if (!in_array($return, $expected['equals'])) {
        throw new Exception(t('Required policy attribute %name did not match expected result or was NULL in policy %policy', array('%name' => $name, '%policy' => $this->getName())));
      }
    }
    return $return;
  }
  function _getAttribute($name) {
    // by default return NULL; subclasses can override this.
    return NULL;
  }

  /**
   * Display rendered requirements for a given policy
   *
   * @param policy
   *  A policy in the format of an associative array including the module and
   *  the class name. It is not necessary that this be valid. If it is not valid
   *  a failed requirement will be returned.
   *
   * @return
   *  Requirements for a policy if it exists, or a failed requirement if the
   *  policy does not exist.
   */
  static function _renderedRequirements($policy) {
    $requirements = dcycle_policy_object($policy)->requirements($policy);
    include_once DRUPAL_ROOT . '/includes/install.inc';
    return theme('status_report', array('requirements' => $requirements));
  }

  static function _requirements($policy) {
    $requirements = array();
    $policy_exists = dcycle_policy_exists($policy);
    $requirements['dcycle_policy_exists'] = array(
      'title' => t('The module @module must be active and the policy @class must be a valid class'),
      'value' => $policy_exists,
      'severity' => $policy_exists?REQUIREMENT_OK:REQUIREMENT_ERROR,
    );
    if ($policy_exists) {
      $requirements = array_merge($requirements, dcycle_policy_object($policy)->requirements());
    }
    return $requirements;
  }

  /**
   * Display rendered settings for a given policy
   *
   * @param policy
   *  A policy in the format of an associative array including the module and
   *  the class name. It is not necessary that this be valid. If it is not valid
   *  a failed requirement will be returned.
   *
   * @return
   *  Rendered settings for a policy if it exists, an empty array() if it does
   *  not.
   */
  static function _renderedSettings($policy) {
    $settings = array();
    $policy_exists = dcycle_policy_exists($policy);
    if ($policy_exists) {
      $settings = array_merge($settings, dcycle_policy_object($policy)->renderedSettings());
    }
    return $settings;
  }

  function renderedRequirements() {
    include_once DRUPAL_ROOT . '/includes/install.inc';
    $requirements = $this->requirements();
    return theme('status_report', array('requirements' => $requirements));
  }
  function renderedSettings() {
    return array();
  }
  function policyFromForm($form) {
    // subclasses can override
    $class = $form['dcycle_policy']['#value'];
    if (get_class() != $class && class_exists($class)) {
      $object = new $class;
      return $object->policyFromForm($form);
    }
    return $this;
  }

  /**
   * This is meant to be called from the command line because
   * it uses exec(), which may be disabled for your webserver.
   *
   * To call this set up your environment in the GUI and then
   * call drush dcycle-test in the command line.
   */
  function testAll() {
    foreach ($this->getRules() as $rule) {
      $rule->test();
    }
  }
}
