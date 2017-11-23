<?php

require_once 'util/class.Data.inc';

/*
 * Klasa definise strukturu entiteta koji opisuje Radnju.
 */
class ShopStructure extends Structure {

   public function getDataDesc() {
      return array(
          'id' => array('integer', 0), 'naziv' => array('string', ''),
          'adresa' => array('string', ''), 'mesto' => array('string', ''),
          'telefon' => array('string', ''), 'status' => array('string', 'Aktivan'),
          'ziroracun' => array('string', '')
      );
   }
   
   public function getFormDesc() {
      return array(
          'id' => array('hidden', ''), 'naziv' => array('text', 'Назив'),
          'adresa' => array('text', 'Адреса'), 'mesto' => array('text', 'Место'),
          'telefon' => array('text', 'Телефон'), 'status' => array('status', 'Статус'),
          'ziroracun' => array('text', 'Жиро рачун')
      );
   }

   public function getValidateDesc() {
      return array(
          'id' => array('not_negative'),
          'naziv' => array('not_empty'),
          'status' => array('not_empty'),
      );
   }

   public function getDBDesc() {
      return array(
          'id' => 'sw', 'naziv' => 'siu', 'adresa' => 'siu',
          'mesto' => 'siu', 'telefon' => 'siu', 'status' => 'siu',
          'ziroracun' => 'siu'
      );
   }

   /*
    * Definise abstraktni metod roditeljske klase i vraca naziv strukture.
    */
   public function getTableName() {
      return "SHOP";
   }
}

/*
 * Klasa koja definise entitet za jednu Radnju.
 * Koristi opstiju Data klasu,
 * kao i klasu za opis strukture entiteta Radnje.
 */
class Shop extends Data {
   /*
    * Konstruktor.
    */
   public function __construct() {
      parent::__construct(new ShopStructure);
   }
}
