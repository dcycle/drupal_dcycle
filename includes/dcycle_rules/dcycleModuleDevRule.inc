<?php

/**
 * @file
 * Rule for module under development.
 */

// load the parent clas
module_load_include('inc', 'Dcycle', 'includes/dcycle_rules/dcycleBaseRule');

/**
 * Rule class for rule for module under development.
 */
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
   * Gets all modules under git.
   *
   * This will allow us to surmise which module we are developing.
   */
  function modulesUnderGit() {
    $modules_under_git = array();

    foreach (module_list() as $machine => $human) {
      if (is_dir(DRUPAL_ROOT . '/' . drupal_get_path('module', $machine) . '/.git')) {
        $modules_under_git[$machine] = $human;
      }
    }
    return $modules_under_git;
  }

  /**
   * Implements dcycleBaseRule::_requirements().
   */
  function _requirements() {
    $requirements = array();

    $modules_under_git = $this->modulesUnderGit();

    foreach (module_list() as $machine => $human) {
      if (is_dir(DRUPAL_ROOT . '/' . drupal_get_path('module', $machine) . '/.git')) {
        $modules_under_git[$machine] = $human;
      }
    }
    
    // this include defines REQUIREMENT_... constants.
    include_once DRUPAL_ROOT . '/includes/install.inc';
    $requirements['dcycle_module_development'] = array(
      'title' => t('Module under development'),
      'value' => implode(', ', $modules_under_git),
      'severity' => (count($modules_under_git) == 1)?REQUIREMENT_OK:REQUIREMENT_ERROR,
      'description' => 'If you are developing a module with Dcycle, exactly one module must have a git directory, the module you are developing',
    );

    return $requirements;
  }

}
