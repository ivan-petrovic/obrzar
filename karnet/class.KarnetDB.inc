<?php

require_once 'class.Karnet.inc';
require_once 'utildb/class.Select.inc';
require_once 'utildb/class.Update.inc';
require_once 'utildb/class.Insert.inc';
require_once 'utildb/class.Delete.inc';

/*
 * Klasa koja ucitava i snima objekte tipa Karnet u mysql bazu.
 */
class KarnetDB {
   /*
    * Ucitaj karnet iz baze za tekuci obracun (u tekucoj firmi)
    * i trazenog zaposlenog.
    * @param integer $zaposleniID - id zaposlenog.
    * @param integer $obracunID - id obracuna.
    * @return Karnet
    */
   static public function load($zaposleniID, $obracunID) {
      $select = new Select();
      $select->from('karnet');
      $select->condition("zaposleniID = ? AND obracunID = ?", array($zaposleniID, $obracunID));
      $result = $select->execute();
      $row = $result->fetch_assoc();
      // echo "$zaposleniID-$obracunID";
      return self::make($row);
   }

   static public function exist($zaposleniID, $obracunID) {
      $select = new Select();
      $select->from('karnet');
      $select->condition("zaposleniID = ? AND obracunID = ?", array($zaposleniID, $obracunID));
      $result = $select->execute();
      $row = $result->fetch_assoc();

      if($row) {
         return true;
      }
      return false;
   }

   static public function load1($zaposleniID, $obracunID) {
      $select = new Select();
      $select->from('karnet');
      $select->condition("zaposleniID = ? AND obracunID = ?", array($zaposleniID, $obracunID));
      $result = $select->execute();
      $row = $result->fetch_assoc();

      if($row) {
         $karnet = new Karnet(); $karnet->setData($row);
         return $karnet;
      }
      return false;
   }
   
   protected static function make($row) {
      if($row) {
         $karnet = new Karnet();
         $karnet->setData($row);
         return $karnet;
      }
      trigger_error("KarnetDB::make(): employe and/or payment not found.");
   }

   /*
    * Ucitaj sve karnete iz baze za tekuci obracuna (u tekucoj firmi).
    * @param integer $obracunID - id obracuna.
    * @return array Karnet
    */
   static public function loadAll($obracunID) {
      $select = new Select();
      $select->from('karnet');
      $select->condition("obracunID = ?", array($obracunID));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::make($row);
      }
      usort($niz, array("KarnetDB", "karnetSortPrezime"));
      return $niz;
   }

   // usort() comparison function for Karnet objects
   protected static function karnetSortPrezime(Karnet $a, Karnet $b) {
      $empA = EmployeDB::load($a->getValue('zaposleniID'));
      $empB = EmployeDB::load($b->getValue('zaposleniID'));
      return strcmp($empA->getValue('prezime'), $empB->getValue('prezime'));
   }

   /*
    * Izmeni karnet u bazi.
    * @param Karnet.
    * @return boolean
    */
   static public function update(Karnet $karnet) {
      $update = new Update();
      $update->what( $karnet->getFieldNames('u') );
      $update->where('karnet');
      $update->condition('zaposleniID = ? AND obracunID = ?');
      $update->values( array_merge($karnet->getFieldValues('u'),
              array($karnet->getValue('zaposleniID'), $karnet->getValue('obracunID'))) );
      $updresult = $update->execute();
      return $updresult;
   }

   static public function updateCena($p1pid, $p2tipCenaRada, $p3cenaRada) {
      $update = new Update();
      $update->what( array('tipCenaRada','cenaRada') );
      $update->where('karnet');
      $update->condition('obracunID = ?');
      $update->values( array($p2tipCenaRada, $p3cenaRada, $p1pid) );
      $updresult = $update->execute();

      return $updresult;
   }
   
   /*
    * Upis novog karneta u bazu.
    * @param Karnet.
    * @return boolean
    */
   static public function insert(Karnet $karnet) {
      $insert = new Insert();
      $insert->what( $karnet->getFieldNames('i') );
      $insert->where('karnet');
      $insert->values( $karnet->getFieldValues('i') );
      $insresult = $insert->execute();
      return $insresult;
   }

   /*
    * Brisanje karneta iz baze. Radi se fizicko brisanje.
    * @param integer $zaposleniID - id zaposlenog.
    * @param integer $obracunID - id obracuna.
    * @return bool
    */
   static public function delete($zaposleniID, $obracunID) {
      $delete = new Delete();
      $delete->where('karnet');
      $delete->condition("zaposleniID = ? AND obracunID = ?");
      $delete->values( array($zaposleniID, $obracunID) );
      $delresult = $delete->execute();
      return ($delresult > 0 ? true : false);
   }
}
