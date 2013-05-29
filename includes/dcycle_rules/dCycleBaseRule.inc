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
  function requirements() {
    $return = array();

    // If this rule is not meant to be used for the policy in its current
    // state, then the requirement should not be returned.
    // For example, you might have a rule that states which git branches
    // should be present, but that rule should not be present in the prod
    // environment. In that case, if we are in prod, the rule's requirements
    // simply does not show up.
    if ($this->usedForPolicy()) {
      $requirements = $this->_requirements();
      if (!is_array($requirements)) {
        throw new Exception(t('The rule %rule did not return an array in its _requirements() function', array('%rule' => $this->getName())));
      }
      $return = array();
      foreach ($requirements as $name => $info) {
        if (!isset($info['description'])) {
          $info['description'] = $this->getName();
        }
        $return[$this->getMachineName() . '_' . $name] = $info;
      }
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
}
