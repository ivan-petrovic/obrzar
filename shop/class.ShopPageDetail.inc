<?php

require_once 'class.ShopDB.inc';
require_once 'class.ShopPage.inc';

class ShopDetails extends ShopPage implements RenderHTML {
   // Radnja ciji se detalji pregledaju.
   protected $shop;
   /*
    * Konstruktor.
    */
   public function __construct($op, $sid, $indent) {
      parent::__construct($op, $sid, $indent);
      $this->shop = ShopDB::load($sid);
   }
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Преглед радње");
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      echo ShopRender::display($this->shop, 2);
      echo $this->backToListButton();
      echo $this->updateButton();
   }
}

class ShopRender extends CommonPage {
   /**
    * Funkcija za prikaz detalja Radnje u HTML-u.
    * @param Shop - radnja koja se prikazuje
    * @param integer indent - indentacija u ispisu firme
    * @return string
    */
   public static function display(Shop $shop, $indent) {
      $output = self::openTable("detalji", $indent);
      
      $nizel = self::displayElements();
      $cnt = count($nizel);
      for($i = 0; $i < $cnt; $i += 2) {
         $output .= self::makeRow($shop, $nizel[$i], $nizel[$i+1], $indent+4);
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
   
   protected static function makeRow(Shop $shop, $el, $label, $indent) {
      $output  = str_repeat("\t", $indent);
      
      $specel = self::specialElements();
      if( array_key_exists($el, $specel) ) {
         $output .= "<tr><th>{$label}</th><td>" . self::$specel[$el]($shop) . "</td></tr>\n";
      } else {
         $output .= "<tr><th>{$label}</th><td>" . $shop->getValue($el) . "</td></tr>\n";
      }

      return $output;
   }

   /**
    * Funkcija za prikaz statusa radnje.
    * @return string
    */
   protected static function getStatusString(Shop $shop) {
      return StatusFormElement::getName($shop->getValue('status'));
   }
}
