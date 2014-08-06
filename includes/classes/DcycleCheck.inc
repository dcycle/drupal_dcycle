<?php

abstract class DcycleCheck {
  private $metrics;
  private $artifacts;
  private $errors;
  private $preflight;

  private function Preflight() {
    return $this->_Preflight_();
  }
  function Run() {
    if ($this->Preflight()) {
      return $this->_Run_();
    }
  }
  function GetArtifacts() {
    self::MakeArray($this->artifacts);
    return $this->artifacts;
  }
  function GetMetrics() {
    self::MakeArray($this->metrics);
    return $this->metrics;
  }
  function GetErrors() {
    self::MakeArray($this->errors);
    return $this->errors;
  }
  protected function AddArtifact($artifact) {
    $this->AddInfo($this->artifacts, $artifact);
  }
  protected function AddMetric($metric) {
    $this->AddInfo($this->metrics, $metric);
  }
  protected function AddError($error) {
    $this->AddInfo($this->errors, $error);
  }
  private function MakeArray(&$variable) {
    if (!is_array($variable)) {
      $variable = array();
    }
  }
  private function AddInfo(&$variable, $info) {
    self::MakeArray($variable);
    if (is_array($info)) {
      $variable = array_merge($variable, $info);
    }
    else {
      $variable[] = $info;
    }
  }

  abstract function _Preflight_();
  abstract function _Run_();
}
