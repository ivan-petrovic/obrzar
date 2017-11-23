<?php

require_once 'class.Deduction.inc';
require_once 'utildb/class.Select.inc';
require_once 'utildb/class.Update.inc';
require_once 'utildb/class.Insert.inc';
require_once 'utildb/class.Delete.inc';

/*
 * Klasa koja ucitava i snima objekte tipa Deduction iz i u mysql bazu.
 */
class DeductionDB {
   /*
    * Ucitaj obustavu iz baze.
    * @param integer $id - id obustave.
    * @return Deduction
    */
   static public function load($id) {
      $select = new Select();
      $select->from('deduction');
      $select->condition("id = ?", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();

      return self::makeDeduction($row);
   }

   static protected function makeDeduction($row) {
      if($row) {
         $d = new Deduction();
         $d->setData($row);
         $d->rate = InstallmentDB::loadAll($d->getValue('id'));
         return $d;
      }
      trigger_error("Deduction with id $id not found.");
   }
   
   /*
    * Ucitaj sve obustave za datog zaposlenog.
    * @param integer $zaposleniID - sifra zaposlenog.
    * @return array Deduction ili prazan niz.
    */
   static public function loadAll($zaposleniID) {
      $select = new Select();
      $select->from('deduction');
      $select->condition("zaposleniID = ?", array($zaposleniID));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::makeDeduction($row);
      }
      return $niz;
   }
   
   /**
    * Ucitaj iz baze sve obustave za datog zaposlenog, za dati obracun.
    * @param int $zaposleniID - sifra zaposlenog
    * @param int $mesec - mesec obracuna
    * @param int $godina - godina obracuna
    * @param string $tipObracuna - tip obracuna (Akontacija ili Konacni)
    * @return niz Obustava ili prazan niz
    */
   static public function ucitajSveZaObracun($zaposleniID, $mesec, $godina, $tipObracuna) {
      $select = new Select(); $select->what('DISTINCT deduction.id');
      $select->from('deduction LEFT JOIN installment ON deduction.id = installment.obustavaID');
      $select->condition("zaposleniID = ? AND mesec = ? AND godina = ? AND tipObracuna = ?",
              array($zaposleniID, $mesec, $godina, $tipObracuna));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = DeductionDB::load($row['id']);
      }
      return $niz;
   }

   /**
    * Ucitaj iz baze sve obustave za datu radnju, za dati obracun.
    * @param int $shopID - sifra radnje
    * @param int $mesec - mesec obracuna
    * @param int $godina - godina obracuna
    * @param string $tipObracuna - tip obracuna (Akontacija ili Konacni)
    * @return niz Obustava ili prazan niz
    */
   static public function loadAllForShop($shopID, $mesec, $godina, $tipObracuna) {
      $select = new Select(); $select->what('DISTINCT deduction.id');
      $select->from('deduction LEFT JOIN installment ON deduction.id = installment.obustavaID');
      $select->condition("radnjaID = ? AND mesec = ? AND godina = ? AND tipObracuna = ?",
              array($shopID, $mesec, $godina, $tipObracuna));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = DeductionDB::load($row['id']);
      }
      return $niz;
   }
   
   /*
    * Izmeni obustavu u bazi.
    * @param Deduction.
    * @return boolean
    */
