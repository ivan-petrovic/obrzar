<?php

// require_once 'employe/class.EmployeDB.inc';
require_once 'class.KarnetDB.inc';
require_once 'class.KarnetPage.inc';
require_once 'util/class.Validator.inc';

class KarnetUpdate extends KarnetPage implements RenderHTML {
   // Message generated after processing form;
   protected $message = "";
   
   function renderHTML() {
      $employe = EmployeDB::load($this->eid);
      $ime = $employe->getValue('prezime') . " " . $employe->getValue('ime');

      $this->openSection();
      
      $this->sectionHeader("Измена карнета: {$ime}");
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
      $karnet = KarnetDB::load($this->eid, $this->pid);
      echo FormMaker::make($karnet, "karnet.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   function processForm() {
      $karnet = new Karnet();
      $karnet->setData($_POST);

      $v = new Validator;
      $v->validate($karnet);

      if($v->is_valid()) {
         $this->save($karnet);
      } else {
         $this->message .= FormMaker::printErrors($v->getErrors(), self::$indent + 2);
         $this->message .= FormMaker::make($karnet, "karnet.php?opID=" . $this->op, "Упис", self::$indent + 2);
      }
   }

   protected function save(Karnet $karnet) {
      $eid = $karnet->getValue('zaposleniID');
      // echo $eid; exit();
      KarnetDB::update($karnet);
      $adr  = "Location: http://" . Application::getServerAddress() . "/oz/karnet.php?opID=" . KarnetPageDispatcher::DETAILS_PRINT;
      $adr .= "&employeID=" . $eid;
      header($adr);
      exit;
   }
}
