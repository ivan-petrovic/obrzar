<?php

require_once 'util/class.Data.inc';

/*
 * Klasa definise strukturu entiteta koji opisuje Virman.
 */
class VirmanStructure extends Structure {
   /*
    * Definise abstraktni metod roditeljske klase i vraca opis strukture.
    */
   public function getDataDesc() {
      return array(
          'id' => array('integer', 0), 'duznik' => array('string', ''),
          'svrha' => array('string', ''), 'poverilac' => array('string', ''),
          'mesto' => array('string', ''), 'datum' => array('string', ''),
          'sifraPlacanja' => array('string', ''), 'valuta' => array('string', ''),
          'iznos' => array('string', ''), 'racunDuznika' => array('string', ''),
          'brojModela1' => array('string', ''), 'pozivNaBroj1' => array('string', ''),
          'racunPoverioca' => array('string', ''), 'brojModela2' => array('string', ''),
          'pozivNaBroj2' => array('string', ''), 'datumValute' => array('string', ''),
          'hitno' => array('string', ''), 'obrazacBr' => array('string', '1')
      );
   }
   
   public function getFormDesc() {
      return array(
          'id' => array('hidden', ''), 'duznik' => array('text', 'Дужник - налогодавац'),
          'svrha' => array('text', 'Сврха плаћања'), 'poverilac' => array('text', 'Поверилац - прималац'),
          'mesto' => array('text', 'Место пријема'), 'datum' => array('text', 'Датум пријема'),
          'sifraPlacanja' => array('text', 'Шифра плаћања'), 'valuta' => array('text', 'Валута'),
          'iznos' => array('text', 'Износ'), 'racunDuznika' => array('Рачун дужника - налогодавца', 'Име'),
          'brojModela1' => array('text', 'Број модела'), 'pozivNaBroj1' => array('text', 'Позив на број (задужење)'),
          'racunPoverioca' => array('text', 'Рачун повериоца - примаоца'), 'brojModela2' => array('text', 'Број модела'),
          'pozivNaBroj2' => array('text', 'Позив на број (одобрење)'), 'datumValute' => array('text', 'Датум валуте'),
          'hitno' => array('text', 'Хитно'),  'obrazacBr' => array('text', 'Образац број')
      );
   }

   public function getValidateDesc() {
      return array(
          'id' => array('not_negative'),
      );
   }

   public function getDBDesc() {
      return array(
          'id' => 'sw', 'duznik' => 'siu', 'svrha' => 'siu',
          'poverilac' => 'siu', 'mesto' => 'siu', 'datum' => 'siu',
          'sifraPlacanja' => 'siu', 'valuta' => 'siu', 'iznos' => 'siu',
          'racunDuznika' => 'siu', 'brojModela1' => 'siu', 'pozivNaBroj1' => 'siu',
          'racunPoverioca' => 'siu', 'brojModela2' => 'siu', 'pozivNaBroj2' => 'siu',
          'datumValute' => 'siu', 'hitno' => 'siu',  'obrazacBr' => 'siu'
      );
   }

   /*
    * Definise abstraktni metod roditeljske klase i vraca naziv strukture.
    */
   public function getTableName() {
      return 'VIRMAN';
   }
}

/*
 * Klasa koja definise entitet za jedan Virman.
 * Koristi opstiju Data klasu,
 * kao i klasu za opis strukture entiteta Virmana.
 */
class Virman extends Data {
   /*
    * Konstruktor.
    */
   public function __construct() {
      parent::__construct(new VirmanStructure);
   }
}

class VirmanRenderer {
   public static function display(Virman $v, $indent = 1) {
      $output  = str_repeat("\t", $indent) . "<div class=\"virman\">\n";
      $output .= self::leftSide($v, $indent+1);
      $output .= self::rightSide1($v, $indent+1);
      $output .= self::rightSide2($v, $indent+1);
      $output .= str_repeat("\t", $indent) . "</div>\n";
      return $output;
   }
   
   public static function leftSide(Virman $v, $indent) {
      $output  = "";
      $output .= self::element('nalog-opis', "НАЛОГ ЗА ПРЕНОС", $indent);
      $output .= self::element('duznik-opis', "дужник - налогодавац", $indent);
      $output .= self::element('duznik', $v->getValue('duznik'), $indent);
      $output .= self::element('svrha-opis', "сврха плаћања", $indent);
      $output .= self::element('svrha', $v->getValue('svrha'), $indent);
      $output .= self::element('poverilac-opis', "поверилац - прималац", $indent);
      $output .= self::element('poverilac', $v->getValue('poverilac'), $indent);
      $output .= self::element('pecat_potpis', "печат и потпис налогодавца", $indent);
      $md = $v->getValue('mesto') . ", " . $v->getValue('datum'); // . " godine";
      $output .= self::element('mesto_datum', $md, $indent);
      $output .= self::element('mesto_datum-opis', "место и датум пријема", $indent);
      return $output;
   }
   
   public static function rightSide1(Virman $v, $indent) {
      $output  = "";
      $output .= self::element('sifra_placanja-opis', "шифра плаћања", $indent);
      $output .= self::element('sifra_placanja', $v->getValue('sifraPlacanja'), $indent);
      $output .= self::element('valuta-opis', "валута", $indent);
      $output .= self::element('valuta', $v->getValue('valuta'), $indent);
      $output .= self::element('iznos-opis', "износ", $indent);
      $output .= self::element('iznos', " = " . $v->getValue('iznos'), $indent);
      $output .= self::element('racun_duznika-opis', "рачун дужника - налогодавца", $indent);
      $output .= self::element('racun_duznika', $v->getValue('racunDuznika'), $indent);
      $output .= self::element('broj_modela1-opis', "број модела", $indent);
      $output .= self::element('broj_modela1', $v->getValue('brojModela1'), $indent);
      $output .= self::element('poziv_na_broj1-opis', "позив на број (задужење)", $indent);
      $output .= self::element('poziv_na_broj1', $v->getValue('pozivNaBroj1'), $indent);

      return $output;
   }
   
   public static function rightSide2(Virman $v, $indent) {
      $output  = "";
      $output .= self::element('racun_poverioca-opis', "рачун повериоца - примаоца", $indent);
      $output .= self::element('racun_poverioca', $v->getValue('racunPoverioca'), $indent);
      $output .= self::element('broj_modela2-opis', "број модела", $indent);
      $output .= self::element('broj_modela2', $v->getValue('brojModela2'), $indent);
      $output .= self::element('poziv_na_broj2', $v->getValue('pozivNaBroj2'), $indent);
      $output .= self::element('poziv_na_broj2-opis', "позив на број (задужење)", $indent);
      $output .= self::element('datum_valute', $v->getValue('datumValute'), $indent);
      $output .= self::element('datum_valute-opis', "датум валуте", $indent);
      $output .= self::element('hitno', $v->getValue('hitno'), $indent);
      $output .= self::element('hitno-opis', "хитно", $indent);
      $output .= self::element('obrazac3-opis', "Образац бр. " . $v->getValue('obrazacBr'), $indent);

      return $output;
   }
   
   public static function element($className, $label, $indent) {
      $output  = str_repeat("\t", $indent);
      $output .= "<p class=\"{$className}\">{$label}</p>\n";
      return $output;
   }
}
