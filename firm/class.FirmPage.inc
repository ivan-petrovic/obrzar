<?php

require_once 'page/class.PageUtil.inc';
require_once 'util/class.FormMaker.inc';

class FirmPage extends CommonPage {
   // Sifra operacije za stranu, prosledjuje je page dipecer
   protected $op;
   // Sifra firme, tamo gde ima smisla inace 0
   protected $fid;

   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $indent) {
      $this->op = $op;
      $this->fid = $fid;
      self::$indent = $indent;
   }
   
   protected function backToListButton() {
      $ref = "firm.php?opID=" . FirmPageDispatcher::LIST_FIRM;
      return $this->button($ref, "Назад на листу");
   }

   protected function newButton() {
      $ref = "firm.php?opID=" . FirmPageDispatcher::NEW_FIRM;
      return $this->button($ref, "Нова");
   }

   protected function updateButton() {
      $ref = "firm.php?opID=" . FirmPageDispatcher::UPDATE_FIRM . "&firmID=" . $this->fid;
      return $this->button($ref, "Измени");
   }

   protected function deleteButton() {
      $ref = "firm.php?opID=" . FirmPageDispatcher::DELETE_FIRM . "&firmID=" . $this->fid;
      return $this->button($ref, "Обриши");      
   }
   
   public function execute() {
      return;
   }
}
