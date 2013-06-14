<?php

/**
 * @file
 * Git folder must exist.
 */

// load base class
module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dcycleBaseRule');

/**
 * Class for rule stating that git folder must exist.
 */
class dcycleGitRule extends dcycleBaseRule {
  /**
   * Overrides dcycleBaseRule::getName().
   */
  function getName() {
    return 'Git must be enabled for your project.';
  }

  /**
   * Overrides dcycleBaseRule::usedForPolicy().
   *
   * Use this rule only for the dev environment.
   */
  function usedForPolicy() {
    return in_array($this->getPolicyAttribute('environment', array('equals' => array('dev', 'prod'))), array('dev'));
  }

  /**
   * Implements dcycleBaseRule::_requirements().
   */
  function _requirements() {
    $requirements = array();

    try {
      // we have to figure out what is the module. One of its parent folders
      // will be in git. It is not necessary here to know which ones
      $is_git = $this->isGit();
      $requirements['git_exists'] = array(
        'title' => t('Git status'),
        'value' => $is_git?'OK':'Not yet set, please set it',
        'severity' => $is_git?REQUIREMENT_OK:REQUIREMENT_ERROR,
        'weight' => -1,
      );
    }
    catch (Exception $e) {
      $requirements['branches_preflight'] = array(
        'title' => t('An exception occured while checking for git'),
        'value' => 'exception',
        'severity' => REQUIREMENT_ERROR,
        'description' => $e->getMessage(),
        'weight' => -1,
      );
    }
    return $requirements;
  }

  /**
   * Find the attribute in the parent policy.
   */
  function getProject() {
    return $this->getPolicyAttribute('active-project', array('not-empty'));
  }

  /**
   * Check if the project is a git project.
   */
  function isGit() {
    if (function_exists('exec')) {
      $output = array();
      $project = $this->getProject();
      exec('cd ' . getcwd() . '/' . drupal_get_path($project['type'], $project['name']) . '; git status', $output);
      foreach ($output as $line) {
        if ($line == 'fatal: Not a git repository (or any of the parent directories): .git') {
          return FALSE;
        }
      }
      return TRUE;
    }
    else {
      throw new Exception('Please enable PHP\'s exec() must be available on your dev machine');
    }
  }

}
