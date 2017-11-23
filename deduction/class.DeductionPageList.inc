<?php

require_once 'employe/class.EmployeDB.inc';
require_once 'payment/class.PaymentDB.inc';
require_once 'class.DeductionDB.inc';
require_once 'class.DeductionPage.inc';

class DeductionList extends DeductionPage implements RenderHTML {
   
   public function renderHTML() {
      $this->openSection();
      $this->sectionContent();
      $this->closeSection();
   }
   
   protected function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->sectionHeader("Списак обустава");
         $this->processForm();
      } else {
         $this->sectionHeader("Избор запосленог");
         $this->renderForm();
      }
   }

   function renderForm() {
      $scriptName = "deduction.php?opID=" . $this->op;
      echo str_repeat("\t", self::$indent+2) .  "<form action=\"$scriptName\" method=\"POST\">\n";
      echo EmployeFormElement::make("Запослени", "zaposleni", 0, self::$indent+3);
      echo FormMaker::submitElement("Изабери", self::$indent+3);
      echo str_repeat("\t", self::$indent+2) . "</form>\n";
   }

   function processForm() {
      $employe = $this->loadSelectedEmploye();
      $ime = $employe->getValue('ime') . " " . $employe->getValue('prezime');

      $nizObustava = DeductionDB::loadAll($this->eid);
      if( count($nizObustava) === 0) {
         echo str_repeat("\t", self::$indent+2) . "<p>Нема унетих обустава за запосленог <strong>{$ime}</strong>.</p>\n";
      } else {
         $this->renderList($nizObustava, $ime);
      }
      echo $this->backToListButton();
      echo $this->newButton();
   }

   function loadSelectedEmploye() {
      $this->eid =  filter_input(INPUT_POST, 'zaposleni', FILTER_VALIDATE_INT,
              array('options' => array('default' => 0)));
      return EmployeDB::load($this->eid);
   }
   
   protected function renderList($nizObustava, $ime) {
      echo str_repeat("\t", self::$indent+2) . "<p>За запосленог <strong>{$ime}</strong> унете су следеће обуставе:</p>\n";
      echo str_repeat("\t", self::$indent+2) . "<table class=\"spisak\">\n";
      echo str_repeat("\t", self::$indent+3) . "<tr><th>Шифра</th><th>Радња</th><th>Број рата</th><th>Плаћено рата</th>";
      echo "<th>Укупан износ</th><th colspan=\"3\">Акција</th></tr>\n";
      foreach ($nizObustava as $d) {
         echo $this->makeRow($d);
      }
      echo str_repeat("\t", self::$indent+2) . "</table>\n";
   }

   protected function makeRow(Deduction $d) {
      $shopnaz = ShopFormElement::getName($d->getValue('radnjaID'));
      $brplacenih = $this->calcBrPlacenih($d);
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= $this->makeDetailsLink($d->getValue('id'), $shopnaz, $d->getBrRata(), $d->getTotalAmount(), $brplacenih);
      $output .= $this->makeUpdateLink($d->getValue('id'));
      $output .= $this->makeDeleteLink($d->getValue('id'));
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function calcBrPlacenih(Deduction $d) {
      $lastPayment = PaymentDB::getLastPayment(Application::getCurrFirmID());
      $lastMesec = $lastPayment->getValue('mesec');
      $lastGodina = $lastPayment->getValue('godina');
      $cnt = 0;
      foreach ($d->rate as $rata) {
         $mes = $rata->getValue('mesec');
         $god = $rata->getValue('godina');
         if( ($god < $lastGodina) || ($god == $lastGodina && $mes <= $lastMesec)) {
            ++$cnt;
         }
      }
      return $cnt;
   }
   
   protected function makeDetailsLink($did, $shopnaz, $brrata, $iznos, $brplacenih) {
      $output  = str_repeat("\t", self::$indent+3);
      $output .= "<td>{$did}</td>";
      $output .= "<td>{$shopnaz}</td>";
      $output .= "<td>{$brrata}</td><td>{$brplacenih}/{$brrata}</td><td>{$iznos}</td>";
      $output .=  "<td><a href=\"deduction.php?opID=" . DeductionPageDispatcher::DETAILS_DEDUCTION;
      $output .=  "&deductionID=" . $did . "\">Преглед</a></td>\n";
      
      return $output;
   }

   protected function makeUpdateLink($did) {
      $output  = str_repeat("\t", self::$indent+3);
      $output .=  "<td><a href=\"deduction.php?opID=" . DeductionPageDispatcher::UPDATE_DEDUCTION;
      $output .=  "&deductionID=" . $did . "\">Измени</a></td>\n";
      return $output;
   }

   protected function makeDeleteLink($did) {
      $output  = str_repeat("\t", self::$indent+3);
      $output .=  "<td><a href=\"deduction.php?opID=" . DeductionPageDispatcher::DELETE_DEDUCTION;
      $output .=  "&deductionID=" . $did . "\">Обриши</a></td>\n";
      return $output;
   }
}
