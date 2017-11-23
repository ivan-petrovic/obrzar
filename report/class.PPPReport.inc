<?php

require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'class.ReportPage.inc';
require_once 'report/class.PFE.inc';
require_once 'report/class.PFERender.inc';

class PPPReport extends ReportPage implements RenderHTML {
   protected $odMesec = 1;
   protected $odGodina = 2015;
   protected $doMesec = 1;
   protected $doGodina = 2015;
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Спецификација за ППП образац");
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      echo $this->makeForm();
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
         echo $this->printReport();
      }
   }

   protected function makeForm() {
      $output  = "";
      $scriptName = "report.php?opID=" . $this->op;
      $output .= FormMaker::openForm($scriptName, 5);

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

   protected function processForm() {
      $this->odMesec = filter_input(INPUT_POST, 'odMesec', FILTER_VALIDATE_INT, array('options' => array('default' => 1)));
      $this->odGodina = filter_input(INPUT_POST, 'odGodina', FILTER_VALIDATE_INT, array('options' => array('default' => 2015)));
      $this->doMesec = filter_input(INPUT_POST, 'doMesec', FILTER_VALIDATE_INT, array('options' => array('default' => 1)));
      $this->doGodina = filter_input(INPUT_POST, 'doGodina', FILTER_VALIDATE_INT, array('options' => array('default' => 2015)));
   }
   
   protected function getNizKarneta($empID, $nizObracuna) {
      $nizKarneta = array();
      foreach ($nizObracuna as $obracun) {
         if(KarnetDB::exist($empID, $obracun->getValue('id'))) {
            $nizKarneta[] = KarnetDB::load($empID, $obracun->getValue('id'));
         }
      }
      return $nizKarneta;
   }
   
   protected function printReport() {
      $output  = "";
      $output .= "<h3>Од {$this->odMesec}.{$this->odGodina}. до {$this->doMesec}.{$this->doGodina}.</h3>";
      $output .= $this->makeTableZaglavlje();
      
      // svi obracuni za tekucu firmu
      $nizObracuna = PaymentDB::loadKarnetZarade(Application::getCurrFirmID(),
              $this->odMesec, $this->odGodina, $this->doMesec, $this->doGodina);
      
      // niz zaposlenih za tekucu firmu
      $nizZaposlenih = EmployeDB::loadAll(Application::getCurrFirmID());
      
      foreach ($nizZaposlenih as $zaposleni) {
         $nizKarneta = $this->getNizKarneta($zaposleni->getValue('id'), $nizObracuna);

         $nizPFE = array();
         foreach ($nizKarneta as $karnet) {
            $payment = PaymentDB::load($karnet->getValue('obracunID'));
            $nizPFE[] = new PaymentForEmploye($zaposleni, $payment, $karnet);
         }

         if(count($nizPFE) == 0) {
            $output .= $this->emptyRow($zaposleni->getValue('jmbg'), $zaposleni->getValue('ime'), $zaposleni->getValue('prezime'));
         } else {
            $output .= $this->printTableBody($nizPFE);
         }
      }
      
      $output .= $this->closeTable(2);
      return $output;
   }

   protected function printTableBody($nizPFE) {
      $output  = "";
      
      $jmbg = $nizPFE[0]->employe->getValue('jmbg');
      $ime = $nizPFE[0]->employe->getValue('ime');
      $prezime = $nizPFE[0]->employe->getValue('prezime');
      $OPUk = 0;
      $porezUk = 0;
      $doprPIOUk = 0;
      $doprZdrUk = 0;
      $doprNezUk = 0;
      
      foreach ($nizPFE as $pfe) {
         if($pfe->payment->getValue('tipObracuna') === 'Konacni') {
            $OPUk += $pfe->konacniPorez->osnovicaPorez();
            $porezUk += $pfe->konacniPorez->porez();
            $doprPIOUk += $pfe->konacniDoprinos->doprinosPIO();
            $doprZdrUk += $pfe->konacniDoprinos->doprinosZdravstvo();
            $doprNezUk += $pfe->konacniDoprinos->doprinosNezaposlenost();
         }
      }
         
      $output .= $this->makeRow($jmbg, $ime, $prezime, $OPUk, $porezUk, $doprPIOUk, $doprZdrUk, $doprNezUk);
      $output .= $this->emptyRow($jmbg, $ime, $prezime);
      $output .= $this->emptyRow('', '', '');
      return $output;
   }

   protected function makeTableZaglavlje() {
      $output  = "";
      $output .= $this->openTable("spisak", 2);
      $output .= str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>ЈМБГ</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Име</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Презиме</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Осн.пореза</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Порез</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>ПИО</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Здравство</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th>Незапосленост</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function makeRow($jmbg, $ime, $prezime, $OP, $porez, $doprPIO, $doprZdr, $doprNez) {
      $fOP = number_format($OP, 2, ",", ".");
      $fporez = number_format($porez, 2, ",", ".");
      $fdoprPIO = number_format($doprPIO, 2, ",", ".");
      $fdoprZdr = number_format($doprZdr, 2, ",", ".");
      $fdoprNez = number_format($doprNez, 2, ",", ".");
      
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$jmbg}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$ime}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$prezime}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fOP}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fporez}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fdoprPIO}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fdoprZdr}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$fdoprNez}</td>\n";
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }
   
   protected function emptyRow($jmbg, $ime, $prezime) {
      return str_repeat("\t", self::$indent+3) . "<tr><td>{$jmbg}</td><td>{$ime}</td><td>{$prezime}</td><td></td><td></td><td></td><td></td><td></td></tr>\n";
   }
}
