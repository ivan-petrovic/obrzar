<?php

require_once 'class.EmployeDB.inc';
require_once 'class.EmployePage.inc';

class EmployeDetails extends EmployePage implements RenderHTML {
   // Zaposleni ciji se detalji pregledaju.
   protected $employe;
   
   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $eid, $indent) {
      parent::__construct($op, $fid, $eid, $indent);
      $this->employe = EmployeDB::load($this->eid);
   }
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Преглед запосленог");
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      echo EmployeRender::display($this->employe, 5);
      echo $this->backToListButton();
      echo $this->updateButton();
   }
}

class EmployeRender extends CommonPage {

   /**
    * Funkcija za prikaz Employee u HTML-u.
    * @param Employe - zaposleni koja se prikazuje
    * @param integer indent - indentacija u ispisu zaposlenog
    * @return string
    */
   public static function display(Employe $employe, $indent) {
      $output = self::openTable("detalji", $indent);
      
      $nizel = self::displayElements();
      $cnt = count($nizel);
      for($i = 0; $i < $cnt; $i += 2) {
         $output .= self::makeRow($employe, $nizel[$i], $nizel[$i+1], $indent+4);
      }
      $output .= self::closeTable($indent);
      
      return $output;
   }

   protected static function displayElements() {
      return array(
          'id', 'Шифра', 'firmaID', 'Шифра фирме',
          'interniID', 'Интерна шифра', 'tipZaposlenja', 'Тип запослења',
          'ime', 'Име', 'prezime', 'Презиме',
           'adresa', 'Улица и број', 'mesto', 'Место', 'telefon', 'Телефон',
          'status', 'Статус', 'koeficijent', 'Коефицијент', 'donetMinuliRad', 'Донет минули рад',
          'datumZaposlenja', 'Датум запослења', 'minuliRad', 'Минули рад', 'ukupanStaz', 'Укупан стаж [г-м-д]',
          'jmbg', 'ЈМБГ', 'datumRodjenja', 'Датум рођења',
          'pol', 'Пол', 'strucnaSpremaID',  'Стручна спрема', 'opstinaID',  'Општина',
          'bankaID', 'Банка', 'racun', 'Жиро рачун', 'radnoMesto', 'Радно место', 'opis', 'Напомена'
      );
   }

   protected static function specialElements() {
      return array(
          'status' => "getStatusString",
          'bankaID' => "getBankaString",
          'strucnaSpremaID' => "getStrSpremaString",
          'opstinaID' => "getOpstinaString",
          'minuliRad' => "getMinuliRadString",
          'ukupanStaz' => "getUkupanStazString",
          'tipZaposlenja' => "getTipZaposlenjaString",
          'pol' => "getPolString"
      );
   }

   protected static function makeRow(Employe $employe, $el, $label, $indent) {
      $output  = str_repeat("\t", $indent);
      
      $specel = self::specialElements();
      if( array_key_exists($el, $specel) ) {
         $output .= "<tr><th>{$label}</th><td>" . self::$specel[$el]($employe) . "</td></tr>\n";
      } else {
         $output .= "<tr><th>{$label}</th><td>" . $employe->getValue($el) . "</td></tr>\n";
      }

      return $output;
   }

   /**
    * Funkcija za prikaz statusa zaposlenog.
    * @return string
    */
   protected static function getStatusString(Employe $employe) {
      return StatusFormElement::getName($employe->getValue('status'));
   }
   
    /**
    * Funkcija za prikaz minulog rada u procentima.
    * @return string
    */
   public static function getMinuliRadString(Employe $employe) {
      return number_format($employe->getMinuliRad(), 2);
   }

    /**
    * Funkcija za prikaz ukupnog staza g-m-d.
    * @return string
    */
   public static function getUkupanStazString(Employe $employe) {
      return $employe->ukupnoStaza();
   }

   /**
    * Funkcija za prikaz naziva banke.
    * @return string
    */
   public static function getBankaString(Employe $employe) {
      return BankFormElement::getName($employe->getValue('bankaID'));
   }
   
   /**
    * Funkcija za prikaz strucne spreme.
    * @return string
    */
   public static function getStrSpremaString(Employe $employe) {
      return EducationFormElement::getName($employe->getValue('strucnaSpremaID'));
   }
   
   /**
    * Funkcija za prikaz opstine zaposlenog.
    * @return string
    */
   public static function getOpstinaString(Employe $employe) {
      return Municipality::getName($employe->getValue('opstinaID'));
   }

   /**
    * Funkcija za prikaz tipa zaposlenja zaposlenog.
    * @return string
    */
   public static function getTipZaposlenjaString(Employe $employe) {
      return TipZaposlenjaFormElement::getName($employe->getValue('tipZaposlenja'));
   }

   /**
    * Funkcija za prikaz pola zaposlenog.
    * @return string
    */
   public static function getPolString(Employe $employe) {
      return PolFormElement::getName($employe->getValue('pol'));
   }
}
