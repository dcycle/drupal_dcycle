<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dcycleBaseRule');

class dcycleModuleDevRule extends dcycleBaseRule {
  /**
   * Overrides dcycleBaseRule::getName().
   */
  function getName() {
    return 'When developing a module, only one active module should have a git repo.';
  }

  /**
   * Overrides dcycleBaseRule::usedForPolicy().
   *
   * Use this rule only for the dev environment.
   */
  function usedForPolicy() {
    return in_array($this->getPolicyAttribute('project_type', array('equals' => array('website', 'module'))), array('module'));
  }

  /**
   * Implements dcycleBaseRule::_requirements().
   */
  function _requirements() {
    $requirements = array();

    $modules_under_git = array();

    foreach (module_list() as $machine => $human) {
      if (is_dir(DRUPAL_ROOT . '/' . drupal_get_path('module', $machine) . '/.git')) {
        $modules_under_git[$machine] = $human;
      }
    }

    $requirements['dcycle_module_development'] = array(
      'title' => t('Module under development'),
      'value' => implode(', ', $modules_under_git),
      'severity' => (count($modules_under_git) == 1)?REQUIREMENT_OK:REQUIREMENT_ERROR,
      'description' => 'If you are developing a module with dcycle, exactly one module must have a git directory, the module you are developing',
    );

    return $requirements;
  }
}