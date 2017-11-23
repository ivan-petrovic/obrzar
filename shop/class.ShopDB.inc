<?php

require_once 'class.Shop.inc';
require_once 'utildb/class.Select.inc';
require_once 'utildb/class.Update.inc';
require_once 'utildb/class.Insert.inc';

/*
 * Klasa koja ucitava i snima objekte tipa Shop u mysql bazu.
 */
class ShopDB {
   /*
    * Ucitaj radnju iz baze.
    * @param integer $id - id radnje.
    * @return Shop
    */
   static public function load($id) {
      $select = new Select();
      $select->from('shop');
      $select->condition("id = ? AND status <> 'Obrisan'", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();
      
      return self::make($row);
   }

   /*
    * Ucitaj sve radnje iz baze.
    * @return array Shop
    */
   static public function loadAll() {
      $select = new Select();
      $select->from('shop');
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
         $shop = new Shop();
         $shop->setData($row);
         return $shop;
      }
      trigger_error("ShopDB::make(): {$row['id']} not found.");
   }

   /*
    * Izmeni radnju u bazi.
    * @param Shop.
    * @return boolean
    */
   static public function update(Shop $shop) {
      $update = new Update();
      $update->what( $shop->getFieldNames('u') );
      $update->where('shop');
      $update->condition('id = ?');
      $update->values( array_merge($shop->getFieldValues('u'), array($shop->getValue('id'))) );
      $updresult = $update->execute();
      return ($updresult > 0 ? true : false);
   }

   /*
    * Upis nove radnje u bazu.
    * @param Shop.
    * @return array(insert_id, affected_rows)
    */
   static public function insert(Shop $shop) {
      $insert = new Insert();
      $insert->what( $shop->getFieldNames('i') );
      $insert->where('shop');
      $insert->values( $shop->getFieldValues('i') );
      $insresult = $insert->execute();
      return $insresult;
   }

   /*
    * Brisanje radnje iz baze. Ne radi se fizicko brisanje,
    * vec se samo status postavlja na 'Pasivan'
    * @param integer $id - id radnje.
    * @return bool
    */
   static public function delete($id) {
      $update = new Update();
      $update->what( array('status') );
      $update->where('shop');
      $update->condition('id = ?');
      $update->values( array('Pasivan', $id) );
      $updresult = $update->execute();
      return ($updresult > 0 ? true : false);
   }
}
