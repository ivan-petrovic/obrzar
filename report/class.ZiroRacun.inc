<?php

class ZiroRacun {
   const ZR_POREZ = 0;
   const ZR_PIO_ZAP = 1;
   const ZR_ZDR_ZAP = 2;
   const ZR_NEZ_ZAP = 3;
   const ZR_PIO_POSL = 4;
   const ZR_ZDR_POSL = 5;
   const ZR_NEZ_POSL = 6;

   // ove konstante moraju da budu usaglasene sa konstantama za tip firme
   const PL_NE_BUDZET = 1;   // Pravno lice koje se ne finansira iz budzeta
   const PREDUZETNIK = 2;
   const PL_DA_BUDZET = 3;   // Pravno lice koje se finansira iz budzeta
   const FIZICKO_LICE = 4;
   const STRANO_PREDSTAVNISTVO = 5;
   
   public static function getZR($tipFirme, $tipZR) {
      $map = array(
          self::PL_NE_BUDZET => array( self::ZR_POREZ => '840-711111843-52',
                                       self::ZR_PIO_ZAP => '840-721111843-18',
                                       self::ZR_ZDR_ZAP => '840-721121843-88',
                                       self::ZR_NEZ_ZAP => '840-721131843-61',
                                       self::ZR_PIO_POSL => '840-721212843-46',
                                       self::ZR_ZDR_POSL => '840-721222843-19',
                                       self::ZR_NEZ_POSL => '840-721232843-89'),
          self::PREDUZETNIK => array( self::ZR_POREZ => '840-711111843-52',
                                       self::ZR_PIO_ZAP => '840-721113843-32',
                                       self::ZR_ZDR_ZAP => '840-721121843-88',
                                       self::ZR_NEZ_ZAP => '840-721131843-61',
                                       self::ZR_PIO_POSL => '840-721214843-60',
                                       self::ZR_ZDR_POSL => '840-721222843-19',
                                       self::ZR_NEZ_POSL => '840-721232843-89'),
          self::PL_DA_BUDZET => array( self::ZR_POREZ => '840-711111843-52',
                                       self::ZR_PIO_ZAP => '840-721111843-18',
                                       self::ZR_ZDR_ZAP => '840-721121843-88',
                                       self::ZR_NEZ_ZAP => '840-721131843-61',
                                       self::ZR_PIO_POSL => '840-721211843-39',
                                       self::ZR_ZDR_POSL => '840-721221843-12',
                                       self::ZR_NEZ_POSL => '840-721231843-82'),
          self::FIZICKO_LICE => array( self::ZR_POREZ => '840-711111843-52',
                                       self::ZR_PIO_ZAP => '840-721112843-25',
                                       self::ZR_ZDR_ZAP => '840-721121843-88',
                                       self::ZR_NEZ_ZAP => '840-721131843-61',
                                       self::ZR_PIO_POSL => '840-721213843-53',
                                       self::ZR_ZDR_POSL => '840-721222843-19',
                                       self::ZR_NEZ_POSL => '840-721232843-89'),
          self::STRANO_PREDSTAVNISTVO => array( self::ZR_POREZ => '840-711111843-52',
                                       self::ZR_PIO_ZAP => '840-721112843-25',
                                       self::ZR_ZDR_ZAP => '840-721121843-88',
                                       self::ZR_NEZ_ZAP => '840-721131843-61',
                                       self::ZR_PIO_POSL => '840-721213843-53',
                                       self::ZR_ZDR_POSL => '840-721222843-19',
                                       self::ZR_NEZ_POSL => '840-721232843-89')
      );
      
      // echo $tipFirme; exit();
      return $map[$tipFirme][$tipZR];
   }
}
