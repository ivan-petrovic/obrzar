<?php

require_once 'class.BankDB.inc';
require_once 'class.BankPage.inc';

class BankDetails extends BankPage implements RenderHTML {
   // Banka ciji se detalji pregledaju.
   protected $bank;
   /*
    * Konstruktor.
    */
   public function __construct($op, $bid, $indent) {
      parent::__construct($op, $bid, $indent);
      $this->bank = BankDB::load($bid);
   }
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Преглед банке");
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      echo BankRender::display($this->bank, 2);
      echo $this->backToListButton();
      echo $this->updateButton();
   }
}

class BankRender extends CommonPage {
   /**
    * Funkcija za prikaz detalja Banke u HTML-u.
    * @param Bank - banka koja se prikazuje
    * @param integer indent - indentacija u ispisu firme
    * @return string
    */
   public static function display(Bank $bank, $indent) {
      $output = self::openTable("detalji", $indent);
      
      $nizel = self::displayElements();
      $cnt = count($nizel);
      for($i = 0; $i < $cnt; $i += 2) {
         $output .= self::makeRow($bank, $nizel[$i], $nizel[$i+1], $indent+4);
      }
      $output .= self::closeTable($indent);
      
      return $output;
   }

   protected static function displayElements() {
      return array(
          'id', 'Шифра', 'naziv', 'Назив', 'adresa', 'Адреса', 'mesto', 'Место',
          'telefon', 'Телефон', 'status', 'Статус', 'ziroracun', 'Жиро рачун'
      );
   }

   protected static function specialElements() {
      return array(
          'status' => "getStatusString"
      );
   }
   
   protected static function makeRow(Bank $bank, $el, $label, $indent) {
      $output  = str_repeat("\t", $indent);
      
      $specel = self::specialElements();
      if( array_key_exists($el, $specel) ) {
         $output .= "<tr><th>{$label}</th><td>" . self::$specel[$el]($bank) . "</td></tr>\n";
      } else {
         $output .= "<tr><th>{$label}</th><td>" . $bank->getValue($el) . "</td></tr>\n";
      }

      return $output;
   }

   /**
    * Funkcija za prikaz statusa banke.
    * @return string
    */
   protected static function getStatusString(Bank $bank) {
      return StatusFormElement::getName($bank->getValue('status'));
   }
}
