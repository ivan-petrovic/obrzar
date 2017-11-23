<?php

require_once 'class.PaymentDB.inc';
require_once 'class.PaymentPage.inc';
require_once 'karnet/class.KarnetDB.inc';

class PaymentUpdateKarnet extends PaymentPage implements RenderHTML {
   
   function renderHTML() {
      $this->openSection();
      $this->sectionHeader("Упис ценe рада у карнет");
      $this->closeSection();
   }
   
   public function execute() {
      $payment = PaymentDB::load($this->pid);
      // var_dump($payment);
      // exit();
      KarnetDB::updateCena($this->pid, $payment->getValue('tipCenaRada'), $payment->getValue('cenaRada'));
      // $adr  = "Location: http://" . Application::getServerAddress() . "/payment.php?opID=" . PaymentPageDispatcher::LIST_PAYMENT;
      $adr  = "Location: http://" . Application::getServerAddress() . "/payment.php?opID=" . PaymentPageDispatcher::DETAILS_PAYMENT;
      $adr .= "&paymentID=" . $this->pid;
      header($adr);
      exit;
   }
}
