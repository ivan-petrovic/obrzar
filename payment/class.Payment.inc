<?php

require_once 'util/class.Data.inc';

/*
 * Klasa definise strukturu entiteta koji opisuje opste podatke o Obracunu.
 */
class PaymentStructure extends Structure {
//   id - sifra obracuna
//   mesec - mesec obracuna
//   godina - godina obracuna
//   tipObracuna - tip obracuna: 'Akontacija' ili 'Konacni'
//   firmaID - firma na koju se obracun odnosi
//   (jer firme mogu razlicito da definisu neke koeficijente)
//   datumIsplate - datum isplate
//   tipCenaRada - bruto (1) ili neto (2)
//   cenaRada - cena rada
//   satiUMesecu - broj sati u mesecu
//   koefRadPraznik - koeficijent za rad na drzavni praznik
//   koefRadNoc - koeficijent za rad nocu
//   koefRadPrekovremeno - koeficijent za prekovremeni rad
//   koefRadSmena - koeficijent za rad u smeni
//   koefRadNedelja - koeficijent za rad nedeljom
//   iznosTOPoDanu - iznos za topli obrok po danu
//   iznosRegres - iznos regres
//   iznosPoklonDeci - iznos poklon deci
//   koefBolDo30D - koeficijent za bolovanje do 30 dana
//   koefPrekid - koeficijent za prekid rada (bez krivice zaposlenog)
//   stopaPoreza - stopa poreza
//   iznosNeoporezivo - neoporezivi iznos
//   stopaPIO - stopa doprinosa za PIO na teret zaposlenog
//   stopaZdravstvo - stopa doprinosa za zdravsto na teret zaposlenog
//   stopaNezaposlenost - stopa doprinosa za nezaposlenost na teret zaposlenog
//   stopaPIOPoslodavac - stopa doprinosa za PIO na teret poslodavca
//   stopaZdravstvoPoslodavac - stopa doprinosa za zdravstvo na teret poslodavca
//   stopaNezaposlenostPoslodavac - stopa doprinosa za nezaposlenost na teret poslodavca
//   minOsnovicaDoprinosa - minimalna stopa doprinosa
//   iznosBrutoZarada - prosecna bruto zarada u republici
//   iznosMinZarada - minimalna zarada po satu u republici
   
   /*
    * Definise abstraktni metod roditeljske klase i vraca opis strukture.
    */
   public function getDataDesc() {
      return array(
          'id' => array('integer', 0), 'firmaID' => array('integer', 0),
          'mesec' => array('integer', 1), 'godina' => array('integer', 2016),
          'tipObracuna' => array('string', 'Konacni'), 'datumIsplate' => array('string', '0000-00-00'),
          'tipCenaRada' => array('integer', 1), 'cenaRada' => array('double', 0.0),
          'satiUMesecu' => array('integer', 0), 'koefRadPraznik' => array('double', 1.10),
          'koefRadNoc' => array('double', 0.26), 'koefRadPrekovremeno' => array('double', 0.26),
          'koefRadSmena' => array('double', 0.02), 'koefRadNedelja' => array('double', 0.10),
          'iznosTOPoDanu' => array('double', 0.0), 'iznosRegres' => array('double', 0.0),
          'iznosPoklonDeci' => array('double', 0.0), 'koefBolDo30D' => array('double', 0.65),
          'koefPrekid' => array('double', 0.60), 'stopaPoreza' => array('double', 10.0),
          'iznosNeoporezivo' => array('double', 11242.0), 'stopaPIO' => array('double', 14.0),
          'stopaZdravstvo' => array('double', 5.15), 'stopaNezaposlenost' => array('double', 0.75),
          'stopaPIOPoslodavac' => array('double', 12.0), 'stopaZdravstvoPoslodavac' => array('double', 5.15),
          'stopaNezaposlenostPoslodavac' => array('double', 0.75), 'minOsnovicaDoprinosa' => array('double', 22282.0),
          'iznosBrutoZarada' => array('double', 52438.0), 'iznosMinZarada' => array('double', 115.0),
      );
   }
   
