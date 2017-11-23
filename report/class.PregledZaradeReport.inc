<?php

require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'karnet/class.KarnetOstalo.inc';
require_once 'class.ReportPage.inc';
require_once 'report/class.PFE.inc';
require_once 'report/class.PFERender.inc';

class PregledZaradeReport extends ReportPage implements RenderHTML {
   protected $empID = 0;
   protected $odMesec = 1;
   protected $odGodina = 2015;
   protected $doMesec = 1;
   protected $doGodina = 2015;
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Карнет зарада за запослене");
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
      $this->odMesec = filter_input(INPUT_POST, 'odMesec', FILTER_VALIDATE_INT, array('options' => array('default' => 1)));
      $this->odGodina = filter_input(INPUT_POST, 'odGodina', FILTER_VALIDATE_INT, array('options' => array('default' => 2015)));
      $this->doMesec = filter_input(INPUT_POST, 'doMesec', FILTER_VALIDATE_INT, array('options' => array('default' => 1)));
      $this->doGodina = filter_input(INPUT_POST, 'doGodina', FILTER_VALIDATE_INT, array('options' => array('default' => 2015)));
   }

   protected function makeForm() {
      $output  = "";
      $scriptName = "report.php?opID=" . $this->op;
      $output .= FormMaker::openForm($scriptName, 5);

      $output .= EmployeFormElement::make("Запослени", "zaposleni", $this->empID, self::$indent+3);
      $output .= "<br/>\n";
      $output .= FormMaker::mesecElement('Од месеца', "odMesec", $this->odMesec, self::$indent+3);
      $output .= FormMaker::textElement('Од године', "odGodina", $this->odGodina, self::$indent+3);
      $output .= "<br/>\n";
      $output .= FormMaker::mesecElement('До месеца', "doMesec", $this->doMesec, self::$indent+3);
      $output .= FormMaker::textElement('До године', "doGodina", $this->doGodina, self::$indent+3);
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
      $netoUk = 0;
      $porezUk = 0;
      $doprinosUk = 0;
      $bruto30Uk = 0;
      $neto30Uk = 0;
      
      foreach ($nizObracuna as $obracun) {
         $bruto = 0;
         $neto = 0;
         $porez = 0;
         $doprinos = 0;
         $bruto30 = 0;
         $neto30 = 0;
         
         if(KarnetDB::exist($this->empID, $obracun->getValue('id'))) {
            $karnet = KarnetDB::load($this->empID, $obracun->getValue('id'));
            $pfe = new PaymentForEmploye($employe, $obracun, $karnet);

            $bruto = $pfe->brutoUkupnoKon();
            $neto = $pfe->konacniNeto->netoUkupno();
            $porez = $pfe->konacniPorez->porez();
            $doprinos = $pfe->konacniDoprinos->doprinos();
         }

         if(KarnetOstaloDB::exist($this->empID, $obracun->getValue('id'))) {
            $ko = KarnetOstaloDB::load($this->empID, $obracun->getValue('id'));
            $bruto30 = $ko->getValue('iznosBruto30');
            $neto30 = $ko->getValue('iznosNeto30');
         }

         $brutoUk += $bruto;
         $netoUk += $neto;
         $porezUk += $porez;
         $doprinosUk += $doprinos;
         $bruto30Uk += $bruto30;
         $neto30Uk += $neto30;      

         $output .= $this->makeRow($pfe->payment->getValue('mesec'), $pfe->payment->getValue('godina'),
                 $pfe->payment->getValue('tipObracuna'), $bruto, $neto, $porez, $doprinos, $bruto30, $neto30);
      }

      $output .= $this->makeRow('', '', '', $brutoUk, $netoUk, $porezUk, $doprinosUk, $bruto30Uk, $neto30Uk);
      $output .= $this->closeTable(2);
      
      return $output;
   }

   protected function makeTableZaglavlje(Employe $employe) {
      $output  = "";
      $output .= "<h3>" . $employe->getValue('prezime') . " " . $employe->getValue('ime') . "</h3>\n";
      $output .= $this->openTable("spisak", 2);
      $output .= str_repeat("\t", self::$indent+3) . "<tr>\n";
      
      $output .= str_repeat("\t", self::$indent+3) . "<th>Месец-Година-Тип</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Бруто</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Порез</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Допринос</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Нето</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Бруто 30</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Нето 30</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function makeRow($mesec, $godina, $tip, $bruto, $neto, $porez, $doprinos, $bruto30, $neto30) {
      $fbruto = number_format($bruto, 2, ",", ".");
      $fneto = number_format($neto, 2, ",", ".");
      $fporez = number_format($porez, 2, ",", ".");
      $fdoprinos = number_format($doprinos, 2, ",", ".");
      $ftip = empty($tip) ? 'Укупно' : "{$mesec}-{$godina}-" . TipObracunaFormElement::getName($tip);
      $fbruto30 = number_format($bruto30, 2, ",", ".");
      $fneto30 = number_format($neto30, 2, ",", ".");
      
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$ftip}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fbruto}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fporez}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fdoprinos}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fneto}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fbruto30}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fneto30}</td>\n";      
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }
}
