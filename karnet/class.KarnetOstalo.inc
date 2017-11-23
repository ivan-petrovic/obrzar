<?php

require_once 'util/class.Data.inc';
require_once 'utildb/class.Select.inc';
require_once 'utildb/class.Update.inc';
require_once 'utildb/class.Insert.inc';
require_once 'utildb/class.Delete.inc';

class KarnetOstaloStructure extends Structure {
// zaposleniID - sifra zaposlenog
// obracunID - sifra obracuna kojem karnet pripada
// iznosBruto30 - bruto zarada za bolovanje preko 30 dana
// iznosNeto30 - neto zarada za bolovanje preko 30 dana
// iznosPIO30 - PIO za bolovanje preko 30 dana
   
   /*
    * Definise abstraktni metod roditeljske klase i vraca opis strukture.
    */
   public function getDataDesc() {
      return array(
          'obracunID' => array('integer', 0),
          'zaposleniID' => array('integer', 0),
          'iznosBruto30' => array('double', 0.0),
          'iznosNeto30' => array('double', 0.0),
          'iznosPIO30' => array('double', 0.0)
      );
   }

   public function getFormDesc() {
      return array(
          'obracunID' => array('hidden', ''),
          'zaposleniID' => array('hidden', ''),
          'iznosBruto30' => array('text', 'Бруто боловање преко 30 дана'),
          'iznosNeto30' => array('text', 'Нето боловање преко 30 дана'),
          'iznosPIO30' => array('text', 'ПИО боловање преко 30 дана')
      );
   }
   
   public function getValidateDesc() {
      return array(
          'obracunID' => array('not_negative'),
          'zaposleniID' => array('not_negative'),
          'iznosBruto30' => array('not_negative'),
          'iznosNeto30' => array('not_negative'),
          'iznosPIO30' => array('not_negative')
      );
   }
   
   public function getDBDesc() {
      return array(
          'obracunID' => 'si',
          'zaposleniID' => 'si',
          'iznosBruto30' => 'siu',
          'iznosNeto30' => 'siu',
          'iznosPIO30' => 'siu'
      );
   }

   /*
    * Definise abstraktni metod roditeljske klase i vraca naziv strukture.
    */
   public function getTableName() {
      return 'KARNETOSTALO';
   }
}

/*
 * Klasa koja definise entitet za jednan KarnetOstalo.
 * Koristi opstiju Data klasu,
 * kao i klasu za opis strukture entiteta KarnetaOstalo.
 */
class KarnetOstalo extends Data {

   public function __construct(Payment $payment = null, Employe $employe = null) {
      parent::__construct(new KarnetOstaloStructure);
      if(!is_null($employe)) {
         $this->setValue('zaposleniID', $employe->getValue('id'));
      }
      if(!is_null($payment)) {
         $this->setValue('obracunID', $payment->getValue('id'));
      }
   }
}

/*
 * Klasa koja ucitava i snima objekte tipa KarnetOstalo u mysql bazu.
 */
class KarnetOstaloDB {
   /*
    * Ucitaj karnet_ostalo iz baze za tekuci obracun (u tekucoj firmi)
    * i trazenog zaposlenog.
    * @param integer $zaposleniID - id zaposlenog.
    * @param integer $obracunID - id obracuna.
    * @return Karnet
    */
   static public function load($zaposleniID, $obracunID) {
      $select = new Select();
      $select->from('karnetostalo');
      $select->condition("zaposleniID = ? AND obracunID = ?", array($zaposleniID, $obracunID));
      $result = $select->execute();
      $row = $result->fetch_assoc();
      // echo "$zaposleniID-$obracunID";
      return self::make($row);
   }

   static public function exist($zaposleniID, $obracunID) {
      $select = new Select();
      $select->from('karnetostalo');
      $select->condition("zaposleniID = ? AND obracunID = ?", array($zaposleniID, $obracunID));
      $result = $select->execute();
      $row = $result->fetch_assoc();

      if($row) {
         return true;
      }
      return false;
   }

   protected static function make($row) {
      if($row) {
         $karnet = new KarnetOstalo();
         $karnet->setData($row);
         return $karnet;
      }
      trigger_error("KarnetOstaloDB::make(): employe and/or payment not found.");
   }

   /*
    * Izmeni karnet ostalo u bazi.
    * @param KarnetOstalo.
    * @return boolean
    */
   static public function update(KarnetOstalo $karnet) {
      $update = new Update();
      $update->what( $karnet->getFieldNames('u') );
      $update->where('karnetostalo');
      $update->condition('zaposleniID = ? AND obracunID = ?');
      $update->values( array_merge($karnet->getFieldValues('u'),
              array($karnet->getValue('zaposleniID'), $karnet->getValue('obracunID'))) );
      $updresult = $update->execute();
      return $updresult;
   }

   /*
    * Upis novog karnetaostalo  u bazu.
    * @param KarnetOstalo.
    * @return boolean
    */
   static public function insert(KarnetOstalo $karnet) {
      $insert = new Insert();
      $insert->what( $karnet->getFieldNames('i') );
      $insert->where('karnetostalo');
      $insert->values( $karnet->getFieldValues('i') );
      $insresult = $insert->execute();
      return $insresult;
   }

   /*
    * Brisanje karneta ostalo iz baze. Radi se fizicko brisanje.
    * @param integer $zaposleniID - id zaposlenog.
    * @param integer $obracunID - id obracuna.
    * @return bool
    */
   static public function delete($zaposleniID, $obracunID) {
      $delete = new Delete();
      $delete->where('karnetostalo');
      $delete->condition("zaposleniID = ? AND obracunID = ?");
      $delete->values( array($zaposleniID, $obracunID) );
      $delresult = $delete->execute();
      return ($delresult > 0 ? true : false);
   }
}