//   static public function update(Deduction $d) {
//      $update = new Update();
//      $update->what( array('zaposleniID', 'radnjaID') );
//      $update->where('deduction');
//      $update->condition('id = ?');
//      $update->values( array_merge(
//              array($d->getValue('zaposleniID'), $d->getValue('radnjaID')),
//              array($d->getValue('id')))
//              );
//      $updresult = $update->execute();
//      return $updresult;
//   }

   /*
    * Upis nove obustave u bazu.
    * @param Deduction.
    * @return boolean
    */
   static public function insert(Deduction $d) {
      $insert = new Insert();
      $insert->what( array('zaposleniID', 'radnjaID', 'tipObustave') );
      $insert->where('deduction');
      $insert->values( array($d->getValue('zaposleniID'), $d->getValue('radnjaID'), $d->getValue('tipObustave')) );
      $insresult = $insert->execute(); // $insresult[0] - insert_id, $insresult[1] - affected_rows
      $d->setValue('id', $insresult[0]);
      
      $tipObustave = $d->getValue('tipObustave');
      if($tipObustave === 1) {
         self::upisRata($d);
      } else {
         self::upisRata2($d);
      }
      return $insresult;
   }
   
   static protected function upisRata(Deduction $d) {
      $iznosRate = floor($d->getValue('ukIznos') / $d->getValue('brrata'));
      $prvihNMinus1Rata = 0;
      $nizRata = array();
      for($i = 1; $i < $d->getValue('brrata'); ++$i) {
         $prvihNMinus1Rata += $iznosRate;
         $nizPomeraj = self::pomeriRatu($d, $i - 1, $d->getValue('tipPlacanja'));
         $rata = new Installment();
         $rata->setData(array(
            'obustavaID' => $d->getValue('id'),
            'iznos' => $iznosRate,
            'mesec' => $nizPomeraj[0],
            'godina' => $nizPomeraj[1],
            'tipObracuna' => $nizPomeraj[2],
         ));
         $nizRata[] = $rata;
      }
      
      $nizPomeraj = self::pomeriRatu($d, $d->getValue('brrata') - 1, $d->getValue('tipPlacanja'));
      $preostalaRata = $d->getValue('ukIznos') - $prvihNMinus1Rata;
      $rata = new Installment();
      $rata->setData(array(
         'obustavaID' => $d->getValue('id'),
         'iznos' => $preostalaRata,
         'mesec' => $nizPomeraj[0],
         'godina' => $nizPomeraj[1],
         'tipObracuna' => $nizPomeraj[2],
      ));
      $nizRata[] = $rata;

      foreach( $nizRata as $rata ) {
         InstallmentDB::insert($rata);
      }
   }

   static protected function upisRata2(Deduction $d) {
      $iznosRate = $d->getValue('ukIznos');

      $nizRata = array();
      for($i = 1; $i <= $d->getValue('brrata'); ++$i) {
         $nizPomeraj = self::pomeriRatu($d, $i - 1, $d->getValue('tipPlacanja'));
         $rata = new Installment();
         $rata->setData(array(
            'obustavaID' => $d->getValue('id'),
            'iznos' => $iznosRate,
            'mesec' => $nizPomeraj[0],
            'godina' => $nizPomeraj[1],
            'tipObracuna' => $nizPomeraj[2],
         ));
         $nizRata[] = $rata;
      }
      
      foreach( $nizRata as $rata ) {
         InstallmentDB::insert($rata);
      }
   }

   /**
    * Pomera ratu za odredjeni broj isplata.
    * Npr. ako obustava pocinje 2.2013. na akontaciji,
    * i isplacuje se pri svakoj isplati, pomeraj za 3
    * daje kao rezultat 3.2013. konacni (racunam dve isplate mesecno).
    * @param Obustava $obustava - obustava koja definise prvu isplatu kada ide rata.
    * @param int n - broj isplata koji se pomera.
    * @param int tipPlacanja - svaka isplata (1), samo na ak (2), samo na kon (3)
    * @return array sa tri elementa: mesec, godina i tip obracuna
    */
   static protected function pomeriRatu(Deduction $obustava, $n, $tipPlacanja) {
      // $mesec = $obustava->getValue('mesec');
      $godina = $obustava->getValue('godina');
      $tip = $obustava->getValue('tipObracuna');

      $rezmesec = $obustava->getValue('mesec');
      $rezgodina = $obustava->getValue('godina');
      $reztip = $obustava->getValue('tipObracuna');

      switch ($tipPlacanja) {
         case 1:
            // pri svakoj isplati
            if($n % 2 === 0) {
               // paran pomeraj
               $reztip = $tip;
               $n /= 2;
               $rezmesec += $n;
               if($rezmesec > 12) {
                  while($rezmesec > 12) {
                     ++$rezgodina;
                     $rezmesec -= 12;
                  }
               } else {
                  $rezgodina = $godina;
               }
            } else {
               // neparan pomeraj
               if($tip === "Akontacija"){
                  $reztip = "Konacni";
                  $n = ($n - 1) / 2;
               } else {
                  $n = ($n + 1) / 2;
                  $reztip = "Akontacija";
               }
               $rezmesec += $n;
               if($rezmesec > 12) {
                  while($rezmesec > 12) {
                     ++$rezgodina;
                     $rezmesec -= 12;
                  }
               } else {
                  $rezgodina = $godina;
               }
            }
         break;
         case 2:
            // samo na akontaciji
            $reztip = "Akontacija";
            $rezmesec += $n;
            if($rezmesec > 12) {
               while($rezmesec > 12) {
                  ++$rezgodina;
                  $rezmesec -= 12;
               }
            } else {
               $rezgodina = $godina;
            }
         break;
         case 3:
            // samo na konacnom
            $reztip = "Konacni";
            $rezmesec += $n;
            if($rezmesec > 12) {
               while($rezmesec > 12) {
                  ++$rezgodina;
                  $rezmesec -= 12;
               }
            } else {
               $rezgodina = $godina;
            }
         break;
      }
      return array($rezmesec, $rezgodina, $reztip);
   }
   
   /*
    * Brisanje obustave iz baze. Brisu se sve rate iz baze rata,
    * kao i obustava iz baze obustava.
    * @param integer $id - id obustave.
    * @return bool
    */
   static public function delete($id) {
      InstallmentDB::delete($id);   // obrisi sve rate;
      
      $delete = new Delete();
      $delete->where('deduction');
      $delete->condition('id = ?');
      $delete->values( array($id) );
      $delresult = $delete->execute();
      return ($delresult > 0 ? true : false);
   }
}

