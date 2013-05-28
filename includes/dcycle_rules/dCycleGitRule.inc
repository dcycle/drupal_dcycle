<?php

/**
 * @file
 */

module_load_include('inc', 'dcycle', 'includes/dcycle_rules/dCycleBaseRule');

class dCycleGitRule extends dCycleBaseRule {
  /**
   * Overrides dCycleBaseRule::getName().
   */
  function getName() {
    return 'Git must be enabled for your project.';
  }

  /**
   * Overrides dCycleBaseRule::usedForPolicy().
   *
   * Use this rule only for the dev environment.
   */
  function usedForPolicy() {
    $environment = in_array($this->getPolicyAttribute('environment', array('equals' => array('dev', 'prod'))), array('dev'));
  }

  /**
   * Implements dCycleBaseRule::_requirements().
   */
  function _requirements() {
    $requirements = array();

    try {
      // we have to figure out what is the module. One of its parent folders
      // will be in git. It is not necessary here to know which ones
      $is_git = $this->isGit();
      $requirements['git_exists'] = array(
        'title' => t('Environment'),
        'value' => $environment?$environment:'Not yet set, please set it',
        'severity' => $environment?REQUIREMENT_INFO:REQUIREMENT_ERROR,
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


  function getProject() {
    return $this->getPolicyAttribute('active-project', array('not-empty');
  }

  function isGit() {
    if (function_exists('exec')) {
      $output = array();
      $project = $this->getProject();
      dcycle_exec('cd ' . getcwd() . '/' . drupal_get_path($project['type'], $project['name']) '; git status', $output);
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
