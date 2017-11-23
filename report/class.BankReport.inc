<?php

require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'bank/class.BankDB.inc';
require_once 'class.ReportPage.inc';
require_once 'class.PFE.inc';
require_once 'class.PFERender.inc';
require_once 'class.Virman.inc';

class BankReport extends ReportPage implements RenderHTML {
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Преглед запослених по банкама");
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

      $this->renderList($nizPFE, 5);
      echo $this->backToListButton();
   }
   
   protected function renderList($nizPFE, $indent) {
      $banks = BankDB::loadAll();
      foreach ($banks as $bank) {
         $cnt = $this->cntEmployesInBank($bank, $nizPFE);
         if($cnt !== 0) {
            echo $this->makeTable($bank, $nizPFE, $indent);
         }
      }
   }

   protected function cntEmployesInBank(Bank $bank, $nizPFE) {
      $cnt = 0;
      foreach ($nizPFE as $pfe) {
         if($pfe->employe->getValue('bankaID') === $bank->getValue('id')) {
            ++$cnt;
         }
      }
      return $cnt;
   }
   
   protected function makeTable(Bank $bank, $nizPFE, $indent) {
      $output  = "";
      $output .= $this->makeZaglavlje($bank, $indent);
      
      $output .= str_repeat("\t", $indent) . "<table class=\"lista-banke\">\n";
      $output .= str_repeat("\t", $indent+1) . "<tr><th>Име и презиме</th><th>Жиро рачун</th><th>Износ</th></tr>\n";
      foreach ($nizPFE as $pfe) {
         $output .= $this->makeRow($bank, $pfe, $indent+1);
      }
      $output .= str_repeat("\t", $indent) . "</table>\n";
      $output .= str_repeat("\t", $indent) . "<hr>\n";
      
      return $output;
   }

   protected function makeZaglavlje(Bank $bank, $indent) {
      $output = "";

      $output .= str_repeat("\t", $indent) . "<div class=\"firma\">\n";
      $output .= str_repeat("\t", $indent) . "<h4>" . Application::getCurrFirmName() . "</h4>\n";
      $output .= str_repeat("\t", $indent) . "<p>"  . Application::getCurrFirmProp('currFirmAddress') . "</p>\n";
      $output .= str_repeat("\t", $indent) . "<p>"  . Application::getCurrFirmProp('currFirmCity') . "</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- firma -->\n";

      $output .= str_repeat("\t", $indent) . "<div class=\"banka\">\n";
      $output .= str_repeat("\t", $indent+1) . "<h4>{$bank->getValue('naziv')}</h4>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>{$bank->getValue('adresa')}</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>{$bank->getValue('mesto')}</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- banka -->\n";

      return $output;
   }
   
   protected function makeRow(Bank $bank, PaymentForEmploye $pfe, $indent) {
      $output = "";
      if($pfe->employe->getValue('bankaID') === $bank->getValue('id')) {
         $ime = $pfe->employe->getValue('prezime') . " " . $pfe->employe->getValue('ime');
         $ziro = $pfe->employe->getValue('racun');
         $iznos = number_format($this->getIznos($pfe), 2,  ",", ".");

         $output .= str_repeat("\t", $indent) . "<tr><td>{$ime}</td><td>{$ziro}</td><td>{$iznos}</td></tr>\n";
      }
      return $output;
   }
   
   // Ista funkcija kao u ListaReport
   public function getIznos(PaymentForEmploye $pfe) {
      $netoUkupno = self::getNeto($pfe, 'netoUkupno');
      $ukObustave = Obustave::ukupnoObustave($pfe->employe->getValue('id'), $pfe->payment, $netoUkupno);
      
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

class BankVirmanReport extends ReportPage implements RenderHTML {
   protected $pozNaBr = "";
   protected $obrazacBroj = "1";
   
   public function renderHTML() {
      $this->openSection();

      $this->sectionHeader("Преглед налога за пренос по банкама");
      $this->sectionContent();

      $this->closeSection();
   }

   protected function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      }
      echo $this->makeForm();
      echo $this->printVirmani();
   }

   protected function makeForm() {
      $output  = "";
      $scriptName = "report.php?opID=" . $this->op;
      $output .= FormMaker::openForm($scriptName, 5);

      $output .= FormMaker::textElement('Позив на број', "poznabr", $this->pozNaBr, self::$indent+3);
      $output .= FormMaker::textElement('Образац број', "obrazacbroj", $this->obrazacBroj, self::$indent+3);
      $output .= FormMaker::submitElement('Примени', 6);
      
      $output .= FormMaker::closeForm(5);
      return $output;
   }

   protected function processForm() {
      $this->pozNaBr = filter_input(INPUT_POST, 'poznabr', FILTER_SANITIZE_STRING);
      $this->obrazacBroj = filter_input(INPUT_POST, 'obrazacbroj', FILTER_SANITIZE_STRING);
   }

   protected function printVirmani() {
      $payment = PaymentDB::load($this->pid);
      $nizKarnet = KarnetDB::loadAll($this->pid);
      
      $nizPFE = array();
      foreach ($nizKarnet as $karnet) {
         $employe = EmployeDB::load($karnet->getValue('zaposleniID'));
         $nizPFE[] = new PaymentForEmploye($employe, $payment, $karnet);
      }

      $this->renderList($nizPFE, 5);
      echo $this->backToListButton();
   }
   
   protected function renderList($nizPFE, $indent) {
      $banks = BankDB::loadAll();
      foreach ($banks as $bank) {
         $cnt = $this->cntEmployesInBank($bank, $nizPFE);
         if($cnt !== 0) {
            echo $this->makeVirman($bank, $nizPFE, $indent);
         }
      }
   }

   protected function cntEmployesInBank(Bank $bank, $nizPFE) {
      $cnt = 0;
      foreach ($nizPFE as $pfe) {
         if($pfe->employe->getValue('bankaID') === $bank->getValue('id')) {
            ++$cnt;
         }
      }
      return $cnt;
   }
   
   protected function makeVirman(Bank $bank, $nizPFE, $indent) {
      // $output = str_repeat("\t", $indent) . "<h4>{$bank->getValue('naziv')}</h4>\n";
      $output = "";
      foreach ($nizPFE as $pfe) {
         $output .= $this->makeOneVirman($bank, $pfe, $indent);
      }
      return $output;
   }

   protected function makeOneVirman(Bank $bank, PaymentForEmploye $pfe, $indent) {
      $output = "";
      if($pfe->employe->getValue('bankaID') === $bank->getValue('id')) {
         $ime = $pfe->employe->getValue('prezime') . " " . $pfe->employe->getValue('ime');
         // $pnb2 = $pfe->payment->getValue('mesec') . "/" . $pfe->payment->getValue('godina');
         $pnb2 = $this->pozNaBr;
         
         $v = new Virman();
         $vdata = array(
             'duznik' => Application::getCurrFirmName(),
             'svrha' => $this->getSvrha(),
             'poverilac' => $ime,
             'mesto' => Application::getCurrFirmProp('currFirmCity'),
             'datum' => $this->getDatum($pfe->payment->getValue('datumIsplate')),
             'sifraPlacanja' => '240',
             'valuta' => 'РСД',
             'iznos' => number_format($this->getIznos($pfe), 2, ",", "."),
             'racunDuznika' => Application::getCurrFirmProp('currFirmAccountNum'),
             'racunPoverioca' => $pfe->employe->getValue('racun'),
             'brojModela2' => '97',
             'pozivNaBroj2' => $pnb2,
             'obrazacBr' => $this->obrazacBroj
         );
         $v->setData($vdata);
         $output .= VirmanRenderer::display($v, $indent);
      }
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
      return "ЛД " . Application::getCurrPaymentName();
   }

   // Ista funkcija kao u ListaReport
   public function getIznos(PaymentForEmploye $pfe) {
      $netoUkupno = self::getNeto($pfe, 'netoUkupno');
      $ukObustave = Obustave::ukupnoObustave($pfe->employe->getValue('id'), $pfe->payment, $netoUkupno);
      
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
