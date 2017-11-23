<?php

require_once 'class.Bank.inc';
require_once 'utildb/class.Select.inc';
require_once 'utildb/class.Update.inc';
require_once 'utildb/class.Insert.inc';

/*
 * Klasa koja ucitava i snima objekte tipa Bank u mysql bazu.
 */
class BankDB {
   /*
    * Ucitaj banku iz baze.
    * @param integer $id - id banke.
    * @return Bank
    */
   static public function load($id) {
      $select = new Select();
      $select->from('bank');
      $select->condition("id = ? AND status <> 'Obrisan'", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();
      
      return self::make($row);
   }

   /*
    * Ucitaj sve banke iz baze.
    * @return array Bank
    */
   static public function loadAll() {
      $select = new Select();
      $select->from('bank');
      $select->condition("status <> 'Obrisan'");
      $result = $select->execute();

      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::make($row);
      }
      return $niz;
   }

   protected static function make($row) {
      if($row) {
         $bank = new Bank();
         $bank->setData($row);
         return $bank;
      }
      trigger_error("BankDB::make(): {$row['id']} not found.");
   }

   /*
    * Izmeni banku u bazi.
    * @param Bank.
    * @return boolean
    */
   static public function update(Bank $bank) {
      $update = new Update();
      $update->what( $bank->getFieldNames('u') );
      $update->where('bank');
      $update->condition('id = ?');
      $update->values( array_merge($bank->getFieldValues('u'), array($bank->getValue('id'))) );
      $updresult = $update->execute();
      return ($updresult > 0 ? true : false);
   }

   /*
    * Upis nove banke u bazu.
    * @param Bank.
    * @return array(insert_id, affected_rows)
    */
   static public function insert(Bank $bank) {
      $insert = new Insert();
      $insert->what( $bank->getFieldNames('i') );
      $insert->where('bank');
      $insert->values( $bank->getFieldValues('i') );
      $insresult = $insert->execute();
      return $insresult;
   }
   
   /*
    * Brisanje banke iz baze. Ne radi se fizicko brisanje,
    * vec se samo status postavlja na 'Pasivan'
    * @param integer $id - id banke.
    * @return bool
    */
   static public function delete($id) {
      $update = new Update();
      $update->what( array('status') );
      $update->where('bank');
      $update->condition('id = ?');
      $update->values( array('Pasivan', $id) );
      $updresult = $update->execute();
      return ($updresult > 0 ? true : false);
   }
}
