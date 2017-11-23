<?php

require_once 'util/class.Data.inc';
require_once 'payment/class.PaymentDB.inc';
require_once 'report/class.PFE.inc';

/*
 * Klasa definise strukturu entiteta koji opisuje opste podatke o Karnet.
 */
class KarnetStructure extends Structure {
// zaposleniID - sifra zaposlenog
// obracunID - sifra obracuna kojem karnet pripada
// koeficijent - koeficijent, inicijalno se postavlja iz podataka o zaposlenom
// satiRadni - broj radnih sati za taj mesec
// tipCenaRada - bruto (1) ili neto (2)
// cenaRada - cena rada za zaposlenog, inicijalno se postavlja iz cene rada iz obracuna
// tipRadniUcinak - tip radni ucinak (da bi se omogucilo racunanje ucinka na vise nacina)
// iznosRadniUcinak - interpretacija ovog iznosa zavisi od tipa za radni ucinak
// satiRadPraznik - sati za rad na drzavni praznik
// satiRadNoc - sati za nocni rad,
// satiRadPraznikNoc - sati za rad nocu na drzavni praznik
// satiPrekovremeno - sati za prekovremeni rad
// satiSmena - sati za rad u smeni
// satiNedelja - sati za rad nedeljom
// koefMinuliRad - koeficijent za minuli rad, inicijalno se postavlja iz podataka o zaposlenom
// tipNagrada - nacin obracuna za nagradu (da bi se omogucilo racunanje na vise nacina)
// iznosNagrada - interpretacija ovog polja zavisi od nacina obracuna nagrade
// iznosOtpremnina - otpremnina
// iznosTOPoDanu - topli obrok po danu, inicijalizuje se iz obracuna
// iznosRegres - regres, inicijalizuje se iz obracuna
// iznosTerenskiDodatak - iznos terenski dodatak
// iznosPoklonDeci - iznos poklon deci, inicijalizuje se iz obracuna
// opisOstalo - opis za neku dodatnu nepredvidjenu stavku
// iznosOstalo - iznos za neku dodatnu nepredvidjenu stavku
// prosek3Meseca - prosek tri meseca
// satiGO - sati godisnji odmor
// satiPlacenoOdsustvo - sati placeno odsustvo
// satiOdsustvoPraznik - sati odsustvo praznik
// satiBolDo30D - sati bolovanje do 30 dana
// satiBolDo30D100Posto - sati bolovanje do 30 dana sa isplatom 100%
// satiBolPreko30D - sati bolovanje preko 30 dana
// satiPrekid - sati prekid rada bez krivice zaposlenog
// satiVojnaVezba - sati vojna vezba
// satiPozivDrzOrgana - sati poziv drzavnih organa
// tipUmanjenja - Standardno ili Granica 25000, samo kod JP
   
   /*
    * Definise abstraktni metod roditeljske klase i vraca opis strukture.
    */
   public function getDataDesc() {
      return array(
          'obracunID' => array('integer', 0), 'zaposleniID' => array('integer', 0),
          'koeficijent' => array('double', 0.0), 'satiRadni' => array('integer', 0),
          'tipCenaRada' => array('integer', 1), 'cenaRada' => array('double', 0.0),
          'tipRadniUcinak' => array('integer', 1), 'iznosRadniUcinak' => array('double', 0.0),
          'satiRadPraznik' => array('integer', 0), 'satiRadNoc' => array('integer', 0),
          'satiRadPraznikNoc' => array('integer', 0), 'satiPrekovremeno' => array('integer', 0),
          'satiSmena' => array('integer', 0), 'satiNedelja' => array('integer', 0),
          'koefMinuliRad' => array('double', 0.0), 'tipNagrada' => array('integer', 1),
          'iznosNagrada' => array('double', 0.0), 'iznosOtpremnina' => array('double', 0.0),
          'iznosTOPoDanu' => array('double', 0.0), 'daniTO' => array('integer', 0),
          'iznosRegres' => array('double', 0.0), 'iznosTerenskiDodatak' => array('double', 0.0),
          'iznosPoklonDeci' => array('double', 0.0), 'opisOstalo' => array('string', ''),
          'iznosOstalo' => array('double', 0.0), 'prosek3Meseca' => array('double', 0.0),
          'satiGO' => array('integer', 0), 'satiPlacenoOdsustvo' => array('integer', 0),
          'satiOdsustvoPraznik' => array('integer', 0), 'satiBolDo30D' => array('integer', 0),
          'satiBolDo30D100Posto' => array('integer', 0), 'satiBolPreko30D' => array('integer', 0),
          'satiPrekid' => array('integer', 0), 'satiVojnaVezba' => array('integer', 0),
          'satiPozivDrzOrgana' => array('integer', 0), 'tipUmanjenja' => array('integer', 1)
      );
   }