class InstallmentDB {
   /*
    * Ucitaj ratu iz baze.
    * @param integer $id - id rate.
    * @return Installment
    */
   static public function load($id) {
      $select = new Select(); $select->from('installment');
      $select->condition("id = ?", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();

      return self::make($row);
   }

   static protected function make($row) {
      if($row) {
         $rata = new Installment();
         $rata->setData($row);
         return $rata;
      }
      trigger_error("Installment with id $id not found.");
   }

   /*
    * Ucitaj sve rate iz baze za datu obustavu.
    * @param integer $obustavaID - id obustave na koju se rate odnose.
    * @return array Installment
    */
   static public function loadAll($obustavaID) {
      $select = new Select(); $select->from('installment');
      $select->condition("obustavaID = ?", array($obustavaID));
      $result = $select->execute();
      
      $niz = array();
      while($row = $result->fetch_assoc()) {
         $niz[] = self::make($row);
      }
      return $niz;
   }

   /*
    * Upis nove rate u bazu.
    * @param Rata.
    * @return array(insert_id, affected_rows)
    */
   static public function insert(Installment $rata) {
      $insert = new Insert();
      $insert->what( $rata->getFieldNames('i') );
      $insert->where('installment');
      $insert->values( $rata->getFieldValues('i') );
      $insresult = $insert->execute();
      return $insresult;
   }

   /*
    * Izmeni ratu u bazi.
    * @param Rata.
    * @return boolean
    */
   static public function update(Installment $rata) {
      $update = new Update();
      $update->what( $rata->getFieldNames('u') );
      $update->where('installment');
      $update->condition('id = ?');
      $update->values( array_merge($rata->getFieldValues('u'), array($rata->getValue('id'))) );
      $updresult = $update->execute();
      return ($updresult > 0 ? true : false);
   }
   
   /*
    * Brisanje svih rata za obustavu sa sifrom id.
    * Rate se fizicki brisu iz baze.
    * @param integer $id - id obustave.
    * @return bool
    */
   static public function delete($id) {
      $delete = new Delete();
      $delete->where('installment');
      $delete->condition('obustavaID = ?');
      $delete->values( array($id) );
      $delresult = $delete->execute();
      return ($delresult > 0 ? true : false);
   }
}
