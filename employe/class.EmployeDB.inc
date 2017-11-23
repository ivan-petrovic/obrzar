<?php

require_once 'class.Employe.inc';
require_once 'utildb/class.Select.inc';
require_once 'utildb/class.Update.inc';
require_once 'utildb/class.Insert.inc';

/*
 * Klasa koja ucitava i snima objekte tipa Employe u mysql bazu.
 */
class EmployeDB {
   /*
    * Ucitaj zaposlenog iz baze za tekucu firmu.
    * @param integer $id - id zaposlenog.
    * @return Employe
    */
   static public function load($id) {
      $select = new Select(); $select->from('employe');
      $select->condition("id = ?", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();

      return self::make($row);
   }

   protected static function make($row) {
      if($row) {
         $e = new Employe();
         $e->setData($row);
         return $e;
      }
      trigger_error("EmployeDB::make(): {$row['id']} not found.");
   }

   /*
    * Vrati ime zaposlenog sa datim id.
    * @param integer id - sifra zaposlenog
    * @return string - ime i prezime zaposlenog
    */
   static public function name($id) {
      $select = new Select(); $select->what(array('ime', 'prezime'));
      $select->from('employe');
      $select->condition("id = ?", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();

      if($row) {
         return $row['ime'] . " " . $row['prezime'];
      } else {
         trigger_error('Employe with id $id not found.');
      }
   }
   
   /*
    * Ucitaj sve zaposlene iz baze za tekucu firmu.
    * @param integer $firmID - id firme.
    * @return array Employe
    */
   static public function loadAll($firmID, $order = 'prezime') {
      $select = new Select(); $select->from('employe');
      $select->condition("firmaID = ?", array($firmID));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::make($row);
      }
      
      return self::sortiraj($niz, $order);
   }

   static public function sortiraj($niz, $order) {
      switch ($order) {
         case 'prezime':
            usort($niz, array("EmployeDB", "employeSortPrezime")); break;
         case 'ime':
            usort($niz, array("EmployeDB", "employeSortIme")); break;
         default:
            usort($niz, array("EmployeDB", "employeSortSifra")); break;
      }
      return $niz;
   }
   
   /*
    * Ucitaj sve aktivne zaposlene iz baze za tekucu firmu.
    * @param integer $firmID - id firme.
    * @return array Employe
    */
   static public function loadAllAktivni($firmID, $order = 'prezime') {
      $select = new Select(); $select->from('employe');
      $select->condition("firmaID = ? AND status = 'Aktivan'", array($firmID));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::make($row);
      }
      
      return self::sortiraj($niz, $order);
   }

   /*
    * Ucitaj sve pasivne zaposlene iz baze za tekucu firmu.
    * @param integer $firmID - id firme.
    * @return array Employe
    */
   static public function loadAllPasivni($firmID, $order = 'prezime') {
      $select = new Select(); $select->from('employe');
      $select->condition("firmaID = ? AND status = 'Pasivan'", array($firmID));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::make($row);
      }
      
      return self::sortiraj($niz, $order);
   }

   // usort() comparison function for Employe objects
   protected static function employeSortPrezime(Employe $a, Employe $b) {
      return strcmp($a->getValue('prezime'), $b->getValue('prezime'));
   }

   // usort() comparison function for Employe objects
   protected static function employeSortIme(Employe $a, Employe $b) {
      return strcmp($a->getValue('ime'), $b->getValue('ime'));
   }
   
   // usort() comparison function for Employe objects
   protected static function employeSortSifra(Employe $a, Employe $b) {
      return $a->getValue('interniID') > $b->getValue('interniID');
   }
   
   /*
    * Izmeni zaposlenog u bazi.
    * @param Employe.
    * @return boolean
    */
   static public function update(Employe $e) {
      $update = new Update();
      $update->what( $e->getFieldNames('u') );
      $update->where('employe');
      $update->condition('id = ?');
      $update->values( array_merge($e->getFieldValues('u'), array($e->getValue('id'))) );
      $updresult = $update->execute();
      return $updresult;
   }

   /*
    * Upis novog zaposlenog u bazu.
    * @param Employe.
    * @return boolean
    */
   static public function insert(Employe $e) {
      $insert = new Insert();
      $insert->what( $e->getFieldNames('i') );
      $insert->where('employe');
      $insert->values( $e->getFieldValues('i') );
      $insresult = $insert->execute();
      return $insresult;
   }
   
   /*
    * Brisanje zaposlenog iz baze. Ne radi se fizicko brisanje,
    * vec se samo status postavlja na 'Pasivan'
    * @param integer $id - id zaposlenog.
    * @return bool
    */
   static public function delete($id) {
      $update = new Update();
      $update->what( array('status') );
      $update->where('employe');
      $update->condition('id = ?');
      $update->values( array('Pasivan', $id) );
      $updresult = $update->execute();
      return ($updresult > 0 ? true : false);
   }
}
