<?php

require_once 'class.FirmPageList.inc';
require_once 'class.FirmPageDetail.inc';
require_once 'class.FirmPageUpdate.inc';
require_once 'class.FirmPageNew.inc';
require_once 'class.FirmPageDelete.inc';

class FirmPageDispatcher {
   const LIST_FIRM = 1000;
   const DETAILS_FIRM = 1100;
   const NEW_FIRM = 1200;
   const UPDATE_FIRM = 1300;
   const DELETE_FIRM = 1400;

   /*
    * Na sifre osnovu operacije izaberi podstranu
    * (nova firma, unos, izmena, lista, pregled).
    * @param integer $op - sifra operacije
    * @param integer $fid - sifra firme tamo gde ima smisla, 0 inace
    * @param integer $indent - pocetna indentacija sadrzaja strane
    */
   static public function choosePage($op, $fid, $indent) {
      switch ($op) {
         case FirmPageDispatcher::LIST_FIRM:
            return new FirmList($op, 0, $indent);
         case FirmPageDispatcher::DETAILS_FIRM:
            return new FirmDetails($op, $fid, $indent);
         case FirmPageDispatcher::NEW_FIRM:
            return new FirmNew($op, 0, $indent);
         case FirmPageDispatcher::UPDATE_FIRM:
            return new FirmUpdate($op, $fid, $indent);
         case FirmPageDispatcher::DELETE_FIRM:
            return new FirmDelete($op, $fid, $indent);
         default:
            return new FirmList($op, 0, $indent);
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
          FirmPageDispatcher::LIST_FIRM => "spisak.css",
          FirmPageDispatcher::DETAILS_FIRM => "detalji.css",
          FirmPageDispatcher::NEW_FIRM => "formstyle.css",
          FirmPageDispatcher::UPDATE_FIRM => "formstyle.css",
      );
   }
}
