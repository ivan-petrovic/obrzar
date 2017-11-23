<?php

require_once 'class.EmployeDB.inc';
require_once 'class.EmployePage.inc';
require_once 'util/class.Validator.inc';

class EmployeUpdate extends EmployePage implements RenderHTML {
   // Message generated after processing form;
   protected $message = "";
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Измена запосленог");
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
   
   protected function renderForm() {
      $employe = EmployeDB::load($this->eid);
      echo FormMaker::make($employe, "employe.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   protected function processForm() {
      $employe = new Employe();
      $employe->setData($_POST);

      $v = new Validator;
      $v->validate($employe);

      if($v->is_valid()) {
         $this->save($employe);
      } else {
         $this->message .= FormMaker::printErrors($v->getErrors(), self::$indent + 2);
         $this->message .= FormMaker::make($employe, "employe.php?opID=" . $this->op, "Упис", self::$indent + 2);
      }
   }

   protected function save(Employe $employe) {
      $eid = $employe->getValue('id');
      EmployeDB::update($employe);
      $adr  = "Location: http://" . Application::getServerAddress() . "/oz/employe.php?opID=" . EmployePageDispatcher::DETAILS_EMPLOYE;
      $adr .= "&employeID=" . $eid;
      header($adr);
      exit;
   }
}