   public function getFormDesc() {
      return array(
          'id' => array('hidden', ''), 'firmaID' => array('hidden', ''),
          'mesec' => array('mesec', 'Месец'), 'godina' => array('text', 'Година'),
          'tipObracuna' => array('tip_obracuna', 'Тип обрачуна'),
          'datumIsplate' => array('text', 'Датум исплате (гггг-мм-дд)'),
          'tipCenaRada' => array('tip_cena_rada', 'Тип цене рада'),
          'cenaRada' => array('text', 'Цена рада'),
          'satiUMesecu' => array('text', 'Број сати у месецу'),
          'koefRadPraznik' => array('text', 'Коефицијент за рад на празник'),
          'koefRadNoc' => array('text', 'Коефицијент за ноћни рад'),
          'koefRadPrekovremeno' => array('text', 'Коефицијент за прековремени рад'),
          'koefRadSmena' => array('text', 'Коефицијент за сменски рад'),
          'koefRadNedelja' => array('text', 'Коефицијент за рад недељом'),
          'iznosTOPoDanu' => array('text', 'Топли оброк по дану'),
          'iznosRegres' => array('text', 'Регрес'),
          'iznosPoklonDeci' => array('text', 'Поклон деци'),
          'koefBolDo30D' => array('text', 'Коефицијент за боловање до 30 дана'),
          'koefPrekid' => array('text', 'Коефицијент за прекид рада'),
          'stopaPoreza' => array('text', 'Стопа пореза'),
          'iznosNeoporezivo' => array('text', 'Неопорезиви износ'),
          'stopaPIO' => array('text', 'Стопа доприноса за ПИО'),
          'stopaZdravstvo' => array('text', 'Стопа доприноса за здравство'),
          'stopaNezaposlenost' => array('text', 'Стопа доприноса за незапосленос'),
          'stopaPIOPoslodavac' => array('text', 'Стопа доприноса за ПИО на терет послодавца'),
          'stopaZdravstvoPoslodavac' => array('text', 'Стопа доприноса за здравство на терет послодавца'),
          'stopaNezaposlenostPoslodavac' => array('text', 'Стопа доприноса за незапосленост на терет послодавца'),
          'minOsnovicaDoprinosa' => array('text', 'Минимална основица доприоноса'),
          'iznosBrutoZarada' => array('text', 'Просечна бруто зарада'),
          'iznosMinZarada' => array('text', 'Минимална зарада по сату'),
      );
   }

   public function getValidateDesc() {
      return array(
          'id' => array('not_negative'), 'firmaID' => array('not_negative'),
          'mesec' => array('not_negative'), 'godina' => array('not_negative'),
          'tipObracuna' => array('not_empty'), 'datumIsplate' => array('datum'),
          'tipCenaRada' => array('not_negative'), 'cenaRada' => array('not_negative'),
          'satiUMesecu' => array('not_negative'), 'koefRadPraznik' => array('not_negative'),
          'koefRadNoc' => array('not_negative'), 'koefRadPrekovremeno' => array('not_negative'),
          'koefRadSmena' => array('not_negative'), 'koefRadNedelja' => array('not_negative'),
          'iznosTOPoDanu' => array('not_negative'), 'iznosRegres' => array('not_negative'),
          'iznosPoklonDeci' => array('not_negative'), 'koefBolDo30D' => array('not_negative'),
          'koefPrekid' => array('not_negative'), 'stopaPoreza' => array('not_negative'),
          'iznosNeoporezivo' => array('not_negative'), 'stopaPIO' => array('not_negative'),
          'stopaZdravstvo' => array('not_negative'), 'stopaNezaposlenost' => array('not_negative'),
          'stopaPIOPoslodavac' => array('not_negative'), 'stopaZdravstvoPoslodavac' => array('not_negative'),
          'stopaNezaposlenostPoslodavac' => array('not_negative'), 'minOsnovicaDoprinosa' => array('not_negative'),
          'iznosBrutoZarada' => array('not_negative'), 'iznosMinZarada' => array('not_negative'),
      );
   }

