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
    return in_array($this->siteType(), array('dev'));
  }
  
  function siteType() {
    return $this->getPolicyAttribute('environment', array('equals' => array('dev', 'prod')));
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
        'title' => $this->reqTitle(),
        'value' => $this->isGitHuman(),
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

  function devType() {
    return $this->getPolicyAttribute('project_type', array('equals' => array('website', 'module')));
  }

  function reqTitle() {
    switch ($this->devType()) {
      case 'module':
        return 'When developing a module, the module under development must be a git folder.';
        break;
      case 'website':
        return 'When developing a website, your path to drupal must be a git folder.';
        break;
      default:
        throw new Exception('Expecting dev type to be module or website');
        break;
    }
  }

  /**
   * Find the attribute in the parent policy.
   */
  function getProject() {
    return $this->getPolicyAttribute('active-project', array('not-empty'));
  }

  function isGitHuman() {
    $info = array();
    $is_git = $this->isGit($info);
    if ($is_git) {
      return $is_git;
    }
    else {
      if (isset($info['desc'])) {
        return $info['desc'];
      }
      else {
        return t('No git repo found.');
      }
    }
  }

  /**
   * Check if the project is a git project.
   */
  function isGit(&$info = array()) {
    if (function_exists('exec')) {
      switch ($this->devType()) {
        case 'module':
          $dir = getcwd() . '/' . drupal_get_path($project['type'], $project['name']);
          break;
        case 'website':
          $dir = getcwd();
          break;
        default:
          throw new Exception('Expecting dev type to be module or website');
          break;
      }
      $return_var = NULL;
      $output = array();
      $project = $this->getProject();
      exec('cd ' . $dir . '; git status', $output, $return_var);
      if ($return_var) {
        $info['desc'] = t('@dir does not seem to be a git directory (or any of its parents).', array('@dir' => $dir));
        return FALSE;
      }
      return t('@dir, or one of its parents, seems to be a git directory.', array('@dir' => $dir));
    }
    else {
      throw new Exception('Please enable PHP\'s exec() must be available on your dev machine');
    }
  }

}
