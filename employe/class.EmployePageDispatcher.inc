<?php

require_once 'class.EmployePageList.inc';
require_once 'class.EmployePageDetail.inc';
require_once 'class.EmployePageUpdate.inc';
require_once 'class.EmployePageNew.inc';
require_once 'class.EmployePageDelete.inc';

class EmployePageDispatcher {
   const LIST_EMPLOYE = 2000;
   const DETAILS_EMPLOYE = 2100;
   const NEW_EMPLOYE = 2200;
   const UPDATE_EMPLOYE = 2300;
   const DELETE_EMPLOYE = 2400;

   /*
    * Na sifre osnovu operacije izaberi podstranu
    * (novi zaposleni, unos, izmena, lista, pregled).
    * @param integer $op - sifra operacije
    * @param integer $fid - sifra tekuce firme
    * @param integer $eid - sifra zaposlenog tamo gde ima smisla, 0 inace
    * @param integer $indent - pocetna indentacija sadrzaja strane
    */
   static public function choosePage($op, $fid, $eid, $indent) {
      switch ($op) {
         case EmployePageDispatcher::LIST_EMPLOYE:
            return new EmployeList($op, $fid, 0, $indent);
         case EmployePageDispatcher::DETAILS_EMPLOYE:
            return new EmployeDetails($op, $fid, $eid, $indent);
         case EmployePageDispatcher::NEW_EMPLOYE:
            return new EmployeNew($op, $fid, 0, $indent);
         case EmployePageDispatcher::UPDATE_EMPLOYE:
            return new EmployeUpdate($op, $fid, $eid, $indent);
         case EmployePageDispatcher::DELETE_EMPLOYE:
            return new EmployeDelete($op, $fid, $eid, $indent);
         default:
            return new EmployeList($op, $fid, 0, $indent);
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
          EmployePageDispatcher::LIST_EMPLOYE => "spisak.css",
          EmployePageDispatcher::DETAILS_EMPLOYE => "detalji.css",
          EmployePageDispatcher::NEW_EMPLOYE => "formstyle.css",
          EmployePageDispatcher::UPDATE_EMPLOYE => "formstyle.css",
      );
   }
}
