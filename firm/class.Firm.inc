<?php

require_once 'util/class.Data.inc';

/*
 * Klasa definise strukturu entiteta koji opisuje Firmu.
 */
class FirmStructure extends Structure {

   public function getDataDesc() {
      return array(
          'id' => array('integer', 0), 'naziv' => array('string', ''),
          'adresa' => array('string', ''), 'mesto' => array('string', ''),
          'telefon' => array('string', ''),'tip' => array('integer', 1),
          'status' => array('string', 'Aktivan'), 'matbr' => array('string', ''),
          'pib' => array('string', ''), 'sifdel' => array('string', ''),
          'ziroracun' => array('string', ''), 'nazivBanke' => array('string', '')
      );
   }
   
   public function getFormDesc() {
      return array(
          'id' => array('hidden', ''), 'naziv' => array('text', 'Назив'),
          'adresa' => array('text', 'Адреса'), 'mesto' => array('text', 'Место'),
          'telefon' => array('text', 'Телефон'), 'tip' => array('tip_firme', 'Тип'),
          'status' => array('status', 'Статус'), 'matbr' => array('text', 'Матични број'),
          'pib' => array('text', 'ПИБ'), 'sifdel' => array('text', 'Шифра делатности'),
          'ziroracun' => array('text', 'Жиро рачун'), 'nazivBanke' => array('text', 'Назив банке')
      );
   }

   public function getValidateDesc() {
      return array(
          'id' => array('not_negative'),
          'naziv' => array('not_empty'),
          'tip' => array('not_negative'),
          'status' => array('not_empty')
      );
   }

   public function getDBDesc() {
      return array(
          'id' => 'sw', 'naziv' => 'siu',
          'adresa' => 'siu', 'mesto' => 'siu',
          'telefon' => 'siu', 'tip' => 'siu',
          'status' => 'siu', 'matbr' => 'siu',
          'pib' => 'siu', 'sifdel' => 'siu',
          'ziroracun' => 'siu', 'nazivBanke' => 'siu'
      );
   }

   /*
    * Definise abstraktni metod roditeljske klase i vraca naziv strukture.
    */
   public function getTableName() {
      return 'FIRM';
   }
}

/*
 * Klasa koja definise entitet za jednu Firmu.
 * Koristi opstiju Data klasu,
 * kao i klasu za opis strukture entiteta Firme.
 */
class Firm extends Data {
   /*
    * Konstruktor.
    */
   public function __construct() {
      parent::__construct(new FirmStructure);
   }
}
