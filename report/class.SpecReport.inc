<?php

require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'class.ReportPage.inc';
require_once 'report/class.PFE.inc';
require_once 'report/class.PFERender.inc';

class SpecReport extends ReportPage implements RenderHTML {
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Спецификација за порез и доприносе за " . Application::getCurrPaymentName());
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
      $output .= $this->tabelaZaglavlje($indent);

      $ukOsnovica = 0; $ukPIO = 0; $ukZdr = 0;
      $ukNez = 0; $ukOP = 0; $ukPorez = 0;
      
      foreach ($nizPFE as $pfe) {
         $output .= $this->makeRow($pfe, $ukOsnovica, $ukPIO, $ukZdr, $ukNez, $ukOP, $ukPorez, $indent+1);
      }
      
      $output .= $this->tabelaFuter(number_format($ukOsnovica, 2,  ",", "."),
              number_format($ukPIO, 2,  ",", "."),number_format($ukZdr, 2,  ",", "."),
              number_format($ukNez, 2,  ",", "."),number_format($ukOP, 2,  ",", "."),
              number_format($ukPorez, 2,  ",", "."), $indent);
      return $output;
   }

   protected function tabelaZaglavlje($indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent) . "<table class=\"spec-lista\">\n";
      $output .= str_repeat("\t", $indent+1) . "<tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>Презиме и име</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>ЈМБГ</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>Основица</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>Осн. пореза</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>Порез</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>ПИО (ук.)</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>Здр. (ук.)</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>Незап. (ук.)</th>\n";
      // $output .= str_repeat("\t", $indent+1) . "<th>Шифра општине</th>\n";
      $output .= str_repeat("\t", $indent+1) . "</tr>\n";
      return $output;
   }

   protected function tabelaFuter($ukOsnovica, $ukPIO, $ukZdr, $ukNez, $ukOP, $ukPorez, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<td colspan=\"2\"></td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$ukOsnovica}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$ukOP}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$ukPorez}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$ukPIO}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$ukZdr}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$ukNez}</td>\n";
      // $output .= str_repeat("\t", $indent+1) . "<td>{$opstina}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "</tr>\n";

      $output .= str_repeat("\t", $indent) . "</table>\n";
      return $output;
   }
   
   protected function makeRow(PaymentForEmploye $pfe, &$ukOsnovica, &$ukPIO, &$ukZdr, &$ukNez, &$ukOP, &$ukPorez, $indent) {
      $jmbg = $pfe->employe->getValue('jmbg');
      $ime = $pfe->employe->getValue('prezime') . " " . $pfe->employe->getValue('ime');
      // $opstina = $pfe->employe->getValue('opstinaID');
      
      $osnovica = $this->getDoprinos($pfe, 'osnovicaDoprinosa');
      $osnovicaPoreza = $this->getPorez($pfe, 'osnovicaPorez');
      $porez = $this->getPorez($pfe, 'porez');      
      $PIOZap = $this->getDoprinos($pfe, 'doprinosPIO');
      $ZdrZap = $this->getDoprinos($pfe, 'doprinosZdravstvo');
      $NezZap = $this->getDoprinos($pfe, 'doprinosNezaposlenost');
      $PIOPosl = $this->getDoprinos($pfe, 'doprinosPIOPoslodavac');
      $ZdrPosl = $this->getDoprinos($pfe, 'doprinosZdravstvoPoslodavac');
      $NezPosl = $this->getDoprinos($pfe, 'doprinosNezaposlenostPoslodavac');
      
      $ukOsnovica += $osnovica;
      $ukPIO += $PIOZap + $PIOPosl;
      $ukZdr += $ZdrZap + $ZdrPosl;
      $ukNez += $NezZap + $NezPosl;
      $ukOP += $osnovicaPoreza;
      $ukPorez += $porez;
      
      $fosnovica = number_format($osnovica, 2,  ",", ".");
      $fPIO = number_format($PIOZap + $PIOPosl, 2,  ",", ".");
      $fZdr = number_format($ZdrZap + $ZdrPosl, 2,  ",", ".");
      $fNez = number_format($NezZap + $NezPosl, 2,  ",", ".");
      $fOsnovicaPoreza = number_format($osnovicaPoreza, 2,  ",", ".");
      $fPorez = number_format($porez, 2,  ",", ".");

      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$ime}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$jmbg}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$fosnovica}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$fOsnovicaPoreza}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$fPorez}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$fPIO}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$fZdr}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "<td>{$fNez}</td>\n";
      // $output .= str_repeat("\t", $indent+1) . "<td>{$opstina}</td>\n";
      $output .= str_repeat("\t", $indent+1) . "</tr>\n";
      return $output;
   }   

   public static function getDoprinos(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaDoprinos->$fname();
      } else {
         return $pfe->konacniDoprinos->$fname();
      }
   }
   
   public static function getPorez(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaPorez->$fname();
      } else {
         return $pfe->konacniPorez->$fname();
      }
   }
}
