<?php

require_once 'employe/class.Employe.inc';
require_once 'payment/class.Payment.inc';
require_once 'payment/class.PaymentDB.inc';
require_once 'karnet/class.Karnet.inc';
require_once 'deduction/class.Deduction.inc';
require_once 'deduction/class.DeductionDB.inc';

/**
 * Listic: formira obracunski listic za zadati obracun i karnet.
 */
class PaymentForEmploye {
   // Zaposleni na koga se odnosi obracun.
   public $employe;
   // Obracun na koji se odnosi listic.
   public $payment;
   // Karnet zaposlenog za taj obracun.
   public $karnet;
   
   // Da li za se radi o konacnom obracunu
   public $flagKon = true;
   // Da li za konacni obracun postoji i akontacija
   public $flagAk = true;

   public $akontacijaZarade = null;
   public $akontacijaNaknade = null;
   public $akontacijaPorez = null;
   public $akontacijaDoprinos = null;
   public $akontacijaNeto = null;
   
   public $konacniZarade = null;
   public $konacniNaknade = null;
   public $konacniPorez = null;
   public $konacniDoprinos = null;
   public $konacniNeto = null;
   
   /**
    * Konstuktor.
    */
   public function __construct(Employe $employe, Payment $payment, Karnet $karnet) {
      $this->employe = $employe;
      $this->payment = $payment;
      $this->karnet = $karnet;
      
      $this->init($this->payment, $this->karnet, $this->employe->getValue('id'));
   }
   
   protected function init(Payment $payment, Karnet $karnet, $eid) {
      if($this->payment->getValue('tipObracuna') === 'Akontacija') {
         $this->initAkontacija($payment, $karnet);
         $this->flagKon = false;
      } else {
         $this->initKonacni($payment, $karnet, $eid);
         $this->flagKon = true;
      }
   }

   protected function initAkontacija(Payment $payment, Karnet $karnet) {
      $this->akontacijaZarade = new Zarade($payment, $karnet);
      $this->akontacijaNaknade = new Naknade($payment, $karnet);
      $brutoUk = $this->brutoUkupnoAk();
      $this->akontacijaPorez = new Porez($payment, $karnet->satiUkupno(), $brutoUk);
      $this->akontacijaDoprinos = new Doprinos($payment, $karnet->satiUkupno(), $brutoUk);
      $this->akontacijaNeto = new Neto($this->employe->getValue('id'), $payment, $brutoUk,
              $this->akontacijaPorez->porez() + $this->akontacijaDoprinos->doprinos());
   }
   
   protected function initKonacni(Payment $payment, Karnet $karnet, $eid) {
      $this->konacniZarade = new Zarade($payment, $karnet);
      $this->konacniNaknade = new Naknade($payment, $karnet);
      $brutoUk = $this->brutoUkupnoKon();
      $this->konacniPorez = new Porez($payment, $karnet->satiUkupno(), $brutoUk);
      $this->konacniDoprinos = new Doprinos($payment, $karnet->satiUkupno(), $brutoUk);
      $this->konacniNeto = new Neto($this->employe->getValue('id'), $payment, $brutoUk,
              $this->konacniPorez->porez() + $this->konacniDoprinos->doprinos());
      
      $this->checkForAk($payment, $eid);
   }

   protected function checkForAk(Payment $payment, $eid) {
      // dodati da ucita akontaciju samo za tekucu firmu
      // ovo ipak radi jer nece naci za datog zaposlenog u drugoj firmi
      $payak = PaymentDB::load1($payment->getValue('mesec'), $payment->getValue('godina'), 'Akontacija');
     
      if($payak !== false) {
         $karnetAk = KarnetDB::load1($eid, $payak->getValue('id'));
         if($karnetAk !== false) {
            $this->initAkontacija($payak, $karnetAk);
         } else {
            $this->flagAk = false;
         }
      }
   }

   public function brutoUkupnoAk() {
      return $this->akontacijaZarade->zaradaUkupno() + $this->akontacijaNaknade->naknadeUkupno();
   }

   public function brutoUkupnoKon() {
      return $this->konacniZarade->zaradaUkupno() + $this->konacniNaknade->naknadeUkupno();
   }

   public function vrednostSata() {
      if($this->payment->getValue('tipObracuna') === 'Akontacija') {
         return $this->akontacijaZarade->vrednostSata();
      } else {
         return $this->konacniZarade->vrednostSata();
      }
   }
}

/**
 * Zarade: klasa koja racuna zarade za zadati obracun i karnet.
 */
