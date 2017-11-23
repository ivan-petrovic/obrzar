<?php

require_once 'class.KarnetPageList.inc';
require_once 'class.KarnetPageDetail.inc';
require_once 'class.KarnetPageUpdate.inc';
require_once 'class.KarnetPageNew.inc';
require_once 'class.KarnetPageDelete.inc';
require_once 'class.KarnetPagePrint.inc';
require_once 'class.KarnetPageOstalo.inc';
require_once 'class.KarnetPageNewOstalo.inc';

class KarnetPageDispatcher {
   const LIST_KARNET = 7000;
   const DETAILS_KARNET = 7100;
   const NEW_KARNET = 7200;
   const UPDATE_KARNET = 7300;
   const DELETE_KARNET = 7400;
   const DETAILS_PRINT = 7500;   
   const OSTALO_KARNET = 7600;
   const NEW_OSTALO_KARNET = 7700;
   
   /*
    * Na sifre osnovu operacije izaberi podstranu
    * (novi obracun, unos, izmena, lista, pregled).
    * @param integer $op - sifra operacije
    * @param integer $fid - sifra tekuce firme
    * @param integer $pid - sifra tekuceg obracuna (izabranog obracuna)
    * @param integer $eid - sifra zaposlenog tamo gde ima smisla, 0 inace
    * @param integer $indent - pocetna indentacija sadrzaja strane
    */
   static public function choosePage($op, $fid, $pid, $eid, $indent) {
      switch ($op) {
         case KarnetPageDispatcher::LIST_KARNET:
            return new KarnetList($op, $fid, $pid, 0, $indent);
         case KarnetPageDispatcher::DETAILS_KARNET:
            return new KarnetDetails($op, $fid, $pid, $eid, $indent);
         case KarnetPageDispatcher::NEW_KARNET:
            return new KarnetNew($op, $fid, $pid, 0, $indent);
         case KarnetPageDispatcher::UPDATE_KARNET:
            return new KarnetUpdate($op, $fid, $pid, $eid, $indent);
         case KarnetPageDispatcher::DELETE_KARNET:
            return new KarnetDelete($op, $fid, $pid, $eid, $indent);
         case KarnetPageDispatcher::DETAILS_PRINT:
            return new KarnetPrint($op, $fid, $pid, $eid, $indent);
         case KarnetPageDispatcher::OSTALO_KARNET:
            return new KarnetPageOstalo($op, $fid, $pid, $eid, $indent);
         case KarnetPageDispatcher::NEW_OSTALO_KARNET:
            return new KarnetPageNewOstalo($op, $fid, $pid, $eid, $indent);
         default:
            return new KarnetList($op, $fid, $pid, 0, $indent);
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
          KarnetPageDispatcher::LIST_KARNET => "spisak.css",
          KarnetPageDispatcher::DETAILS_KARNET => "detalji.css",
          KarnetPageDispatcher::NEW_KARNET => "formstyle.css",
          KarnetPageDispatcher::UPDATE_KARNET => "formstyle.css",
          KarnetPageDispatcher::DETAILS_PRINT => "listic.css",
          KarnetPageDispatcher::OSTALO_KARNET => "formstyle.css",
          KarnetPageDispatcher::NEW_OSTALO_KARNET => "spisak.css"
      );
   }
}
