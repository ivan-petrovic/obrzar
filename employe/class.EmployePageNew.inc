<?php

require_once 'class.EmployeDB.inc';
require_once 'class.EmployePage.inc';
require_once 'util/class.Validator.inc';

class EmployeNew extends EmployePage implements RenderHTML {
   // Message generated after processing form;
   protected $message = "";
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Унос новог запосленог");
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
      $employe = new Employe($this->fid);
      echo FormMaker::make($employe, "employe.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   function processForm() {
      $employe = new Employe();
      $employe->setData($_POST);

      $v = new Validator;
      $v->validate($employe);

      if($v->is_valid()) {
         $this->save($employe);
      } else {
         $this->message .= FormMaker::printErrors($v->getErrors(), self::$indent+2);
         $this->message .= FormMaker::make($employe, "employe.php?opID=" . $this->op, "Упис", self::$indent+2);
      }
   }

   protected function save(Employe $employe) {
      $rez = EmployeDB::insert($employe); // return value is array(insert_id, affected_rows)
      $adr  = "Location: " . Application::getServerAddress() . "/employe.php?opID=" . EmployePageDispatcher::DETAILS_EMPLOYE;
      $adr .= "&employeID=" . $rez[0];
      header($adr);
      exit;
   }
}
