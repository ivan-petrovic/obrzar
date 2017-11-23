<?php

require_once 'class.DeductionPageList.inc';
require_once 'class.DeductionPageDetail.inc';
require_once 'class.DeductionPageUpdate.inc';
require_once 'class.DeductionPageNew.inc';
require_once 'class.DeductionPageDelete.inc';

class DeductionPageDispatcher {
   const LIST_DEDUCTION = 4000;
   const DETAILS_DEDUCTION = 4100;
   const NEW_DEDUCTION = 4200;
   const UPDATE_DEDUCTION = 4300;
   const DELETE_DEDUCTION = 4400;

   /*
    * Na sifre osnovu operacije izaberi podstranu
    * (nova obustava, unos, izmena, lista, pregled).
    * @param integer $op - sifra operacije
    * @param integer $did - sifra obustave tamo gde ima smisla, 0 inace
    * @param integer $indent - pocetna indentacija sadrzaja strane
    */
   static public function choosePage($op, $did, $indent) {
      switch ($op) {
         case DeductionPageDispatcher::LIST_DEDUCTION:
            return new DeductionList($op, 0, $indent);
         case DeductionPageDispatcher::DETAILS_DEDUCTION:
            return new DeductionDetails($op, $did, $indent);
         case DeductionPageDispatcher::NEW_DEDUCTION:
            return new DeductionNew($op, 0, $indent);
         case DeductionPageDispatcher::UPDATE_DEDUCTION:
            return new DeductionUpdate($op, $did, $indent);
         case DeductionPageDispatcher::DELETE_DEDUCTION:
            return new DeductionDelete($op, $did, $indent);
         default:
            return new DeductionList($op, 0, $indent);
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
          DeductionPageDispatcher::LIST_DEDUCTION => "spisak.css",
          DeductionPageDispatcher::DETAILS_DEDUCTION => "detalji.css",
          DeductionPageDispatcher::NEW_DEDUCTION => "formstyle.css",
          DeductionPageDispatcher::UPDATE_DEDUCTION => "formstyle.css",
      );
   }
}
