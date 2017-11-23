<?php

require_once 'class.FirmDB.inc';
require_once 'class.FirmPage.inc';
require_once 'util/class.Validator.inc';

class FirmNew extends FirmPage implements RenderHTML {
   // Message generated after processing form;
   protected $message = "";
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Унос нове фирме");
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

   protected function renderForm() {
      $firm = new Firm();
      echo FormMaker::make($firm, "firm.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   protected function processForm() {
      $firm = new Firm();
      $firm->setData($_POST);

      $v = new Validator;
      $v->validate($firm);

      if($v->is_valid()) {
         $this->save($firm);
      } else {
         $this->message .= FormMaker::printErrors($v->getErrors(), self::$indent+2);
         $this->message .= FormMaker::make($firm, "firm.php?opID=" . $this->op, "Упис", self::$indent+2);
      }
   }
   
   protected function save(Firm $firm) {
      $rez = FirmDB::insert($firm); // return value is array(insert_id, affected_rows)
      $adr  = "Location: " . Application::getServerAddress() . "/firm.php?opID=" . FirmPageDispatcher::DETAILS_FIRM;
      $adr .= "&firmID=" . $rez[0];
      header($adr);
      exit;
   }
}
