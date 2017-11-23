<?php

require_once 'util/class.Data.inc';

/*
 * Klasa definise strukturu entiteta koji opisuje Obustavu.
 */
class DeductionStructure extends Structure {
   // id - sifra obustave
   // zaposleniID - sifra zaposlenog kome obustava pripada
   // radnjaID - sifra radnje na koju se obustava odnosi
   // tipObustave - standardna u vise rata(1), procenat od neto zarade (2),...
   // tipPlacanja - pri svakoj isplati(1), samo na akontaciji(2) ili samo na konacnom(3)
   // mesec, godina, tipObracuna - Mesec, godina i tip obracuna
   // (akontacija ili konacni) od kada pocinje obustava.

   /*
    * Definise abstraktni metod roditeljske klase i vraca opis strukture.
    */
   public function getDataDesc() {
      return array(
          'id' => array('integer', 0), 'zaposleniID' => array('integer', 0),
          'ukIznos' => array('double', 0.0), 'brrata' => array('integer', 0),
          'radnjaID' => array('integer', 0), 'tipObustave' => array('integer', 1),
          'tipPlacanja' => array('integer', 1), 'mesec' => array('integer', 1),
          'godina' => array('integer', 2015), 'tipObracuna' => array('string', 'Konacni')
      );
   }
   
   public function getFormDesc() {
      return array(
          'id' => array('hidden', ''), 'zaposleniID' => array('hidden', ''),
          'ukIznos' => array('text', 'Укупан износ'), 'brrata' => array('text', 'Број рата'),
          'radnjaID' => array('radnja', 'Радња'), 'tipObustave' => array('tip_obustave', 'Тип Обуставе'),
          'tipPlacanja' => array('tip_placanja', 'Тип плаћања'), 'mesec' => array('mesec', 'Месец'),
          'godina' => array('text', 'Година'), 'tipObracuna' => array('tip_obracuna', 'Тип обрачуна')
      );
   }
   
   public function getValidateDesc() {
      return array(
          'id' => array('not_negative'), 'zaposleniID' => array('not_negative'),
          'ukIznos' => array('positive'), 'brrata' => array('positive'),
          'radnjaID' => array('not_negative'), 'tipObustave' => array('not_negative'),
          'tipPlacanja' => array('not_negative'), 'mesec' => array('not_negative'),
          'godina' => array('not_negative'), 'tipObracuna' => array('not_empty')
      );
   }
   
   public function getDBDesc() {
      return array(
          'id' => 'sw', 'zaposleniID' => 'siu', 'ukIznos' => 'siu',
          'brrata' => 'siu', 'radnjaID' => 'siu', 'tipObustave' => 'siu',
          'tipPlacanja' => 'siu', 'mesec' => 'siu', 'godina' => 'siu',
          'tipObracuna' => 'siu'
      );
   }

   /*
    * Definise abstraktni metod roditeljske klase i vraca naziv strukture.
    */
   public function getTableName() {
      return 'DEDUCTION';
   }
}

/*
 * Klasa koja definise entitet za jednu Obustavu.
 * Koristi opstiju Data klasu,
 * kao i klasu za opis strukture entiteta Obustave.
 */
class Deduction extends Data {
   // Niz rata koje pripadaju obustavi.
   public $rate = array();
   
   /*
    * Konstruktor.
    * @param integer $eid - id zaposlenog kome pripada obustava.
    */
   public function __construct($eid = 0) {
      parent::__construct(new DeductionStructure);
      if($eid !== 0) {
         $this->setValue('zaposleniID', $eid);
         $this->setValue('mesec', date("n"));
         $this->setValue('godina', date("Y"));
         $this->setValue('tipPlacanja', 3);
      }
   }

   /*
    * Vraca broj rata obustave.
    */
   public function getBrRata() {
      return count($this->rate);
   }
   
   /*
    * Vraca ukupan iznos obustave
    */
   public function getTotalAmount() {
      $tot = 0;
      foreach( $this->rate as $rata ) {
         $tot += $rata->getValue('iznos');
      }
      return $tot;
   }
}


/*
 * Klasa definise strukturu entiteta koji opisuje Ratu.
 */
class InstallmentStructure extends Structure {
   // id - Sifra rate.
   // obustavaID -ID obustave kojoj rata pripada.
   // iznos - Ukupan iznos rate.
   // mesec, godina, tipObracuna - Mesec, godina i tip obracuna
   // kada ce rata da se obustavi. Npr. 11 mesec, 2014. godine na akontaciji.
   
   /*
    * Definise abstraktni metod roditeljske klase i vraca opis strukture.
    */
   public function getDataDesc() {
      return array(
          'id' => array('integer', 0), 'obustavaID' => array('integer', 0),
          'iznos' => array('double', 0.0), 'mesec' => array('integer', 1),
          'godina' => array('integer', 2015), 'tipObracuna' => array('string', 'Konacni')
      );
   }

   public function getFormDesc() {
      return array(
          'id' => array('hidden', ''), 'obustavaID' => array('hidden', ''),
          'iznos' => array('text', 'Коефицијент'), 'mesec' => array('mesec', 'Месец'),
          'godina' => array('godina', 'Година'), 'tipObracuna' => array('tipObracuna', 'Тип обрачуна')
      );
   }

   public function getValidateDesc() {
      return array(
          'id' => array('not_negative'), 'obustavaID' => array('not_negative'),
          'iznos' => array('not_negative'), 'mesec' => array('not_negative'),
          'godina' => array('not_negative'), 'tipObracuna' => array('not_empty')
      );
   }

   public function getDBDesc() {
      return array(
          'id' => 'sw', 'obustavaID' => 'siu', 'iznos' => 'siu',
          'mesec' => 'siu', 'godina' => 'siu', 'tipObracuna' => 'siu'
      );
   }

   /*
    * Definise abstraktni metod roditeljske klase i vraca naziv strukture.
    */
   public function getTableName() {
      return 'INSTALLMENT';
   }
}

/*
 * Klasa koja definise entitet za jednu Ratu obustave.
 * Koristi opstiju Data klasu,
 * kao i klasu za opis strukture entiteta Rate.
 */
class Installment extends Data {
   /*
    * Konstruktor.
    */
   public function __construct() {
      parent::__construct(new InstallmentStructure);
   }
}
