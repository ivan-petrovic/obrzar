<?php

require_once 'class.BankDB.inc';
require_once 'class.BankPage.inc';
require_once 'util/class.Validator.inc';

class BankNew extends BankPage implements RenderHTML {
   // Message generated after processing form;
   protected $message = "";
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Унос нове банке");
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
      if($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      }
   }

   function renderForm() {
      $bank = new Bank();
      echo FormMaker::make($bank, "bank.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   function processForm() {
      $bank = new Bank();
      $bank->setData($_POST);

      $v = new Validator;
      $v->validate($bank);

      if($v->is_valid()) {
         $this->save($bank);
      } else {
         $this->message .= FormMaker::printErrors($v->getErrors(), self::$indent+2);
         $this->message .= FormMaker::make($bank, "bank.php?opID=" . $this->op, "Упис", self::$indent+2);
      }
   }
   
   protected function save(Bank $bank) {
      $rez = BankDB::insert($bank); // return value is array(insert_id, affected_rows)
      $adr  = "Location: http://" . Application::getServerAddress() . "/bank.php?opID=" . BankPageDispatcher::DETAILS_BANK;
      $adr .= "&bankID=" . $rez[0];
      header($adr);
      exit;
   }
}
