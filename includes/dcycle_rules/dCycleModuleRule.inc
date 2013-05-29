<?php

/**
 * @file
 */

abstract class dcycleModuleRule extends dcycleBaseRule {
  function getName() {
    return format_plural(count($this->getModules()), 'The @m module is required to @required. Reason: @r', 'The modules @m are required to be @required. Reason: @r', array('@m' => implode(', ', $this->getModules()), '@required' => implode('; ', $this->getReqInfo()), '@r' => $this->getDescription()));
  }
  function getModules() {
    return $this->_getModules();
  }
  abstract function _getModules();
  function getReqInfo() {
    $return = array();
    foreach (array('on' => $this->getEnvironmentsOn(), 'off' => $this->getEnvironmentsOff()) as $status => $environments) {
      foreach ($environments as $environment) {
        $return[] = t('@status on @env', array('@status' => $status, '@env' => $environment));
      }
    }
    return $return;
  }
  function getEnvironmentsOn() {
    return array();
  }
  function getEnvironmentsOff() {
    return array();
  }
  abstract function getDescription();
  function _requirements() {
    $requirements = array();
    foreach ($this->getModules() as $module) {
      $requirements['module_' . rand(1000, 9999) . '_' . $module] = array(
        'title' => $module,
        'value' => $this->moduleExistsHuman($module),
        'severity' => $this->moduleStateOk($module)?REQUIREMENT_OK:REQUIREMENT_ERROR,
      );
    }
    return $requirements;
  }

  /**
   * Returns whether a module is in an acceptable state or not.
   *
   * For example if we are in the dev environment and our module is
   * required in dev, this will return TRUE only if our module is enabled.
   * If we are in dev and our module should be disabled in prod, then
   * any state is OK.
   */
  function moduleStateOk($module) {
    $state = module_exists($module);
    $environment = $this->currentEnvironment();
    foreach ($this->getEnvironmentsOn() as $env) {
      if ($env == $environment && !$state) {
        return FALSE;
      }
    }
    foreach ($this->getEnvironmentsOff() as $env) {
      if ($env == $environment && $state) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the current environment as returned by the policy
   */
  function currentEnvironment() {
    $this->getPolicyAttribute('environment', array('equals' => array('dev', 'prod')));
  }

  function moduleExistsHuman($module) {
    $exists = module_exists($module);
    return $exists?t('Module exists'):t('Module does not exist or is enabled');
  }
}
