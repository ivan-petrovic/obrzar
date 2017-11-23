<?php

require_once 'class.DeductionDB.inc';
require_once 'class.DeductionPage.inc';

class DeductionDetails extends DeductionPage implements RenderHTML {
   // Obustava ciji se detalji pregledaju.
   protected $deduction;
   /*
    * Konstruktor.
    */
   public function __construct($op, $did, $indent) {
      parent::__construct($op, $did, $indent);
      $this->deduction = DeductionDB::load($this->did);
   }
   
   function renderHTML() {
      $this->openSection();
      
      $employe = EmployeDB::load($this->deduction->getValue('zaposleniID'));
      $ime = $employe->getValue('ime') . " " . $employe->getValue('prezime');
      
      $this->sectionHeader("Преглед обуставе за " . $ime);
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      echo DeductionRender::display($this->deduction, 5);
      echo $this->backToListButton();
      echo $this->updateButton();
   }
}

class DeductionRender extends CommonPage {
   /**
    * Funkcija za prikaz Obustave u HTML-u.
    * Prikazuje se kao jedan red u okviru tabele.
    * @return string
    */
   public static function display(Deduction $deduction, $indent) {
      $output  = self::openTable("detalji", $indent);
      $output .= str_repeat("\t", $indent+1) . "<tr><th>Шифра</th><th>Радња</th>";
      $output .= "<th>Број рата</th><th>Укупан износ</th></tr>\n";
      $output .= str_repeat("\t", $indent+1);
      $shopnaz = ShopFormElement::getName($deduction->getValue('radnjaID'));
      $output .= "<tr><td>{$deduction->getValue('id')}</td><td>{$deduction->getValue('radnjaID')} {$shopnaz}</td>";
      $output .= "<td>{$deduction->getBrRata()}</td><td>{$deduction->getTotalAmount()}</td></tr>\n";
      $output .= self::closeTable(2);
      
      $output .= self::prikazRata($deduction, $indent);
      
      return $output;
   }

   /**
    * Funkcija za prikaz rata koje pripadaju Obustavi.
    * @return string
    */
   public static function prikazRata(Deduction $deduction, $indent) {
      $output  = self::openTable("detalji", 2);
      $output .= str_repeat("\t", $indent+1) . "<tr><th rowspan=\"2\">Шифра</th>";
      $output .= "<th rowspan=\"2\">Износ</th><th colspan=\"3\">Доспеће</th></tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<tr>";
      $output .= "<th>Месец</th><th>Година</th><th>Тип обрачуна</th></tr>\n";
      
      foreach ($deduction->rate as $rata) {
         $output .= self::makeRow($rata, $indent);
      }

      $output .= self::closeTable(2);
      return $output;
   }
   
   protected static function makeRow(Installment $rata, $indent) {
      $output  = str_repeat("\t", $indent+1);
      
      $output .= "<tr><td>{$rata->getValue('id')}</td><td>{$rata->getValue('iznos')}</td>";
      $output .= "<td>{$rata->getValue('mesec')}</td><td>{$rata->getValue('godina')}</td>";
      $output .= "<td>" . self::getTipObracunaString($rata->getValue('tipObracuna')) . "</td></tr>\n";

      return $output;
   }

   /**
    * Funkcija za prikaz statusa zaposlenog.
    * @return string
    */
   public static function getTipObracunaString($tip) {
      return $tip === "Akontacija" ? "Аконтација" : "Коначни обрачун";
   }
}