   public function getDBDesc() {
      return array(
          'id' => 'sw', 'firmaID' => 'siu', 'mesec' => 'siu',
          'godina' => 'siu', 'tipObracuna' => 'siu', 'datumIsplate' => 'siu',
          'tipCenaRada' => 'siu', 'cenaRada' => 'siu', 'satiUMesecu' => 'siu',
          'koefRadPraznik' => 'siu', 'koefRadNoc' => 'siu', 'koefRadPrekovremeno' => 'siu',
          'koefRadSmena' => 'siu', 'koefRadNedelja' => 'siu',
          'iznosTOPoDanu' => 'siu', 'iznosRegres' => 'siu', 'iznosPoklonDeci' => 'siu',
          'koefBolDo30D' => 'siu', 'koefPrekid' => 'siu', 'stopaPoreza' => 'siu',
          'iznosNeoporezivo' => 'siu', 'stopaPIO' => 'siu', 'stopaZdravstvo' => 'siu',
          'stopaNezaposlenost' => 'siu', 'stopaPIOPoslodavac' => 'siu',
          'stopaZdravstvoPoslodavac' => 'siu', 'stopaNezaposlenostPoslodavac' => 'siu',
          'minOsnovicaDoprinosa' => 'siu', 'iznosBrutoZarada' => 'siu',
          'iznosMinZarada' => 'siu',
      );
   }

   /*
    * Definise abstraktni metod roditeljske klase i vraca naziv strukture.
    */
   public function getTableName() {
      return 'PAYMENT';
   }
}

/*
 * Klasa koja definise entitet za jednan Obracun.
 * Koristi opstiju Data klasu,
 * kao i klasu za opis strukture entiteta Obracuna.
 */
class Payment extends Data {
   /*
    * Konstruktor.
    * @param integer $fid - id firme kojoj pripada obracun.
    */
   public function __construct($fid = 0) {
      parent::__construct(new PaymentStructure);
      if($fid !== 0) {
         $this->setValue('firmaID', $fid);
      }
   }
   /*
    * Vraca opisni naziv obracuna
    * @return string - naziv obracuna
    */
   public function getName() {
      $output = "";
      $output .= $this->getMonthName() . " ";
      $output .= $this->getValue('godina') . " - " . $this->getPaymentType();
      return $output;
   }
   
   /**
    * Funkcija za prikaz tipa Obracuna.
    * U bazi je tip: Akontacija ili Konacni.
    * F-ja sluzi da vrati lepse formatiran string.
    * @return string
    */
   public function getPaymentType() {
      $tipObracuna = $this->getValue('tipObracuna');
      if($tipObracuna === "Akontacija") return "Аконтација";
      if($tipObracuna === "LZPred") return "Лична зарада предузетника";
      if($tipObracuna === "Konacni") return "Коначни обрачун";
      return "НИЈЕ ПРОНАЂЕН";
   }

   /**
    * Funkcija za tekstualni prikaz meseca Obracuna.
    * @return string
    */
   public function getMonthName() {
      $mesec = $this->getValue('mesec');
      if($mesec < 0 || $mesec > 12) {
         $mesec = 0;
      }
      $strMeseci = array(
          "Грешка", "Јануар", "Фебруар", "Март", "Април", "Мај", "Јун",
          "Јул", "Август", "Септембар", "Октобар", "Новембар", "Децембар"
         );
      return $strMeseci[$mesec];
   }
   
   /**
   * Minimalna zarada ukupno: $iznosMinZarada[po satu] *  broj sati u mesecu
   * @return float
   */
   function minZaradaUkupno() {
      return round($this->getValue('iznosMinZarada') * $this->getValue('satiUMesecu'), 2);
   }
   
   /**
   * Ukupna stopa doprinosa na teret zaposlenog.
   * @return float
   */
   function ukStopaDoprinosaZaposleni() {
      return round($this->getValue('stopaPIO') + $this->getValue('stopaZdravstvo') +
              $this->getValue('stopaNezaposlenost'), 2);
   }

   /**
   * Ukupna stopa doprinosa na teret poslodavca.
   * @return float
   */
   function ukStopaDoprinosaPoslodavac() {
      return round($this->getValue('stopaPIOPoslodavac') +
              $this->getValue('stopaZdravstvoPoslodavac') +
              $this->getValue('stopaNezaposlenostPoslodavac'), 2);
   }

   /**
   * Maksimalna osnovica doprinosa (5 * iznosBrutoZarada).
   * @return float
   */
   function maxOsnovicaDoprinosa() {
      return round($this->getValue('iznosBrutoZarada') * 5, 2);
   }
}
