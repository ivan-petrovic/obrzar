<?php

require_once 'page/class.PageUtil.inc';
require_once 'class.DeductionForm.inc';

class DeductionPage extends CommonPage {
   // Sifra operacije za stranu, prosledjuje je page dipecer
   protected $op;
   // sifra obustave, tamo gde ima smisla inace 0
   protected $did;
   // Sifra trenutno izabranog zaposleog ili 0 ako nije izabran
   protected $eid = 0;

   /*
    * Konstruktor.
    */
   public function __construct($op, $did, $indent) {
      $this->op = $op;
      $this->did = $did;
      self::$indent = $indent;
   }

   protected function backToListButton() {
      $ref = "deduction.php?opID=" . DeductionPageDispatcher::LIST_DEDUCTION;
      return $this->button($ref, "Назад на листу");
   }

   protected function newButton() {
      $ref = "deduction.php?opID=" . DeductionPageDispatcher::NEW_DEDUCTION . "&employeID=" . $this->eid;
      return $this->button($ref, "Нова");
   }

   protected function updateButton() {
      $ref = "deduction.php?opID=" . DeductionPageDispatcher::UPDATE_DEDUCTION . "&deductionID=" . $this->did;
      return $this->button($ref, "Измени");
   }

   protected function deleteButton() {
      $ref = "deduction.php?opID=" . DeductionPageDispatcher::DELETE_DEDUCTION . "&deductionID=" . $this->did;
      return $this->button($ref, "Обриши");      
   }
   
   public function execute() {
      return;
   }
}
