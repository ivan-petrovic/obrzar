<?php

require_once 'class.ShopDB.inc';
require_once 'class.ShopPage.inc';

class ShopDelete extends ShopPage implements RenderHTML {
   function renderHTML() {
      $this->openSection();
      $this->sectionHeader("Брисање радње");
      $this->closeSection();
   }
   
   public function execute() {
      ShopDB::delete($this->sid);
      $adr  = "Location: http://" . Application::getServerAddress() . "/oz/shop.php?opID=" . ShopPageDispatcher::LIST_SHOP;
      header($adr);
      exit;
   }
}
