<?php

require_once 'page/class.PageUtil.inc';
require_once 'util/class.FormMaker.inc';
require_once 'karnet/class.KarnetDB.inc';

class PaymentPage extends CommonPage {
   // Sifra operacije za stranu, prosledjuje je page dipecer
   protected $op;
   // Sifra trenutno izabrane firme
   protected $fid;
   // sifra obracuna, tamo gde ima smisla inace 0
   protected $pid;

   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $pid, $indent) {
      $this->op = $op;
      $this->fid = $fid;
      $this->pid = $pid;
      self::$indent = $indent;
   }

   protected function backToListButton() {
      $ref = "payment.php?opID=" . PaymentPageDispatcher::LIST_PAYMENT;
      return $this->button($ref, "Назад на листу");
   }

   protected function updateCenaRadaUKarnetu() {
      $ref = "payment.php?opID=" . PaymentPageDispatcher::UPDATE_KARNET_PAYMENT . "&paymentID=" . $this->pid;
      return $this->button($ref, "Упиши цену у карнет");
   }

   protected function newButton() {
      $ref = "payment.php?opID=" . PaymentPageDispatcher::NEW_PAYMENT;
      return $this->button($ref, "Нови");
   }

   protected function updateButton() {
      $ref = "payment.php?opID=" . PaymentPageDispatcher::UPDATE_PAYMENT . "&paymentID=" . $this->pid;
      return $this->button($ref, "Измени");
   }

   protected function deleteButton() {
      $ref = "payment.php?opID=" . PaymentPageDispatcher::DELETE_PAYMENT . "&paymentID=" . $this->pid;
      return $this->button($ref, "Обриши");      
   }
   
   public function execute() {
      return;
   }
}
