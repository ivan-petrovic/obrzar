<?php

require_once 'util/class.Data.inc';

/*
 * Klasa definise strukturu entiteta koji opisuje Zaposlenog.
 */
class EmployeStructure extends Structure {
   /*
    * Definise abstraktni metod roditeljske klase i vraca opis strukture.
    */
   public function getDataDesc() {
      return array(
          'id' => array('integer', 0), 'firmaID' => array('integer', 0),
          'interniID' => array('integer', 0), 'tipZaposlenja' => array('integer', 0),
          'ime' => array('string', ''), 'prezime' => array('string', ''),
          'adresa' => array('string', ''), 'mesto' => array('string', ''),
          'telefon' => array('string', ''), 'status' => array('string', 'Aktivan'),
          'koeficijent' => array('double', 0.0), 'donetMinuliRad' => array('string', '0-0-0'),
          'datumZaposlenja' => array('string', '0000-00-00'), 'jmbg' => array('string', ''),
          'datumRodjenja' => array('string', '0000-00-00'), 'pol' => array('string', 'm'),
          'strucnaSpremaID' => array('integer', 0), 'opstinaID' => array('integer', 0),
          'bankaID' => array('integer', 0), 'racun' => array('string', ''),
          'radnoMesto' => array('string', ''), 'opis' => array('string', '')
      );
   }
   
   public function getFormDesc() {
      return array(
          'id' => array('hidden', ''), 'firmaID' => array('hidden', ''),
          'interniID' => array('text', 'Интерна шифра'), 'tipZaposlenja' => array('tip_zaposlenja', 'Тип запослења'),
          'ime' => array('text', 'Име'), 'prezime' => array('text', 'Презиме'),
          'adresa' => array('text', 'Адреса'), 'mesto' => array('text', 'Место'),
          'telefon' => array('text', 'Телефон'), 'status' => array('status', 'Статус'),
          'koeficijent' => array('text', 'Коефицијент'), 'donetMinuliRad' => array('text', 'Донет минули рад (г-м-д)'),
          'datumZaposlenja' => array('text', 'Датум запослења (гггг-мм-дд)'), 'jmbg' => array('text', 'ЈМБГ'),
          'datumRodjenja' => array('text', 'Датум рођења (гггг-мм-дд)'), 'pol' => array('pol', 'Пол'),
          'strucnaSpremaID' => array('strucna_sprema', 'Стручна спрема'), 'opstinaID' => array('opstina', 'Општина'),
          'bankaID' => array('banka', 'Банка'), 'racun' => array('text', 'Жиро рачун'),
          'radnoMesto' => array('text', 'Радно место'), 'opis' => array('text', 'Напомена')
      );
   }

   public function getValidateDesc() {
      return array(
          'id' => array('not_negative'), 'firmaID' => array('not_negative'),
          'interniID' => array('not_negative'), 'tipZaposlenja' => array('not_negative'),
          'ime' => array('not_empty'), 'prezime' => array('not_empty'),
          'status' => array('not_empty'), 'koeficijent' => array('not_negative'),
          'donetMinuliRad' => array('donet_minuli'), 'datumZaposlenja' => array('datum'),
          'datumRodjenja' => array('datum'), 'strucnaSpremaID' => array('not_negative'),
          'opstinaID' => array('not_negative'), 'bankaID' => array('not_negative')
      );
   }

   public function getDBDesc() {
      return array(
          'id' => 'sw', 'firmaID' => 'siu', 'ime' => 'siu',
          'interniID' => 'siu', 'tipZaposlenja' => 'siu',
          'prezime' => 'siu', 'adresa' => 'siu', 'mesto' => 'siu',
          'telefon' => 'siu', 'status' => 'siu', 'koeficijent' => 'siu',
          'donetMinuliRad' => 'siu', 'datumZaposlenja' => 'siu',
          'jmbg' => 'siu', 'datumRodjenja' => 'siu', 'pol' => 'siu',
          'strucnaSpremaID' => 'siu', 'opstinaID' => 'siu',
          'bankaID' => 'siu', 'racun' => 'siu', 'radnoMesto' => 'siu',
          'opis' => 'siu'
      );
   }
   
   /*
    * Definise abstraktni metod roditeljske klase i vraca naziv strukture.
    */
   public function getTableName() {
      return 'EMPLOYE';
   }
}

/*
 * Klasa koja definise entitet za jednog Zaposlenog.
 * Koristi opstiju Data klasu,
 * kao i klasu za opis strukture entiteta Zaposlenog.
 */
class Employe extends Data {
   const MINULI_RAD_GODISNJE = .40;
   /*
    * Konstruktor.
    * @param integer $fid - id firme kojoj pripada zaposleni.
    */
   public function __construct($fid = 0) {
      parent::__construct(new EmployeStructure);
      if($fid !== 0) {
         $this->setValue('firmaID', $fid);
      }
   }

   /**
   * Funkcija koja racuna minuli rad na osnovu donetog minulog rada
   * i datuma zaposlenja. I naravno tekuceg datuma.
   * @return float minuli rad u procentima.
   */
   public function getMinuliRad() {
      if($this->getValue('datumZaposlenja') === '0000-00-00') { return 0; }
      $prosloOdZaposlenja = $this->getProsloOdZaposlenja();

      list($godina, $meseci, $dana) = explode('-', $this->getValue('donetMinuliRad'));
      $donetInterval = new DateInterval("P{$godina}Y{$meseci}M{$dana}D");

      $ref1 = new DateTime('2000-01-01'); $ref2 = clone $ref1; // proizvoljan datum kao referenca.
      $ref1->add($prosloOdZaposlenja); $ref1->add($donetInterval);

      // echo 'Total godina, meseci i dana: ', $ref2->diff($ref1)->format('%y/%m/%d') . '<br/>';
      $minuli = (int)($ref2->diff($ref1)->format("%y")) * self::MINULI_RAD_GODISNJE;
      return $minuli;
   }

   /**
   * Funkcija koja racuna ukupan staz za zaposlenog:
   * dakle donet staz + tekuci staz u firmi.
   * @return string G-M-D.
   */
   public function ukupnoStaza() {
      if($this->getValue('datumZaposlenja') === '0000-00-00') {
         return $this->getValue('donetMinuliRad');
      }
      $prosloOdZaposlenja = $this->getProsloOdZaposlenja();

      list($godina, $meseci, $dana) = explode('-', $this->getValue('donetMinuliRad'));
      $donetInterval = new DateInterval("P{$godina}Y{$meseci}M{$dana}D");

      $ref1 = new DateTime('2000-01-01'); $ref2 = clone $ref1; // proizvoljan datum kao referenca.
      $ref1->add($prosloOdZaposlenja); $ref1->add($donetInterval);
      
      return $ref2->diff($ref1)->format('%y-%m-%d');
   }

   /**
   * Funkcija koja racuna koliko je dana proslo od zaposlenja.
   * @return integer broj dana.
   */
  protected function getProsloOdZaposlenja() {
      $today = new DateTime('now');
      $dZaposlenje = new DateTime($this->getValue('datumZaposlenja'));
      return $dZaposlenje->diff($today);
   }
}
