<?php

require_once 'class.ShopDB.inc';
require_once 'class.ShopPage.inc';
require_once 'util/class.Validator.inc';

class ShopUpdate extends ShopPage implements RenderHTML {
   // Message generated after processing form;
   protected $message = "";
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Измена радње");
      $this->sectionContent();

      $this->closeSection();
   }

   protected function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         echo $this->message;
         echo $this->backToListButton();
      } else {
         $this->renderForm();
      }
   }

   public function execute() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      }
   }

   function renderForm() {
      $shop = ShopDB::load($this->sid);
      echo FormMaker::make($shop, "shop.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   function processForm() {
      $shop = new Shop();
      $shop->setData($_POST);

      $v = new Validator;
      $v->validate($shop);

      if($v->is_valid()) {
         $this->save($shop);
      } else {
         $this->message .= FormMaker::printErrors($v->getErrors(), self::$indent + 2);
         $this->message .= FormMaker::make($shop, "shop.php?opID=" . $this->op, "Упис", self::$indent + 2);
      }
   }

   protected function save(Shop $shop) {
      $sid = $shop->getValue('id');
      ShopDB::update($shop);
      $adr  = "Location: http://" . Application::getServerAddress() . "/shop.php?opID=" . ShopPageDispatcher::DETAILS_SHOP;
      $adr .= "&shopID=" . $sid;
      header($adr);
      exit;
   }
}
