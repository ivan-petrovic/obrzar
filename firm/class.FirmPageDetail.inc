<?php

require_once 'class.FirmDB.inc';
require_once 'class.FirmPage.inc';

class FirmDetails extends FirmPage implements RenderHTML {
   // Firma ciji se detalji pregledaju.
   protected $firm;
   
   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $indent) {
      parent::__construct($op, $fid, $indent);
      $this->firm = FirmDB::load($fid);
      if($fid !== Application::getCurrFirmID()) {
         Application::setFirm($this->firm);
      }
   }
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Преглед фирме");
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      echo FirmRender::display($this->firm, 2);
      echo $this->backToListButton();
      echo $this->updateButton();
   }
}

class FirmRender extends CommonPage {
   /**
    * Funkcija za prikaz detalja Firme u HTML-u.
    * @param Firm - firma koja se prikazuje
    * @param integer indent - indentacija u ispisu firme
    * @return string
    */
   public static function display(Firm $firm, $indent) {
      $output = self::openTable("detalji", $indent);
      
      $nizel = self::displayElements();
      $cnt = count($nizel);
      for($i = 0; $i < $cnt; $i += 2) {
         $output .= self::makeRow($firm, $nizel[$i], $nizel[$i+1], $indent+4);
      }
      $output .= self::closeTable($indent);
      
      return $output;
   }
   
   protected static function displayElements() {
      return array(
          'id', 'Шифра', 'naziv', 'Назив', 'adresa', 'Адреса', 'mesto', 'Место',
          'telefon', 'Телефон', 'tip', 'Тип', 'status', 'Статус', 'matbr', 'Матични број',
          'pib',  'ПИБ','sifdel', 'Шифра делатности', 'ziroracun', 'Жиро рачун', 'nazivBanke', 'Назив банке'
      );
   }

   protected static function specialElements() {
      return array(
          'tip' => "getTipString",
          'status' => "getStatusString", 
      );
   }
   
   protected static function makeRow(Firm $firm, $el, $label, $indent) {
      $output  = str_repeat("\t", $indent);
      
      $specel = self::specialElements();
      if( array_key_exists($el, $specel) ) {
         $output .= "<tr><th>{$label}</th><td>" . self::$specel[$el]($firm) . "</td></tr>\n";
      } else {
         $output .= "<tr><th>{$label}</th><td>" . $firm->getValue($el) . "</td></tr>\n";
      }

      return $output;
   }

   /**
    * Funkcija za prikaz statusa firme.
    * @return string
    */
   protected static function getStatusString(Firm $firm) {
      return StatusFormElement::getName($firm->getValue('status'));
   }
   
   /**
    * Funkcija za prikaz tipa firme.
    * @return string
    */
   protected static function getTipString(Firm $firm) {
      return TipFirmeFormElement::getName($firm->getValue('tip'));
   }
}
