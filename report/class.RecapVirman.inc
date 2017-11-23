<?php

require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'class.ReportPage.inc';
require_once 'class.PFE.inc';
require_once 'class.Virman.inc';

class RecapVirman extends ReportPage implements RenderHTML {
   
   public function renderHTML() {
      $this->openSection();

      $this->sectionHeader("Вирман за обједињену наплату пореза и доприноса по одбитку");
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
      echo str_repeat("\t", 4) . "<h4></h4>\n";
      echo $this->makeOneVirman($nizPFE, '1', 5);
      echo $this->makeOneVirman($nizPFE, '2', 5);
      echo $this->backToListButton();
   }

   // $oBr obrazac broj na virmanu
   protected function makeOneVirman($nizPFE, $oBr, $indent) {
      $output = "";

      $v = new Virman();
      $vdata = array(
          'duznik' => Application::getCurrFirmName(),
          'svrha' => $this->getSvrha(),
          'poverilac' => $this->getPoverilac(),
          'mesto' => Application::getCurrFirmProp('currFirmCity'),
          'datum' => $this->getDatum($nizPFE[0]->payment->getValue('datumIsplate')),
          'sifraPlacanja' => '254',
          'valuta' => 'РСД',
          'iznos' => number_format($this->getUkupanIznos($nizPFE), 2, ",", "."),
          'racunDuznika' => Application::getCurrFirmProp('currFirmAccountNum'),
          'racunPoverioca' => '840-4848-37',
          'brojModela2' => '97',
          'pozivNaBroj2' => '',
          'obrazacBr' => $oBr
      );
      $v->setData($vdata);
      
      $output .= VirmanRenderer::display($v, $indent);
      return $output;
   }

   protected function getDatum($datum) {
      if($datum === '0000-00-00') {
         $datumString = '';
      } else {
         $di = new DateTime($datum);
         $datumString = $di->format("j.n.Y");
      }
      return $datumString;
   }

   protected function getSvrha() {
      return "Порез и доприноси за ЛД за " . Application::getCurrPaymentName();
   }

   protected function getPoverilac() {
      return "Обједињена наплата пореза и доприноса по одбитку";
   }

   public static function getUkupanIznos($nizPFE) {
      $ukPorez = self::getUkupno($nizPFE, 'Porez', 'porez');
      // $ukDoprinosZap = self::getUkupno($nizPFE, 'Doprinos', 'doprinos');
      // $ukDoprinosPosl = self::getUkupno($nizPFE, 'Doprinos', 'doprinosPoslodavac');
      $ukPIOZap = self::getUkupno($nizPFE, 'Doprinos', 'doprinosPIO');
      $ukZdrZap = self::getUkupno($nizPFE, 'Doprinos', 'doprinosZdravstvo');
      $ukNezZap = self::getUkupno($nizPFE, 'Doprinos', 'doprinosNezaposlenost');
      $ukPIOPosl = self::getUkupno($nizPFE, 'Doprinos', 'doprinosPIOPoslodavac');
      $ukZdrPosl = self::getUkupno($nizPFE, 'Doprinos', 'doprinosZdravstvoPoslodavac');
      $ukNezPosl = self::getUkupno($nizPFE, 'Doprinos', 'doprinosNezaposlenostPoslodavac');

      // return $ukPorez + $ukDoprinosZap + $ukDoprinosPosl;
      return $ukPorez + $ukPIOZap + $ukZdrZap + $ukNezZap + $ukPIOPosl + $ukZdrPosl + $ukNezPosl;
   }
   
   public static function getUkupno($nizPFE, $className, $fname) {
      $uk = 0;
      $method = 'get' . $className;
      foreach ($nizPFE as $pfe) {
         $uk += self::$method($pfe, $fname);
      }
      return $uk;
   }

   public static function getPorez(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaPorez->$fname();
      } else {
         return $pfe->konacniPorez->$fname();
      }
   }
   
   public static function getDoprinos(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaDoprinos->$fname();
      } else {
         return $pfe->konacniDoprinos->$fname();
      }
   }
}