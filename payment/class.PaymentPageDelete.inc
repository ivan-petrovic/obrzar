<?php

require_once 'class.PaymentDB.inc';
require_once 'class.PaymentPage.inc';


class PaymentDelete extends PaymentPage implements RenderHTML {
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Брисање обрачуна");
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      echo str_repeat("\t", self::$indent+2) . "<p>Садржај стране за брисање обрачуна. НИЈЕ АКТИВНО!</p>\n";
      echo $this->backToListButton();
   }

//   public function execute() {
//      PaymentDB::delete($this->pid);
//      $adr  = "Location: http://" . Application::getServerAddress() . "/oz/payment.php?opID=" . PaymentPageDispatcher::LIST_PAYMENT;
//      header($adr);
//      exit;
//   }
}