   public function getFormDesc() {
      // 'zaposleniID' => array('zaposleni', 'Избор запосленог'),
      return array(
          'obracunID' => array('hidden', ''), 'zaposleniID' => array('hidden', ''),
          'koeficijent' => array('text', 'Коефицијент'), 'satiRadni' => array('text', 'Радни сати'),
          'tipCenaRada' => array('tip_cena_rada', 'Тип цене рада'), 'cenaRada' => array('text', 'Цена рада'),
          'tipRadniUcinak' => array('tip_radni_ucinak', 'Тип обрачуна радног учинка'),
          'iznosRadniUcinak' => array('text', 'Радни учинак'),
          'satiRadPraznik' => array('text', 'Рад на државни празник сати'),
          'satiRadNoc' => array('text', 'Ноћни рад сати'),
          'satiRadPraznikNoc' => array('text', 'Ноћни рад на државни празник сати'),
          'satiPrekovremeno' => array('text', 'Прековремени рад сати'),
          'satiSmena' => array('text', 'Сменски рад сати'),
          'satiNedelja' => array('text', 'Рад недељом сати'),
          'koefMinuliRad' => array('text', 'Коефицијент за минули рад'),
          'tipNagrada' => array('tip_nagrada', 'Тип обрачуна награде'),
          'iznosNagrada' => array('text', 'Награда'), 'iznosOtpremnina' => array('text', 'Отпремнина'),
          'iznosTOPoDanu' => array('text', 'Топли оброк по дану'),
          'daniTO' => array('text', 'Топли оброк број дана'),
          'iznosRegres' => array('text', 'Регрес'), 'iznosTerenskiDodatak' => array('text', 'Теренски додатак'),
          'iznosPoklonDeci' => array('text', 'Поклон деци'), 'opisOstalo' => array('text', 'Опис за ставку остало'),
          'iznosOstalo' => array('text', 'Износ за ставку остало'), 'prosek3Meseca' => array('text', 'Просек предходна три месеца'),
          'satiGO' => array('text', 'Годишњи одмор сати'), 'satiPlacenoOdsustvo' => array('text', 'Плаћено одсуство сати'),
          'satiOdsustvoPraznik' => array('text', 'Одсуство на дан који је државни празник сати'),
          'satiBolDo30D' => array('text', 'Боловање до 30 дана сати'),
          'satiBolDo30D100Posto' => array('text', 'Боловање до 30 дана са исплатом 100% сати'),
          'satiBolPreko30D' => array('text', 'Боловање преко 30 дана сати'),
          'satiPrekid' => array('text', 'Прекид рада без кривице заполсеног сати'),
          'satiVojnaVezba' => array('text', 'Војна вежба сати'),
          'satiPozivDrzOrgana' => array('text', 'Позив државних ограна сати'),
          'tipUmanjenja' => array('tip_umanjenja', 'Тип обрачуна умањења')
      );
   }
   
