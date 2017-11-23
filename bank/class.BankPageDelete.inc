<?php

require_once 'class.BankDB.inc';
require_once 'class.BankPage.inc';

class BankDelete extends BankPage implements RenderHTML {
   function renderHTML() {
      $this->openSection();
      $this->sectionHeader("Брисање банке");
      $this->closeSection();
   }

   public function execute() {
      BankDB::delete($this->bid);
      $adr  = "Location: http://" . Application::getServerAddress() . "/bank.php?opID=" . BankPageDispatcher::LIST_BANK;
      header($adr);
      exit;
   }
}
