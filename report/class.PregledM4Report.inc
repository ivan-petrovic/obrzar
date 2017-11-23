<?php

require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'karnet/class.KarnetOstalo.inc';
require_once 'class.ReportPage.inc';
require_once 'report/class.PFE.inc';
require_once 'report/class.PFERender.inc';
/*
class DatumObracuna {
   public $mesec;
   public $godina;
   public $tipObracuna;   // 'Akontacija' ili 'Konacni'
   
   public function __construct($mesec, $godina, $tipObracuna) {
      $this->mesec = $mesec;
      $this->godina = $godina;
      $this->tipObracuna = $tipObracuna;
   }

   public function equal(DatumObracuna $datobr) {
      if($datobr->godina != $this->godina) { return false; }
      if($datobr->mesec != $this->mesec) { return false; }
      if($datobr->tipObracuna != $this->tipObracuna) { return false; }
      return true;
   }

   public function lessThan(DatumObracuna $datobr) {
      if($this->godina < $datobr->godina) { return true; }
      
      if($this->godina == $datobr->godina) {
         if($this->mesec < $datobr->mesec) { return true; }
         if($this->mesec > $datobr->mesec) { return false; }
         
         // $this->mesec == $datobr->mesec
         if($this->tipObracuna == 'Akontacija' && $datobr->tipObracuna == 'Konacni') { return true; }
         return false;
      }
      
      // $this->godina > $datobr->godina
      return false;
   }
   
   public function increment() {
      if($this->tipObracuna == 'Akontacija') {
         $this->tipObracuna = 'Konacni';
      } else {
         $this->tipObracuna = 'Akontacija';
         $this->mesec++;
         if($this->mesec > 12) {
            $this->mesec = 1;
            $this->godina++;
         }
      }
   }
   
   public function display() {
      echo "{$this->mesec} - {$this->godina} - {$this->tipObracuna} </br>";
   }
}
*/
class PregledM4Report extends ReportPage implements RenderHTML {
   protected $empID = 0;
   protected $odMesec = 1;
   protected $odGodina = 2015;
   protected $doMesec = 12;
   protected $doGodina = 2015;
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Преглед за М4 образац");
      $this->sectionContent();

