<?php

require_once 'employe/class.EmployeDB.inc';
require_once 'payment/class.PaymentDB.inc';
require_once 'class.KarnetDB.inc';
require_once 'class.KarnetPage.inc';
require_once 'report/class.PFE.inc';
require_once 'report/class.PFERender.inc';

class KarnetPrint extends KarnetPage implements RenderHTML {
   // Karnet koji se ispisuje;
   protected $listic;

   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $pid, $eid, $indent) {
      parent::__construct($op, $fid, $pid, $eid, $indent);
      
      $employe = EmployeDB::load($this->eid);
      $payment = PaymentDB::load($this->pid);
      $karnet = KarnetDB::load($this->eid, $this->pid);
      
      $this->listic = new PaymentForEmploye($employe, $payment, $karnet);
   }
   
   function renderHTML() {
      $this->openSection();

      $this->sectionHeader("Листић обрачуна");
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      PFERender::display1($this->listic, 5);
      echo $this->backToListButton();
      echo $this->updateButton();
   }
}
