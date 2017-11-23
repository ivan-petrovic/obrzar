<?php

require_once 'class.Firm.inc';
require_once 'utildb/class.Select.inc';
require_once 'utildb/class.Update.inc';
require_once 'utildb/class.Insert.inc';

/*
 * Klasa koja ucitava i snima objekte tipa Firm u mysql bazu.
 */
class FirmDB {
   /*
    * Ucitaj firmu iz baze.
    * @param integer $id - id firme.
    * @return Firm
    */
   static public function load($id) {
      $select = new Select();
      $select->from('firm');
      $select->condition("id = ? AND status <> 'Obrisan'", array($id));
      
      $result = $select->execute();
      $row = $result->fetch_assoc();
      
      return self::make($row);
   }
   
   protected static function make($row) {
      if($row) {
         $firm = new Firm();
         $firm->setData($row);
         return $firm;
      }
      trigger_error("FirmDB::make(): {$row['id']} not found.");
   }

   /*
    * Ucitaj sve firme iz baze.
    * @return array Firm
    */
   static public function loadAll() {
      $select = new Select();
      $select->from('firm');
      $select->condition("status <> 'Obrisan'");
      $result = $select->execute();

      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::make($row);
      }
      return $niz;
   }

   /*
    * Izmeni firmu u bazi.
    * @param Firm.
    * @return boolean
    */
   static public function update(Firm $firm) {
      $update = new Update();
      $update->what( $firm->getFieldNames('u') );
      $update->where('firm');
      $update->condition('id = ?');
      $update->values( array_merge($firm->getFieldValues('u'), array($firm->getValue('id'))) );
      $updresult = $update->execute();
      return ($updresult > 0 ? true : false);
   }

   /*
    * Upis nove firme u bazu.
    * @param Firm.
    * @return array(insert_id, affected_rows)
    */
   static public function insert(Firm $firm) {
      $insert = new Insert();
      $insert->what( $firm->getFieldNames('i') );
      $insert->where('firm');
      $insert->values( $firm->getFieldValues('i') );
      $insresult = $insert->execute();
      return $insresult;
   }

   /*
    * Brisanje firme iz baze. Ne radi se fizicko brisanje,
    * vec se samo status postavlja na 'Pasivan'
    * @param integer $id - id firme.
    * @return bool
    */
   static public function delete($id) {
      $update = new Update();
      $update->what( array('status') );
      $update->where('firm');
      $update->condition('id = ?');
      $update->values( array('Pasivan', $id) );
      $updresult = $update->execute();
      return ($updresult > 0 ? true : false);
   }
}