class Zarade {
   // Obracun na koji se odnosi listic.
   public $payment;
   // Karnet zaposlenog za taj obracun.
   public $karnet;
   // vrednost sata, racuna se jednom a posle se samo procita.
   protected $vrednostSata = null;

   /**
    * Konstuktor.
    */
   public function __construct(Payment $payment, Karnet $karnet) {
      $this->payment = $payment;
      $this->karnet = $karnet;
   }

   /**
    * ZARADA: 1. + 2. + 3.
    * @return float
    */
   public function zaradaUkupno() {
      return ( $this->zaradaZaObavljeniRad() + $this->zaradaDoprinosPoslovnomUspehu() + $this->drugaPrimanja() );
   }

   /**
    * 1. Zarada za obavljeni rad i vreme provedeno na radu, cine je: 1.1. + 1.2. + 1.3.
    * @return float
    */
   public function zaradaZaObavljeniRad() {
      return ( $this->osnovnaZarada() + $this->zaradaZaRadniUcinak() + $this->uvecanaZarada() );
   }

   public function vrednostSata() {
      if(is_null($this->vrednostSata)) {
         $this->vrednostSata = $this->calcVrednostSata();
      }
      return $this->vrednostSata;
   }
   
   /**
    * Vrednost radnog sata zaposlenog, zavisi od koeficijenta zaposlenog,
    * od tipa cene rada (bruto/neto) i od cene rada za taj mesec i od broja radnih sati.
    * @return float
    */
   public function calcVrednostSata() {
       if($this->payment->getValue('tipObracuna') === 'Akontacija') {
            if ($this->karnet->getValue('tipCenaRada') === 1) {
               // bruto
               return round($this->karnet->getValue('cenaRada') * $this->karnet->getValue('koeficijent') /
                       $this->payment->getValue('satiUMesecu'), 5);
            } else {
               // neto
               return round($this->calcVrSataNaNetoCenuRada(), 5);
            }
       }
      if ($this->karnet->getValue('tipCenaRada') === 1) {
         // bruto
         return round($this->karnet->getValue('cenaRada') * $this->karnet->getValue('koeficijent') /
                 $this->payment->getValue('satiUMesecu'), 2);
      } else {
         // neto
         return round($this->calcVrSataNaNetoCenuRada(), 2);
      }
   }

   protected function calcVrSataNaNetoCenuRada() {
      $rez = $this->karnet->getValue('cenaRada') * $this->karnet->getValue('koeficijent');
      $umanj = $this->payment->getValue('iznosNeoporezivo') * $this->payment->getValue('stopaPoreza') / 100;
      $koef = $this->payment->getValue('stopaPoreza') + $this->payment->ukStopaDoprinosaZaposleni(); // u procentima
      $koef /= 100;
      return ($rez - $umanj) / (1 - $koef) / $this->payment->getValue('satiUMesecu');
   }
   
   /**
    * 1.1. Osnovna zarada = radni sati * vrednost sata.
    * @return float
    */
   public function osnovnaZarada() {
      $tipUm = $this->karnet->getValue('tipUmanjenja');
      $brSati = $this->karnet->radniSatiOsnovnaZarada();
      // standardno umanjenje, tj. umanjenje samo na osnovu umanjene cene rada
      if($tipUm === 1)
      {
         return round($brSati * $this->vrednostSata(), 2);
      }
      // $tipUm === 2, granicni slucaj, gde pada ispod 25000 neto, tj. 34059,63 bruto
      // pa ga treba vestacki vratiti na taj iznos
      $minBruto = 34059.63; // min bruto na 25000
      return round($minBruto / $this->payment->getValue('satiUMesecu') * $brSati, 2);
   }

   /**
    * 1.2. Zarada za radni ucinak.
    * @return float
    */
   public function zaradaZaRadniUcinak() {
      if ($this->karnet->getValue('tipRadniUcinak') === 1) {
         // iznos predstavlja procenat
         $osnovnaZarada = $this->osnovnaZarada();
         return round($this->karnet->getValue('iznosRadniUcinak') * $osnovnaZarada / 100, 2);
      } else {
         // iznos predstavlja fiksni bruto iznos
         return $this->karnet->getValue('iznosRadniUcinak');
      }
   }

   /**
    * 1.3. Uvecana zarada, cine je: 1.3.1., 1.3.2., 1.3.3., 1.3.4. i 1.3.5.
    * @return float
    */
   public function uvecanaZarada() {
      return ( $this->radNaPraznik() + $this->radNocu() + $this->radPraznikNoc() +
              $this->radPrekovremeno() + $this->radSmena() + 
              $this->radNedelja()+ $this->minuliRad() );
   }

