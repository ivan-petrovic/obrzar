<?php
require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'class.ReportPage.inc';
require_once 'report/class.PFE.inc';
require_once 'report/class.PFERender.inc';

class ListaReport extends ReportPage implements RenderHTML {
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Исплатна листа за " . Application::getCurrPaymentName());
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $payment = PaymentDB::load($this->pid);
      $nizKarnet = KarnetDB::loadAll($this->pid);
      
      $nizPFE = array();
      foreach ($nizKarnet as $karnet) {
         $employe = EmployeDB::load($karnet->getValue('zaposleniID'));
         $nizPFE[] = new PaymentForEmploye($employe, $payment, $karnet);
      }
      echo $this->renderList($nizPFE, 5);
      echo $this->backToListButton();
   }
   
   protected function renderList($nizPFE, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent) . "<table class=\"isplatna-lista\">\n";
      $output .= str_repeat("\t", $indent+1) . "<tr><th>Име и презиме</th><th>Износ</th><th>Потпис</th></tr>\n";

      foreach ($nizPFE as $pfe) {
         $output .= str_repeat("\t", $indent+1) . $this->makeRow($pfe);
      }
      $output .= str_repeat("\t", $indent) . "</table>\n";
      return $output;
   }
   
   protected function makeRow(PaymentForEmploye $pfe) {
      $ime = $pfe->employe->getValue('prezime') . " " . $pfe->employe->getValue('ime');
      $fiznos = number_format($this->getIznos($pfe), 2,  ",", ".");
      return "<tr><td>{$ime}</td><td>{$fiznos}</td><td></td></tr>\n";
   }   

   public function getIznos(PaymentForEmploye $pfe) {
      $netoUkupno = self::getNeto($pfe, 'netoUkupno');
      // $ukObustave = Obustave::ukupnoObustave($pfe->employe->getValue('id'), $pfe->payment, $netoUkupno);
      $ukObustave = 0;
      
      if(!$pfe->flagKon) {
         return $pfe->akontacijaNeto->netoZaIsplatu($ukObustave);
      } else {
         return $pfe->konacniNeto->netoZaIsplatu($ukObustave);
      }
   }
   
   public static function getNeto(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaNeto->$fname();
      } else {
         return $pfe->konacniNeto->$fname();
      }
   }
}
