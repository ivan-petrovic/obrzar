<?php

require_once 'class.DeductionDB.inc';
require_once 'class.DeductionPage.inc';
require_once 'util/class.Validator.inc';
// require_once 'class.DeductionForm.inc';

class DeductionNew extends DeductionPage implements RenderHTML {
   
   function renderHTML() {
      $this->openSection();

      $empid = Application::getEmployeID();
      if($empid !== 0) {
         $employe = EmployeDB::load(Application::getEmployeID());
         $ime = $employe->getValue('ime') . " " . $employe->getValue('prezime');
         $this->sectionHeader("Унос нове обуставе за " . $ime);
      } else {
         $this->sectionHeader("Унос нове обуставе");
      }
      
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      } else {
         $this->renderForm();
      }      
   }

   function renderForm() {
      $deduction = new Deduction(Application::getEmployeID());
      echo DeductionForm::make($deduction, "deduction.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   function processForm() {
      $deduction = new Deduction(); $deduction->setData($_POST);

      $v = new Validator; $v->validate($deduction);

      if($v->is_valid()) {
         DeductionDB::insert($deduction);
         echo str_repeat("\t", self::$indent+2) . "<p>Успешан упис.</p>\n";
         echo $this->backToListButton();
      } else {
         echo DeductionForm::make($deduction, "deduction.php?opID=" . $this->op, "Упис", self::$indent+2, $v);
      }
   }
}