   /**
    * 1.3.1. Rad na praznik koji je neradan dan.
    * @return float
    */
   public function radNaPraznik() {
      return round($this->karnet->getValue('satiRadPraznik') *
              $this->payment->getValue('koefRadPraznik') * $this->vrednostSata(), 2);
   }

   /**
    * 1.3.2. Rad nocu.
    * @return float
    */
   public function radNocu() {
      return round($this->karnet->getValue('satiRadNoc') *
              $this->payment->getValue('koefRadNoc') * $this->vrednostSata(), 2);
   }

   /**
    * 1.3.3. Rad na praznik nocu.
    * @return float
    */
   public function radPraznikNoc() {
      return round($this->karnet->getValue('satiRadPraznikNoc') *
              ($this->payment->getValue('koefRadNoc') + $this->payment->getValue('koefRadPraznik')) * $this->vrednostSata(), 2);
   }

   /**
    * 1.3.4. Prekovremeni rad.
    * @return float
    */
   public function radPrekovremeno() {
      return round($this->karnet->getValue('satiPrekovremeno') *
              $this->payment->getValue('koefRadPrekovremeno') * $this->vrednostSata(), 2);
   }

   /**
    * 1.3.6. Smenski rad.
    * @return float
    */
   public function radSmena() {
      return round($this->karnet->getValue('satiSmena') *
              $this->payment->getValue('koefRadSmena') * $this->vrednostSata(), 2);
   }

   /**
    * 1.3.6. Rad nedeljom.
    * @return float
    */
   public function radNedelja() {
      return round($this->karnet->getValue('satiNedelja') *
              $this->payment->getValue('koefRadNedelja') * $this->vrednostSata(), 2);
   }

   /**
    * 1.3.5. Minuli rad.
    * @return float
    */
   public function minuliRad() {
      return round($this->karnet->getValue('koefMinuliRad') * $this->osnovnaZarada() / 100, 2);
   }

   /**
    * 2. Zarada po osnovu doprinosa zaposlenog poslovnom uspehu poslodavca.
    *    (nagrade, bonusi, "trinaesta plata" i sl.)
    * @return float
    */
   public function zaradaDoprinosPoslovnomUspehu() {
      if ($this->karnet->getValue('tipNagrada') === 1) {
         return round($this->karnet->getValue('iznosNagrada'), 2);
      } else {
         return 0;
      }
   }

   /**
    * 3. Druga primanja u skladu sa opstim aktom (topli obrok, regres,
    *    otpremnina, terenski dodatak, poklon deci, ostalo) - definisano
    *    direktno u samom karnetu
    * @return float
    */
   public function drugaPrimanja() {
      return round($this->topliObrok() + $this->karnet->getValue('iznosOtpremnina') +
              $this->karnet->getValue('iznosRegres') + $this->karnet->getValue('iznosTerenskiDodatak') +
              $this->karnet->getValue('iznosPoklonDeci') + $this->karnet->getValue('iznosOstalo'), 2);
   }

   /**
    * Topli obrok: u karnetu je dat iznos po danu.
    * @return float
    */
   public function topliObrok() {
      return round($this->karnet->getValue('iznosTOPoDanu') *
              $this->karnet->getValue('daniTO'), 2);
   }
}

/**
 * Naknade: klasa koja racuna naknade za zadati obracun i karnet.
 */
class Naknade {
   // Obracun na koji se odnosi listic.
   public $payment;
   // Karnet zaposlenog za taj obracun.
   public $karnet;

   /**
    * Konstuktor.
    */
   public function __construct(Payment $payment, Karnet $karnet) {
      $this->payment = $payment;
      $this->karnet = $karnet;
   }

   /**
    * NAKNADE ZARADE
    * @return float
    */
   public function naknadeUkupno() {
      return ($this->naknadaGO() + $this->naknadaPlacenoOdsustvo() + $this->naknadaOdsustvoPraznik() +
              $this->naknadaBolDo30D() + $this->naknadaBolDo30D100Posto() + $this->naknadaPrekid() +
              $this->naknadaVojnaVezba() + $this->naknadaPozivDrzOrgana() );
   }