   public function getValidateDesc() {
      return array(
          'obracunID' => array('not_negative'), 'zaposleniID' => array('not_negative'),
          'koeficijent' => array('not_negative'), 'satiRadni' => array('not_negative'),
          'tipCenaRada' => array('not_negative'), 'cenaRada' => array('not_negative'),
          'tipRadniUcinak' => array('not_negative'), 'satiRadPraznik' => array('not_negative'),
          'satiRadNoc' => array('not_negative'), 'satiRadPraznikNoc' => array('not_negative'),
          'satiPrekovremeno' => array('not_negative'), 'satiSmena' => array('not_negative'),
          'satiNedelja' => array('not_negative'), 'koefMinuliRad' => array('not_negative'),
          'tipNagrada' => array('not_negative'), 'iznosNagrada' => array('not_negative'),
          'iznosOtpremnina' => array('not_negative'), 'iznosTOPoDanu' => array('not_negative'),
          'daniTO' => array('not_negative'), 'iznosRegres' => array('not_negative'),
          'iznosTerenskiDodatak' => array('not_negative'), 'iznosPoklonDeci' => array('not_negative'),
          'prosek3Meseca' => array('not_negative'), 'satiGO' => array('not_negative'),
          'satiPlacenoOdsustvo' => array('not_negative'), 'satiOdsustvoPraznik' => array('not_negative'),
          'satiBolDo30D' => array('not_negative'), 'satiBolDo30D100Posto' => array('not_negative'),
          'satiBolPreko30D' => array('not_negative'), 'satiPrekid' => array('not_negative'),
          'satiVojnaVezba' => array('not_negative'), 'satiPozivDrzOrgana' => array('not_negative'),
          'tipUmanjenja' => array('not_negative')
      );
   }
   
   public function getDBDesc() {
      return array(
          'obracunID' => 'si', 'zaposleniID' => 'si', 'koeficijent' => 'siu',
          'satiRadni' => 'siu', 'tipCenaRada' => 'siu', 'cenaRada' => 'siu',
          'tipRadniUcinak' => 'siu', 'iznosRadniUcinak' => 'siu', 'satiRadPraznik' => 'siu',
          'satiRadNoc' => 'siu', 'satiRadPraznikNoc' => 'siu', 'satiPrekovremeno' => 'siu',
          'satiSmena' => 'siu', 'satiNedelja' => 'siu',
          'koefMinuliRad' => 'siu', 'tipNagrada' => 'siu', 'iznosNagrada' => 'siu',
          'iznosOtpremnina' => 'siu', 'iznosTOPoDanu' => 'siu', 'daniTO' => 'siu',
          'iznosRegres' => 'siu', 'iznosTerenskiDodatak' => 'siu', 'iznosPoklonDeci' => 'siu',
          'opisOstalo' => 'siu', 'iznosOstalo' => 'siu', 'prosek3Meseca' => 'siu',
          'satiGO' => 'siu', 'satiPlacenoOdsustvo' => 'siu', 'satiOdsustvoPraznik' => 'siu',
          'satiBolDo30D' => 'siu', 'satiBolDo30D100Posto' => 'siu', 'satiBolPreko30D' => 'siu',
          'satiPrekid' => 'siu', 'satiVojnaVezba' => 'siu', 'satiPozivDrzOrgana' => 'siu',
          'tipUmanjenja' => 'siu'
      );
   }

   /*
    * Definise abstraktni metod roditeljske klase i vraca naziv strukture.
    */
   public function getTableName() {
      return 'KARNET';
   }
}

/*
 * Klasa koja definise entitet za jednan Karnet.
 * Koristi opstiju Data klasu,
 * kao i klasu za opis strukture entiteta Karneta.
 */
class Karnet extends Data {
   /*
    * Konstruktor.
    * @param integer $fid - id firme kojoj pripada obracun.
    */
   public function __construct(Payment $payment = null, Employe $employe = null) {
      parent::__construct(new KarnetStructure);
      if(!is_null($employe)) {
         $this->initEmployeFields($employe);
      }
      if(!is_null($payment)) {
         $this->initPaymentFields($payment);
      }
      if(!is_null($employe) && !is_null($payment)) {
         $this->setValue('prosek3Meseca', $this->calcProsek3Meseca($payment, $employe));
      }
   }
   
   protected function initEmployeFields(Employe $employe) {
      $this->setValue('zaposleniID', $employe->getValue('id'));
      $this->setValue('koeficijent', $employe->getValue('koeficijent'));
      $this->setValue('koefMinuliRad', $employe->getMinuliRad());
   }
   