      $this->closeSection();
   }

   protected function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      }
      echo $this->makeForm();
      echo $this->printReport();
   }

   protected function processForm() {
      $this->empID = filter_input(INPUT_POST, 'zaposleni', FILTER_VALIDATE_INT, array('options' => array('default' => 0)));
      $this->odGodina = filter_input(INPUT_POST, 'odGodina', FILTER_VALIDATE_INT, array('options' => array('default' => 2015)));
      $this->doGodina = $this->odGodina;
   }

   protected function makeForm() {
      $output  = "";
      $scriptName = "report.php?opID=" . $this->op;
      $output .= FormMaker::openForm($scriptName, 5);

      $output .= EmployeFormElement::make("Запослени", "zaposleni", $this->empID, self::$indent+3);
      $output .= "<br/>\n";
      $output .= FormMaker::textElement('За годину', "odGodina", $this->odGodina, self::$indent+3);
      $output .= "<br/>\n";

      $output .= FormMaker::submitElement('Примени', 6);
      
      $output .= FormMaker::closeForm(5);
      return $output;
   }

   protected function printReport() {
      $output  = "";

      if($this->empID == 0) {
         return $output;
      }
      
      $employe = EmployeDB::load($this->empID);
      $nizObracuna = PaymentDB::loadKarnetZarade(Application::getCurrFirmID(),
              $this->odMesec, $this->odGodina, $this->doMesec, $this->doGodina);

      $output .= $this->makeTableZaglavlje($employe);

      $brutoUk = 0;
      $doprinosUk = 0;
      $ceoMesecUk = 0;
      $brDanaTotal = 0;
      $bruto30Uk = 0;
      $PIO30Uk = 0;
      $bolDo30Uk = 0;
      $bolDo30PIOUk = 0;
      
      foreach ($nizObracuna as $obracun) {
         $bruto = 0;
         $doprinos = 0;
         $ceoMesec = 0;
         $brDana = 0;
         $bruto30 = 0;
         $PIO30 = 0;
	 $bolDo30 = 0;
         $bolDo30PIO = 0;
         
         if(KarnetDB::exist($this->empID, $obracun->getValue('id'))) {
            $karnet = KarnetDB::load($this->empID, $obracun->getValue('id'));
            $pfe = new PaymentForEmploye($employe, $obracun, $karnet);
            
            $bruto = $pfe->brutoUkupnoKon();
            $doprinos = $pfe->konacniDoprinos->doprinosPIO() + $pfe->konacniDoprinos->doprinosPIOPOslodavac();
            $bolDo30 = $pfe->konacniNaknade->naknadaBolDo30D() + $pfe->konacniNaknade->naknadaBolDo30D100Posto();
            $bolDo30PIO = $bolDo30 * ($pfe->payment->getValue('stopaPIO') + $pfe->payment->getValue('stopaPIOPoslodavac')) / 100;

            if($pfe->karnet->satiUkupno() >= $pfe->payment->getValue('satiUMesecu')) {
               $ceoMesec = 1;
               $brDana = 0;
            } else {
               $ceoMesec = 0;
               $brDana = ceil($pfe->karnet->satiUkupno() / $pfe->payment->getValue('satiUMesecu') * 30);
            }
         }
	 /*
         if(KarnetOstaloDB::exist($this->empID, $obracun->getValue('id'))) {
            $ko = KarnetOstaloDB::load($this->empID, $obracun->getValue('id'));
            $bruto30 = $ko->getValue('iznosBruto30');
            $PIO30 = $ko->getValue('iznosPIO30');
         }
	 */

         $brutoUk += $bruto;
         $doprinosUk += $doprinos;
         $ceoMesecUk += $ceoMesec;
         $brDanaTotal += $brDana;
         $bruto30Uk += $bruto30;
         $PIO30Uk += $PIO30; 
	 $bolDo30Uk += $bolDo30;
         $bolDo30PIOUk += $bolDo30PIO;

         $output .= $this->makeRow($obracun->getValue('mesec'), $obracun->getValue('godina'),
                 $this->formatirajBrMD($ceoMesec, $brDana), $bruto, $doprinos, $bolDo30, $bolDo30PIO);
      }
      
      $ceoMesecUk += (int)($brDanaTotal / 30);
      $brDanaUk = ($brDanaTotal % 30);
      
      $output .= $this->makeRow('', 'Укупно', $this->formatirajBrMD($ceoMesecUk, $brDanaUk), $brutoUk, $doprinosUk, $bolDo30Uk, $bolDo30PIOUk);
      $output .= $this->closeTable(2);
      
      return $output;
   }
   
   protected function makeTableZaglavlje(Employe $employe) {
      $output  = "";
      $output .= "<h3>" . $employe->getValue('prezime') . " " . $employe->getValue('ime') . "</h3>\n";
      $output .= "<h4>ЈМБГ: " . $employe->getValue('jmbg') . "</h4>\n";
      
      $output .= $this->openTable("spisak", 2);
      $output .= str_repeat("\t", self::$indent+3) . "<tr>\n";
      
      $output .= str_repeat("\t", self::$indent+3) . "<th>Месец</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Година</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th class=\"sire\">Месеци и дана</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Бруто (без бол. до 30 дана)</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Допринос за ПИО ((без бол.)</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Накнада за бол. до 30 дана</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>ПИО за бол. до 30 дана</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function makeRow($mesec, $godina, $brMD, $bruto, $doprinos, $bolDo30, $bolDo30PIO) {
      $fbruto = number_format($bruto - $bolDo30, 2, ",", ".");
      $fdoprinos = number_format($doprinos - $bolDo30PIO, 2, ",", ".");
      $fbruto30 = number_format($bolDo30, 2, ",", ".");
      $fPIO30 = number_format($bolDo30PIO, 2, ",", ".");
      
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$mesec}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$godina}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$brMD}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fbruto}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fdoprinos}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fbruto30}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fPIO30}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }
   
   protected function formatirajBrMD($brMes, $brDana) {
      $output  = "";

      if($brMes === 1) {
         $output .= $brMes . " месец";
      } elseif($brMes === 2) {
         $output .= $brMes . " месеца";
      } elseif($brMes === 3) {
         $output .= $brMes . " месеца";
      } elseif($brMes === 4) {
         $output .= $brMes . " месеца";
      } else {
         $output .= $brMes . " месеци";
      }
      
      if($brDana <> 0) {
         $output .= ", " . $brDana . " дана";
      }
      return $output;
   }
}
