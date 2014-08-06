<?php

class DcycleCoder extends DcycleCheck {
  function _Preflight_() {
    $errors = array();
    if (!DcycleEnvironment::ExecAvailable()) {
      $errors[] = t('The exec() function is not available');
    }
    elseif (!DcycleEnvironment::DrushExists()) {
      $errors[] = t('Drush is not available, if you have it on your machine, please set the variable dcycle_drush_path (e.g. drush vset dcycle_drush_path /path/to/drush). You can find the drush path by typing which drush in the command line.');
    }
    elseif (!DcycleEnvironment::DrushCommandExists('coder')) {
      $errors[] = t('We cannot find the command drush coder.');
    }
    if (!module_exists('coder_review')) {
      $errors[] = t('Please enable the coder_review module');
    }
    if (count($errors)) {
      $this->AddError($errors);
      return FALSE;
    }
    return TRUE;
  }

  function _Run_() {
    $lines = array();
    // see https://www.drupal.org/node/2316653
    DcycleEnvironment::DrushCommand('coder --minor sites/all/modules/custom/*/*.module sites/all/modules/custom', $lines);
    $this->AddArtifact(self::ParseArtifacts($lines));
    $this->AddMetric(self::ParseMetrics($lines));
    return $lines;
  }

  static function ParseArtifacts($lines) {
    return implode(PHP_EOL, $lines);
  }

  static function ParseMetrics($lines) {
    $metrics = array();
    foreach ($lines as $line) {
      if (drupal_substr(trim($line), 0, strlen('Coder found')) == 'Coder found') {
        foreach (array('normal', 'minor') as $type) {
          $matches = array();
          if (preg_match('/([0-9]*) ' . $type . ' warnings/', $line, $matches)) {
            $metrics[$type . ' coder warnings'] = $matches[1];
          }
        }
      }
    }
    return $metrics;
  }
}
