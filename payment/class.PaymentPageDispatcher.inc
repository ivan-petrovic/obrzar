<?php

require_once 'class.PaymentPageList.inc';
require_once 'class.PaymentPageDetail.inc';
require_once 'class.PaymentPageUpdate.inc';
require_once 'class.PaymentPageNew.inc';
require_once 'class.PaymentPageDelete.inc';
require_once 'class.PaymentPageUpdKarnet.inc';

class PaymentPageDispatcher {
   const LIST_PAYMENT = 6000;
   const DETAILS_PAYMENT = 6100;
   const NEW_PAYMENT = 6200;
   const UPDATE_PAYMENT = 6300;
   const DELETE_PAYMENT = 6400;
   const UPDATE_KARNET_PAYMENT = 6500;

   /*
    * Na sifre osnovu operacije izaberi podstranu
    * (novi obracun, unos, izmena, lista, pregled).
    * @param integer $op - sifra operacije
    * @param integer $fid - sifra tekuce firme
    * @param integer $pid - sifra obracuna tamo gde ima smisla, 0 inace
    * @param integer $indent - pocetna indentacija sadrzaja strane
    */
   static public function choosePage($op, $fid, $pid, $indent) {
      switch ($op) {
         case PaymentPageDispatcher::LIST_PAYMENT:
            return new PaymentList($op, $fid, 0, $indent);
         case PaymentPageDispatcher::DETAILS_PAYMENT:
            return new PaymentDetails($op, $fid, $pid, $indent);
         case PaymentPageDispatcher::NEW_PAYMENT:
            return new PaymentNew($op, $fid, 0, $indent);
         case PaymentPageDispatcher::UPDATE_PAYMENT:
            return new PaymentUpdate($op, $fid, $pid, $indent);
         case PaymentPageDispatcher::DELETE_PAYMENT:
            return new PaymentDelete($op, $fid, $pid, $indent);
         case PaymentPageDispatcher::UPDATE_KARNET_PAYMENT:
            return new PaymentUpdateKarnet($op, $fid, $pid, $indent);
         default:
            return new PaymentList($op, $fid, 0, $indent);
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
          PaymentPageDispatcher::LIST_PAYMENT => "spisak.css",
          PaymentPageDispatcher::DETAILS_PAYMENT => "obracundetalji.css",
          PaymentPageDispatcher::NEW_PAYMENT => "formstyle.css",
          PaymentPageDispatcher::UPDATE_PAYMENT => "formstyle.css",
      );
   }
}
