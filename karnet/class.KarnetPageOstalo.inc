<?php

require_once 'employe/class.EmployeDB.inc';
require_once 'payment/class.PaymentDB.inc';
require_once 'class.KarnetOstalo.inc';
require_once 'class.KarnetPage.inc';
require_once 'util/class.Validator.inc';

class KarnetPageOstalo extends KarnetPage implements RenderHTML {
   // Message generated after processing form;
   protected $message = "";
   
   function renderHTML() {
      $this->openSection();
      $this->sectionContent();
      $this->closeSection();
   }

   public function execute() {
      if($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      }
   }

   function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->sectionHeader("Остали подаци у карнету");
         echo $this->message;
         echo $this->backToListButton();
      } else {
         $this->renderForm();
      }      
   }

   function renderForm() {
      $employe = EmployeDB::load($this->eid);
      $ime = $employe->getValue('prezime') . " " . $employe->getValue('ime');
      $payment = PaymentDB::load($this->pid);

      if (KarnetOstaloDB::exist($this->eid, $this->pid)) {
         $ko = KarnetOstaloDB::load($this->eid, $this->pid);
         $this->sectionHeader("Измена осталих података у карнету за запосленог: " . $ime);
      } else {
         $ko = new KarnetOstalo($payment, $employe);
         $this->sectionHeader("Унос осталих података у карнету за запосленог: " . $ime);
      }

      echo FormMaker::make($ko, "karnet.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   function processForm() {
      $ko = new KarnetOstalo();
      $ko->setData($_POST);

      $v = new Validator;
      $v->validate($ko);

      if($v->is_valid()) {
         $this->save($ko);
      } else {
         $this->message .= FormMaker::printErrors($v->getErrors(), self::$indent+2);
         $this->message .= FormMaker::make($ko, "karnet.php?opID=" . $this->op, "Упис", self::$indent+2);
      }
   }

   protected function save(KarnetOstalo $ko) {
      $eid = $ko->getValue('zaposleniID');
      $pid = $ko->getValue('obracunID');
      $employe = EmployeDB::load($eid);
      $ime = $employe->getValue('prezime') . " " . $employe->getValue('ime');
      
      if(KarnetOstaloDB::exist($eid, $pid)) {
         KarnetOstaloDB::update($ko);
      } else {
         KarnetOstaloDB::insert($ko);
      }
      
      $this->message .= "<p><em>Запослени:</em> " . $ime . "</p>";
      $this->message .= "<p>Бруто за боловање преко 30 дана: " . $ko->getValue('iznosBruto30') . "</p>";
      $this->message .= "<p>Нето за боловање преко 30 дана: " . $ko->getValue('iznosNeto30') . "</p>";
      $this->message .= "<p>ПИО за боловање преко 30 дана: " . $ko->getValue('iznosPIO30') . "</p>";
      $this->message .= "<p></p>";
      // $adr  = "Location: http://" . Application::getServerAddress() . "/karnet.php?opID=" . KarnetPageDispatcher::OSTALO_KARNET;
      // $adr .= "&employeID=" . $eid;
     
      // header($adr);
      // exit;
   }
}
