<?php

require_once 'page/class.PageUtil.inc';
require_once 'util/class.FormMaker.inc';

class BankPage extends CommonPage {
   // Sifra operacije za stranu, prosledjuje je page dipecer
   protected $op;
   // Sifra banke, tamo gde ima smisla inace 0
   protected $bid;

   /*
    * Konstruktor.
    */
   public function __construct($op, $bid, $indent) {
      $this->op = $op;
      $this->bid = $bid;
      self::$indent = $indent;
   }

   protected function backToListButton() {
      $ref = "bank.php?opID=" . BankPageDispatcher::LIST_BANK;
      return $this->button($ref, "Назад на листу");
   }

   protected function newButton() {
      $ref = "bank.php?opID=" . BankPageDispatcher::NEW_BANK;
      return $this->button($ref, "Нова");
   }

   protected function updateButton() {
      $ref = "bank.php?opID=" . BankPageDispatcher::UPDATE_BANK . "&bankID=" . $this->bid;
      return $this->button($ref, "Измени");
   }

   protected function deleteButton() {
      $ref = "bank.php?opID=" . BankPageDispatcher::DELETE_BANK . "&bankID=" . $this->bid;
      return $this->button($ref, "Обриши");      
   }
   
   public function execute() {
      return;
   }
}
