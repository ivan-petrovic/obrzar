<?php

require_once 'class.BankPageList.inc';
require_once 'class.BankPageDetail.inc';
require_once 'class.BankPageUpdate.inc';
require_once 'class.BankPageNew.inc';
require_once 'class.BankPageDelete.inc';

class BankPageDispatcher {
   const LIST_BANK = 3000;
   const DETAILS_BANK = 3100;
   const NEW_BANK = 3200;
   const UPDATE_BANK = 3300;
   const DELETE_BANK = 3400;

   /*
    * Na sifre osnovu operacije izaberi podstranu
    * (nova banka, unos, izmena, lista, pregled).
    * @param integer $op - sifra operacije
    * @param integer $bid - sifra banke tamo gde ima smisla, 0 inace
    * @param integer $indent - pocetna indentacija sadrzaja strane
    */
   static public function choosePage($op, $bid, $indent) {
      switch ($op) {
         case BankPageDispatcher::LIST_BANK:
            return new BankList($op, 0, $indent);
         case BankPageDispatcher::DETAILS_BANK:
            return new BankDetails($op, $bid, $indent);
         case BankPageDispatcher::NEW_BANK:
            return new BankNew($op, 0, $indent);
         case BankPageDispatcher::UPDATE_BANK:
            return new BankUpdate($op, $bid, $indent);
         case BankPageDispatcher::DELETE_BANK:
            return new BankDelete($op, $bid, $indent);
         default:
            return new BankList($op, 0, $indent);
      }
   }

   /*
    * Na sifre osnovu operacije bira CSS fajl koji treba da ucita zaglavlje strane.
    * @param integer $op - sifra operacije
    */
   static public function getCSSFileName($op) {
      $cssFiles = self::CSSFileNamesMap();
      if(array_key_exists($op, $cssFiles)) {
         return $cssFiles[$op];
      }
      return "spisak.css";
   }
   
   /*
    * Funkcija vraca niz koji mapira sifru operacije na naziv css fajla
    * pridruzenog toj operaciji.
    */
   static protected function CSSFileNamesMap() {
      return array(
          BankPageDispatcher::LIST_BANK => "spisak.css",
          BankPageDispatcher::DETAILS_BANK => "detalji.css",
          BankPageDispatcher::NEW_BANK => "formstyle.css",
          BankPageDispatcher::UPDATE_BANK => "formstyle.css",
      );
   }
}
