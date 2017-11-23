<?php

require_once 'class.Payment.inc';
require_once 'utildb/class.Select.inc';
require_once 'utildb/class.Update.inc';
require_once 'utildb/class.Insert.inc';

/*
 * Klasa koja ucitava i snima objekte tipa Payment u mysql bazu.
 */
class PaymentDB {
   /*
    * Ucitaj obracuna iz baze za tekucu firmu.
    * @param integer $id - id obracuna.
    * @return Payment
    */
   static public function load($id) {
      $select = new Select();
      $select->from('payment');
      $select->condition("id = ?", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();

      return self::make($row);
   }

   protected static function make($row) {
      if($row) {
         $payment = new Payment();
         $payment->setData($row);
         return $payment;
      }
      trigger_error("PaymentDB::make(): {$row['id']} not found.");
   }

   /*
    * Ucitaj obracuna iz baze za tekucu firmu.
    * @param integer $mesec - mesec obracuna.
    * @param integer $godina - godina obracuna.
    * @param integer $tip - tip obracuna.
    * @return Payment
    */
   static public function load1($mesec, $godina, $tip) {
      $select = new Select();
      $select->from('payment');
      $select->condition("firmaID = ? AND mesec = ? AND godina = ? AND tipObracuna = ?",
              array(Application::getCurrFirmID(), $mesec, $godina, $tip));
      $result = $select->execute(); $row = $result->fetch_assoc();

      if($row) {
         $payment = new Payment(); $payment->setData($row);
         return $payment;
      }
      return false;
   }
   
   /*
    * Ucitaj zaposlenog iz baze za tekucu firmu.
    * @param integer $id - id zaposlenog.
    * @param integer $firmID - id firme.
    * @return Payment
    */
//   static public function load($id, $firmID) {
//      $select = new Select(); $select->from('payment');
//      $select->condition("id = ? AND firmaID = ?", array($id, $firmID));
//      $result = $select->execute();
//      $row = $result->fetch_assoc();
//      // var_dump($row);
//      if($row) {
//         $payment = new Payment(); $payment->setData($row);
//         return $payment;
//      } else {
//         trigger_error('Payment with id $id not found.');
//      }
//   }

   /*
    * Vrati naziv obracuna sa datim id.
    * @param integer id - sifra obracuna
    * @return string - naziv obracuna
    */
   static public function name($id) {
      $payment = self::load($id);
      return $payment->getName();
   }
   
   /*
    * Ucitaj sve obracuna iz baze za tekucu firmu.
    * @param integer $firmID - id firme.
    * @return array Payment
    */
   static public function loadAll($firmID) {
      $select = new Select();
      $select->from('payment');
      $select->condition("firmaID = ? ORDER BY godina DESC, mesec DESC", array($firmID));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::make($row);
      }
      return $niz;
   }

   /*
    * Ucitaj sve obracune iz baze za u opsegu zadatih meseci, za tekucu firmu.
    * @param integer $firmID - id firme.
    * @param integer $odMesec - pocetni mesec.
    * @param integer $odGodina - pocetna godina.
    * @param integer $doMesec - zavrsni mesec.
    * @param integer $doGodina - zavrsna godina.
    * @return array Payment
    */
   static public function loadKarnetZarade($firmID, $odMesec, $odGodina, $doMesec, $doGodina) {
      if($odGodina <> $doGodina) {
         $doMesec1 = 12;
         $odMesec1 = 1;
      } else {
         $doMesec1 = $doMesec;
         $odMesec1 = $odMesec;
      }
      $select = new Select();
      $select->from('payment');
      $select->condition("firmaID = ? AND ((godina = ? AND mesec >= ? AND mesec <= ?) OR (godina = ? AND mesec >= ? AND mesec <= ?)) AND tipObracuna <> 'Akontacija' ORDER BY godina DESC, mesec DESC",
              array($firmID, $odGodina, $odMesec, $doMesec1, $doGodina, $odMesec1, $doMesec));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::make($row);
      }
      return $niz;
   }

   /*
    * Ucitava za zadatu firmu poslednji unet obracun
    * @param integer $firmID - sifra firme.
    */
   static public function getLastPayment($firmID) {
      $select = new Select();
      $select->from('payment');
      $select->condition("firmaID = ? ORDER BY godina DESC, mesec DESC LIMIT 1", array($firmID));
      $result = $select->execute(); $row = $result->fetch_assoc();

      if($row) {
         $payment = new Payment();
         $payment->setData($row);
         return $payment;
      }
      return false;
   }
   
   /*
    * Izmeni obracun u bazi.
    * @param Payment.
    * @return boolean
    */
   static public function update(Payment $payment) {
      $update = new Update();
      $update->what( $payment->getFieldNames('u') );
      $update->where('payment');
      $update->condition('id = ?');
      $update->values( array_merge($payment->getFieldValues('u'), array($payment->getValue('id'))) );
      $updresult = $update->execute();
      return $updresult;
   }

   /*
    * Upis novog obracuna u bazu.
    * @param Payment.
    * @return boolean
    */
   static public function insert(Payment $payment) {
      $insert = new Insert();
      $insert->what( $payment->getFieldNames('i') );
      $insert->where('payment');
      $insert->values( $payment->getFieldValues('i') );
      $insresult = $insert->execute();
      return $insresult;
   }
}