   protected function initPaymentFields(Payment $payment) {
      $this->setValue('obracunID', $payment->getValue('id'));
      $this->setValue('tipCenaRada', $payment->getValue('tipCenaRada'));
      $this->setValue('cenaRada', $payment->getValue('cenaRada'));
      $this->setValue('iznosTOPoDanu', $payment->getValue('iznosTOPoDanu'));
      $this->setValue('iznosRegres', $payment->getValue('iznosRegres'));
      $this->setValue('iznosPoklonDeci', $payment->getValue('iznosPoklonDeci'));
   }
   
   protected function calcProsek3Meseca(Payment $payment, Employe $employe) {
      $nizKarneta = $this->getPredh3Meseca($payment, $employe);
      if(count($nizKarneta) === 0) {
         return 0;
      }

      $nizPFE = array();
      foreach ($nizKarneta as $karnet) {
         $payment = PaymentDB::load($karnet->getValue('obracunID'));
         $nizPFE[] = new PaymentForEmploye($employe, $payment, $karnet);
      }

      // Racunaj prosek ako ima tri predhodna obracuna za zaposlenog.
      $avg = 0;
      $avgsati = 0;
      foreach ($nizPFE as $pfe) {
         $avg += $pfe->konacniZarade->zaradaZaObavljeniRad();
         $avgsati += $pfe->karnet->radniSatiOsnovnaZarada();
      }
      
      return round($avg / $avgsati, 2);
   }

   // Vrati karnete od predhodna tri meseca, ako ih ima za zaposlenog.
   // Do 08.2014. racuna se prosek predhodnih 12 meseci.
   protected function getPredh3Meseca(Payment $payment, Employe $employe) {
      $mesec = $payment->getValue('mesec');
      $godina = $payment->getValue('godina');
      
      $nizObracuna = PaymentDB::loadKarnetZarade(Application::getCurrFirmID(),
              $this->prethodniMesec($mesec, 12), $this->prethodnaGodina($mesec, $godina, 12),
              $this->prethodniMesec($mesec, 1), $this->prethodnaGodina($mesec, $godina, 1));
      
      $nizKarneta = array();
      foreach ($nizObracuna as $obracun) {
         if(KarnetDB::exist($employe->getValue('id'), $obracun->getValue('id'))) {
            $nizKarneta[] = KarnetDB::load($employe->getValue('id'), $obracun->getValue('id'));
         }
      }

      return $nizKarneta;
   }

   // Vrati mesec obracuna za $n iza tekuceg
   protected function prethodniMesec($mesec, $n) {
      $m = $mesec - $n;
      if($m <= 0) {
         $m += 12;
      }
      return $m;
   }

   // Vrati karnete od predhodna tri meseca, ako ih ima za zaposlenog.
   protected function prethodnaGodina($mesec, $godina, $n) {
      $m = $mesec - $n;
      if($m <= 0) {
         return $godina-1;
      }
      return $godina;
   }
   
   /**
   * Ukupni radni sati za osnovnu zaradu.
   * @return float
   */
   function radniSatiOsnovnaZarada() {
      return round($this->getValue('satiRadni') + $this->getValue('satiRadPraznik') +
              $this->getValue('satiRadNoc') + $this->getValue('satiRadPraznikNoc') +
              $this->getValue('satiPrekovremeno') + $this->getValue('satiSmena') + 
              $this->getValue('satiNedelja'), 2);
   }

   /**
   * Ukupni radni sati za naknade na teret poslodavca.
   * NEMA sadi za prekid! Da li mozda treba i oni da se uvrste?
   * @return float
   */
   function radniSatiNaknade() {
      return round($this->getValue('satiGO') + $this->getValue('satiPlacenoOdsustvo') +
              $this->getValue('satiOdsustvoPraznik') + $this->getValue('satiBolDo30D') +
              $this->getValue('satiBolDo30D100Posto') + $this->getValue('satiVojnaVezba') +
              $this->getValue('satiPozivDrzOrgana'), 2);
   }

   /**
   * Ukupno radnih sati za racunanje poreza i doprinosa.
   * @return int
   */
   function satiUkupno() {
      return $this->radniSatiOsnovnaZarada() + $this->radniSatiNaknade();
   }
}
