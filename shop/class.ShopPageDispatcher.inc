<?php

require_once 'class.ShopPageList.inc';
require_once 'class.ShopPageDetail.inc';
require_once 'class.ShopPageUpdate.inc';
require_once 'class.ShopPageNew.inc';
require_once 'class.ShopPageDelete.inc';

class ShopPageDispatcher {
   const LIST_SHOP = 5000;
   const DETAILS_SHOP = 5100;
   const NEW_SHOP = 5200;
   const UPDATE_SHOP = 5300;
   const DELETE_SHOP = 5400;

   /*
    * Na sifre osnovu operacije izaberi podstranu
    * (nova radnja, unos, izmena, lista, pregled).
    * @param integer $op - sifra operacije
    * @param integer $sid - sifra radnje tamo gde ima smisla, 0 inace
    * @param integer $indent - pocetna indentacija sadrzaja strane
    */
   static public function choosePage($op, $sid, $indent) {
      switch ($op) {
         case ShopPageDispatcher::LIST_SHOP:
            return new ShopList($op, 0, $indent);
         case ShopPageDispatcher::DETAILS_SHOP:
            return new ShopDetails($op, $sid, $indent);
         case ShopPageDispatcher::NEW_SHOP:
            return new ShopNew($op, 0, $indent);
         case ShopPageDispatcher::UPDATE_SHOP:
            return new ShopUpdate($op, $sid, $indent);
         case ShopPageDispatcher::DELETE_SHOP:
            return new ShopDelete($op, $sid, $indent);
         default:
            return new ShopList($op, 0, $indent);
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
          ShopPageDispatcher::LIST_SHOP => "spisak.css",
          ShopPageDispatcher::DETAILS_SHOP => "detalji.css",
          ShopPageDispatcher::NEW_SHOP => "formstyle.css",
          ShopPageDispatcher::UPDATE_SHOP => "formstyle.css",
      );
   }
}