   /**
    * Godisnji odmor.
    * @return float
    */
   public function naknadaGO() {
      return round($this->karnet->getValue('satiGO') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
    * Placeno odsustvo.
    * @return float
    */
   public function naknadaPlacenoOdsustvo() {
      return round($this->karnet->getValue('satiPlacenoOdsustvo') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
    * Odsustvovanje sa rada na dan praznika koji je neradni dan.
    * @return float
    */
   public function naknadaOdsustvoPraznik() {
      return round($this->karnet->getValue('satiOdsustvoPraznik') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
    * Bolovanje do 30 dana (standardno) - ne manje od minimalne zarade.
    * @return float
    */
   public function naknadaBolDo30D() {
      return round($this->karnet->getValue('satiBolDo30D') *
              $this->payment->getValue('koefBolDo30D') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
    * Bolovanje do 30 dana sa isplatom 100%.
    * @return float
    */
   public function naknadaBolDo30D100Posto() {
      return round($this->karnet->getValue('satiBolDo30D100Posto') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
    * Prekid rada do kojeg je doslo bez krivice zaposlenog.
    * (najduze 45 radnih dana u kalendarskoj godini - ne manje od minimalne zarade)
    * @return float
    */
   public function naknadaPrekid() {
      return round($this->karnet->getValue('satiPrekid') *
              $this->payment->getValue('koefPrekid') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
    * Odsustvovanje zbog vojne vezbe.
    * @return float
    */
   public function naknadaVojnaVezba() {
      return round($this->karnet->getValue('satiVojnaVezba') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
    * Odsustvovanje zbog odazivanja na poziv drzavnog organa.
    * @return float
    */
   public function naknadaPozivDrzOrgana() {
      return round($this->karnet->getValue('satiPozivDrzOrgana') * $this->karnet->getValue('prosek3Meseca'), 2);
   }
}

/**
 * Porez: klasa koja racuna porez za zadati obracun i karnet.
 */
class Porez {
   // Obracun na koji se odnosi listic.
   public $payment;
   // Ukupno sati za zaposlenog.
   public $ukSati;
   // Ukupan bruto za zaposlenog.
   public $ukBruto;

   /**
    * Konstuktor.
    */
   public function __construct(Payment $payment, $ukSati, $ukBruto) {
      $this->payment = $payment;
      $this->ukSati = $ukSati;
      $this->ukBruto = $ukBruto;
   }

   /**
    * Porez.
    * @return float - izracunati porez
    */
   public function porez() {
      $ukNeoporezovano = $this->neoporezivo();

      if ($this->ukBruto > $ukNeoporezovano) {
         return round(($this->ukBruto - $ukNeoporezovano) * $this->payment->getValue('stopaPoreza') / 100, 2);
      } else {
         return 0;
      }
   }

   /**
    * Osnovica za obracun poreza.
    * @return float - izracunati porez
    */
   public function osnovicaPorez() {
      $ukNeoporezovano = $this->neoporezivo();

      if ($this->ukBruto > $ukNeoporezovano) {
         return round($this->ukBruto - $ukNeoporezovano, 2);
      } else {
         return 0;
      }
   }

   /**
    * Neoporezivi iznos poreza.
    * @return float
    */
   public function neoporezivo() {
      if ($this->ukSati >= $this->payment->getValue('satiUMesecu')) {
         return $this->payment->getValue('iznosNeoporezivo');
      }

      $neoporezivoPoSatu = $this->payment->getValue('iznosNeoporezivo') / $this->payment->getValue('satiUMesecu');
      return round($neoporezivoPoSatu * $this->ukSati, 2);
   }

   /**
    * Stopa poreza.
    * @return float
    */
   public function stopa() {
      return $this->payment->getValue('stopaPoreza');
   }
}

/**
 * Doprinos: klasa koja racuna doprinose za zadati obracun i karnet.
 */
class Doprinos {
   // Obracun na koji se odnosi listic.
   public $payment;
   // Ukupno sati za zaposlenog.
   public $ukSati;
   // Ukupan bruto za zaposlenog.
   public $ukBruto;

   /**
    * Konstuktor.
    */
   public function __construct(Payment $payment, $ukSati ,$ukBruto) {
      $this->payment = $payment;
      $this->ukSati = $ukSati;
      $this->ukBruto = $ukBruto;
   }

   /**
    * Doprinos.
    * @return float
    */
   public function doprinos() {
      return round($this->osnovicaDoprinosa($this->ukBruto) * $this->payment->ukStopaDoprinosaZaposleni() / 100, 2);
   }

   /**
    * Doprinos.
    * @return float
    */
   public function doprinosPoslodavac() {
      return round($this->osnovicaDoprinosa($this->ukBruto) * $this->payment->ukStopaDoprinosaPoslodavac() / 100, 2);
   }

   /**
    * Osnovica za paymentacun doprinosa.
    * @return float
    */
   public function osnovicaDoprinosa() {
      $osnovica = $this->izracunajMinOsnDop();

      if ($this->ukBruto > $osnovica) {
         if ($this->ukBruto > $this->payment->maxOsnovicaDoprinosa()) {
            $osnovica = $this->payment->maxOsnovicaDoprinosa();
         } else {
            $osnovica = $this->ukBruto;
         }
      }
      return round($osnovica, 2);
   }

   /**
    * Minimalna osnovica doprinosa.
    * @return float
    */
   protected function izracunajMinOsnDop() {
      if ($this->ukSati >= $this->payment->getValue('satiUMesecu')) {
         return $this->payment->getValue('minOsnovicaDoprinosa');
      }

      $osnovica = $this->payment->getValue('minOsnovicaDoprinosa') / $this->payment->getValue('satiUMesecu') * $this->ukSati;
      return round($osnovica, 2);
   }

   /**
    * Doprinos za PIO na teret zaposlenog.
    * @return float
    */
   public function doprinosPIO() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaPIO') / 100, 2);
   }

   /**
    * Doprinos za zdravstveno osiguranje na teret zaposlenog.
    * @return float
    */
   public function doprinosZdravstvo() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaZdravstvo') / 100, 2);
   }

   /**
    * Doprinos za nezaposlenost na teret zaposlenog.
    * @return float
    */
   public function doprinosNezaposlenost() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaNezaposlenost') / 100, 2);
   }

   /**
    * Doprinos za PIO na teret poslodavca.
    * @return float
    */
   public function doprinosPIOPoslodavac() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaPIOPoslodavac') / 100, 2);
   }

   /**
    * Doprinos za zdravstveno osiguranje na teret poslodavca.
    * @return float
    */
   public function doprinosZdravstvoPoslodavac() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaZdravstvoPoslodavac') / 100, 2);
   }

   /**
    * Doprinos za nezaposlenost na teret poslodavca.
    * @return float
    */
   public function doprinosNezaposlenostPoslodavac() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaNezaposlenostPoslodavac') / 100, 2);
   }
}

/**
 * Neto: klasa koja racuna neto za zadati obracun i karnet.
 */
class Neto {
   // Id zaposlenog
   public $eid;
   // Obracun na koji se odnosi listic.
   public $payment;
   // Ukupan bruto na obracunu.
   public $ukBruto;
   // Ukupno porezi i doprinosi.
   public $ukPID;
   // Ukupan bruto za zaposlenog.
   public $ukUmanjenje;

   /**
    * Konstuktor.
    */
   public function __construct($eid, Payment $payment, $ukBruto, $ukPID) {
      $this->eid = $eid;
      $this->payment = $payment;
      $this->ukBruto = $ukBruto;
      $this->ukPID = $ukPID;
      $this->ukUmanjenje = NetoUmanjenje::get($this->netoUkupno());
   }

   /**
    * Ukupna neto zarada: ukupan bruto minus porezi i doprinosi.
    * @return float
    */
   public function netoUkupno() {
      return ( $this->ukBruto - $this->ukPID );
   }

   /**
    * Ukupna neto zarada: ukupan bruto minus porezi i doprinosi.
    * @return float
    */
   public function umanjenje() {
      return $this->ukUmanjenje;
   }

   /**
    * Ukupna neto zarada: ukupan bruto minus porezi i doprinosi.
    * @return float
    */
   public function umanjeniNeto() {
      return ( $this->netoUkupno() - $this->ukUmanjenje );
   }
   
   /**
    * Ukupna neto zarada za isplatu: ukupan neto minus umanjenje minus obustave.
    * @return float
    */
   public function netoZaIsplatu($ukupnoObustave) {
      return ( $this->netoUkupno() - $this->ukUmanjenje - $ukupnoObustave );
   }
}

class NetoUmanjenje {
   public static function check() {
      $tipFirme = Application::getCurrFirmProp("currFirmType");
      if($tipFirme === 1 || $tipFirme === 3) {
         // ako je javno preduzece;
         $payment = PaymentDB::load(Application::getCurrPaymentID());
         $mesec = $payment->getValue('mesec');
         $godina = $payment->getValue('godina');
         $lDatumUslov = ($godina === 2014 && $mesec > 10) || $godina > 2014;

         if($lDatumUslov) {
            // umanjenje je racunato zakljucno sa oktobrom 2014.
            return false; 
         } else {
            return true; 
         }
      }
      return false; // ako nije javno preduzece;
   }

   public static function get($neto) {
      if(!self::check()) {
         return 0;
      }
      if($neto <= 60000) {
         return 0;
      }
      if($neto <= 100000) {
         return round( ($neto - 60000) * 0.2, 2);
      }
      return round( 8000 + ($neto - 100000) * 0.25, 2);
   }
}
