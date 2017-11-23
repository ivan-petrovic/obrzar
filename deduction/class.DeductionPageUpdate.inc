<?php

require_once 'class.DeductionDB.inc';
require_once 'class.DeductionPage.inc';
require_once 'util/class.Validator.inc';

class DeductionUpdate extends DeductionPage implements RenderHTML {
   
   function renderHTML() {
      $this->openSection();
      $this->sectionHeader("Измена обуставе");
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
   // public static function hiddenElement($label, $nameAndID, $value, $indent) {
   function renderForm() {
      $deduction = DeductionDB::load($this->did);
      echo DeductionForm::openForm("deduction.php?opID=" . $this->op, self::$indent+2);
      echo FormMaker::hiddenElement("", "deductionID", $this->did, self::$indent+3);
      echo DeductionForm::update($deduction, self::$indent+3);
      echo DeductionForm::closeForm1("Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   function processForm() {
      $ids = $_POST['id']; $amounts = $_POST['iznos'];
      $months = $_POST['mesec']; $years = $_POST['godina'];
      $paymentTypes = $_POST['tipObracuna']; $did = $_POST['deductionID'];
      
      foreach( $amounts as $key => $amount ) {
         $rata = new Installment();
         // echo $did;
         $rata->setValue('id', $ids[$key]); $rata->setValue('obustavaID', $did);
         $rata->setValue('iznos', $amount); $rata->setValue('mesec', $months[$key]);
         $rata->setValue('godina', $years[$key]); $rata->setValue('tipObracuna', $paymentTypes[$key]);
         InstallmentDB::update($rata);
//         print "The id is " . $ids[$key] . "<br/>\n";
//         print "The amount is " . $amount . "<br/>\n";
//         print "The month is " . $months[$key] . "<br/>\n";
//         print "The year is " . $years[$key] . "<br/>\n";
//         print "The paymentType is " . $paymentTypes[$key] . "<br/>\n";
      }
      echo str_repeat("\t", self::$indent+2) . "<p>Успешна измена.</p>\n";
      echo $this->backToListButton();
//      $deduction = new Deduction(); $deduction->setData($_POST);
//
//      $v = new Validator; $v->validate($deduction);
//
//      if($v->is_valid()) {
//         DeductionDB::update($deduction);
//         echo str_repeat("\t", self::$indent+2) . "<p>Успешна измена.</p>\n";
//         echo $this->backToListButton();
//      } else {
//         echo DeductionForm::form($deduction, "deduction.php?opID=" . $this->op, "Упис", self::$indent+2, $v);
//      }
   }
}
