<?php

/**
 * @file
 * Defines the dcycle policy
 */

abstract class dcycleBaseRule {
  private $policy;
  function __construct($policy) {
    $this->policy = $policy;
  }
  /**
   * Get requirements only if relevant in the policy
   *
   * For example, if the policy states that we are developing a website,
   * the requirements which check if we are developing a module are
   * irrelevant and will not be included here.
   */
  function requirements() {
    $return = array();

    // If this rule is not meant to be used for the policy in its current
    // state, then the requirement should not be returned.
    // For example, you might have a rule that states which git branches
    // should be present, but that rule should not be present in the prod
    // environment. In that case, if we are in prod, the rule's requirements
    // simply does not show up.
    if ($this->usedForPolicy()) {
      $return = $this->allRequirements();
    }
    return $return;
  }
  
  /**
   * Get requirements even if irrelevant to the policy
   *
   * For example, if the policy is trying to determine if we are developing
   * a module or a website, it might use checkReqs() on the module dev rule,
   * which needs to know if the requirements are met, in which case the module 
   * will consider that we are developing a module.
   */
  function allRequirements() {
    $return = array();
    $requirements = $this->_requirements();
    if (!is_array($requirements)) {
      throw new Exception(t('The rule %rule did not return an array in its _requirements() function', array('%rule' => $this->getName())));
    }
    foreach ($requirements as $name => $info) {
      if (!isset($info['description'])) {
        $info['description'] = $this->getName();
      }
      $return[$this->getMachineName() . '_' . $name] = $info;
    }
    return $return;
  }   
  abstract function getName();
  function getMachineName() {
    return get_class();
  }
  abstract function _requirements();
  function getPolicy() {
    // The policy here is not necessarily the site's policy. It might
    // be the site's policy with some overridden info, or another policy
    // altogether, for preview purposes.
    return $this->policy;
  }
  function getPolicyAttribute($name, $expected) {
    $policy = $this->getPolicy();
    return $policy->getAttribute($name, $expected);
  }

  /**
   * Determines whether this rule is valid for a policy.
   *
   * For example, you might have a rule that checks that your module
   * is under version control.
   */
  function usedForPolicy() {
    return TRUE;
  }

  /**
   * Peer review checklist
   */
  function peerReviewChecklist() {
    return array();
  }

  /**
   * Check requirements, return TRUE if passed, FALSE if error
   */
  function checkReqs() {
    foreach ($this->allRequirements() as $req => $info) {
      if ($info['severity'] == REQUIREMENT_ERROR) {
        return FALSE;
      }
    }
    return TRUE;
  }
  
  /**
   * Test, called from the command line.
   *
   * Subclasses should override.
   */
  function test() {
  }
}
