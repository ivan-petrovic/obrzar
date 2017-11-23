<?php

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class ZBIR {
   public static function brutoUkupno($nizPFE) {
      $ukBruto = 0;
      foreach ($nizPFE as $pfe) {
         $ukBruto += self::getBruto($pfe);
      }
      return round($ukBruto);
   }

   protected static function getBruto(PaymentForEmploye $pfe) {
      if(!$pfe->flagKon) {
         return $pfe->brutoUkupnoAk();
      } else {
         return $pfe->brutoUkupnoKon();
      }
   }
   
   public static function brZapNaIsplacenuZaradu($nizPFE, Payment $payment) {
      $cnt = 0;
      foreach ($nizPFE as $pfe) {
         $osnovica = self::getOsnovicaDoprinosa($pfe);
         if($osnovica > $payment->getValue('minOsnovicaDoprinosa') &&
            $osnovica < $payment->maxOsnovicaDoprinosa() ) {
            ++$cnt;
         }
      }
      return $cnt;
   }
   
   public static function brZapNaNajnizuOsnovicu($nizPFE, Payment $payment) {
      $cnt = 0;
      foreach ($nizPFE as $pfe) {
         $osnovica = self::getOsnovicaDoprinosa($pfe);
         if($osnovica <= $payment->getValue('minOsnovicaDoprinosa')) {
            ++$cnt;
         }
      }
      return $cnt;
   }

   public static function osnovicaUkupno($nizPFE) {
      $ukBruto = 0;
      foreach ($nizPFE as $pfe) {
         $ukBruto += self::getOsnovicaDoprinosa($pfe);
      }
      return round($ukBruto);
   }

   public static function osnovicaNaIzplacenuZaradu($nizPFE, Payment $payment) {
      $uk = 0;
      foreach ($nizPFE as $pfe) {
         $osnovica = self::getOsnovicaDoprinosa($pfe);
         if($osnovica > $payment->getValue('minOsnovicaDoprinosa') &&
            $osnovica < $payment->maxOsnovicaDoprinosa() ) {
            $uk += $osnovica;
         }
      }
      return round($uk);
   }
   
   public static function osnovicaNaMinimalnuZaradu($nizPFE, Payment $payment) {
      $uk = 0;
      foreach ($nizPFE as $pfe) {
         $osnovica = self::getOsnovicaDoprinosa($pfe);
         if($osnovica <= $payment->getValue('minOsnovicaDoprinosa')) {
            $uk += $osnovica;
         }
      }
      return round($uk);
   }

   protected static function getOsnovicaDoprinosa(PaymentForEmploye $pfe) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaDoprinos->osnovicaDoprinosa();
      } else {
         return $pfe->konacniDoprinos->osnovicaDoprinosa();
      }
   }

   public function olaksicaUkupno($nizPFE) {
      $uk = 0;
      foreach ($nizPFE as $pfe) {
         $uk += self::getOlaksica($pfe);
      }
      return $uk;
   }
   
   protected static function getOlaksica(PaymentForEmploye $pfe) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaPorez->neoporezivo();
      } else {
         return $pfe->konacniPorez->neoporezivo();
      }
   }
}
