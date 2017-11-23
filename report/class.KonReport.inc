<?php

require_once 'employe/class.Employe.inc';
require_once 'payment/class.Payment.inc';
require_once 'karnet/class.Karnet.inc';
require_once 'firm/class.FirmDB.inc';
require_once 'deduction/class.Deduction.inc';
require_once 'deduction/class.DeductionDB.inc';
require_once 'deduction/class.DeductionForm.inc';

/**
 * Listic: formira obracunski listic za zadat konacni obracun i karnet.
 */
class KonReport {
   // Zaposleni na koga se odnosi obracun. Potreban je zbog stampe zaglavlja.
   public $employe;
   // Obracun na koji se odnosi listic.
   public $payment;
   // Karnet zaposlenog za taj obracun.
   public $karnet;

   const RADNI_SATI_U_DANU = 8;

   /**
    * Konstuktor.
    */
   function __construct(Employe $employe, Payment $payment, Karnet $karnet ) {
      $this->employe = $employe;
      $this->payment = $payment;
      $this->karnet = $karnet;
   }

   /**
   * ZARADA: 1. + 2. + 3.
   * @return float
   */
   function zaradaUkupno() {
      return ( $this->zaradaZaObavljeniRad() + $this->zaradaDoprinosPoslovnomUspehu() + $this->drugaPrimanja() );
   }
   
   /**
   * 1. Zarada za obavljeni rad i vreme provedeno na radu, cine je: 1.1. + 1.2. + 1.3.
   * @return float
   */
   function zaradaZaObavljeniRad() {
      return ( $this->osnovnaZarada() + $this->zaradaZaRadniUcinak() + $this->uvecanaZarada() );
   }
   
   /**
   * 1.1. Osnovna zarada = radni sati * vrednost sata.
   * @return float
   */
   function osnovnaZarada() {
      return round($this->karnet->radniSatiOsnovnaZarada() * $this->vrednostSata(), 2);
   }

   /**
   * Vrednost radnog sata zaposlenog, zavisi od koeficijenta zaposlenog,
   * od tipa cene rada (bruto/neto) i od cene rada za taj mesec i od broja radnih sati.
   * @return float
   */
   function vrednostSata() {
      if($this->karnet->getValue('tipCenaRada') === 1) {
         // bruto
         return round($this->karnet->getValue('cenaRada') * $this->karnet->getValue('koeficijent') /
                 $this->payment->getValue('satiUMesecu'), 6);
      } else {
         // neto
         return round($this->calcVrSataNaNetoCenuRada(), 6); 
      }
   }
   
   function calcVrSataNaNetoCenuRada() {   
      $rez = $this->karnet->getValue('cenaRada') * $this->karnet->getValue('koeficijent');
      $umanj = $this->payment->getValue('iznosNeoporezivo') * $this->payment->getValue('stopaPoreza') / 100;
      $koef = $this->payment->getValue('stopaPoreza') + $this->payment->ukStopaDoprinosaZaposleni(); // u procentima
      $koef /= 100;
      return ($rez - $umanj) / (1 - $koef) / $this->payment->getValue('satiUMesecu');
   }
   
   /**
   * 1.2. Zarada za radni ucinak.
   * @return float
   */
   function zaradaZaRadniUcinak() {
      $osnovnaZarada = $this->osnovnaZarada();
      
      if($this->karnet->getValue('tipRadniUcinak') === 1) {
         // iznos predstavlja procenat
         return round($this->karnet->getValue('iznosRadniUcinak') * $osnovnaZarada / 100, 2);
      } else {
         // iznos predstavlja fiksni bruto iznos
         return $this->karnet->getValue('iznosRadniUcinak');
      }
   }

   /**
   * 1.3. Uvecana zarada, cine je: 1.3.1., 1.3.2., 1.3.3. i 1.3.4.
   * @return float
   */
   function uvecanaZarada() {
      return ( $this->radNaPraznik() + $this->radNocu() + $this->radPraznikNoc() +
              $this->radPrekovremeno() + $this->minuliRad() );
   }

   /**
   * 1.3.1. Rad na praznik koji je neradan dan.
   * @return float
   */
   function radNaPraznik() {
      return round($this->karnet->getValue('satiRadPraznik') *
              $this->payment->getValue('koefRadPraznik') * $this->vrednostSata(), 2);
   }

   /**
   * 1.3.2. Rad nocu.
   * @return float
   */
   function radNocu() {
      return round($this->karnet->getValue('satiRadNoc') *
              $this->payment->getValue('koefRadNoc') * $this->vrednostSata(), 2);
   }

   /**
   * 1.3.3. Rad na praznik nocu.
   * @return float
   */
   function radPraznikNoc() {
      return round($this->karnet->getValue('satiRadPraznikNoc') *
              ($this->payment->getValue('koefRadNoc') + $this->payment->getValue('koefRadPraznik')) * $this->vrednostSata(), 2);
   }
   
   /**
   * 1.3.4. Prekovremeni rad.
   * @return float
   */
   function radPrekovremeno() {
      return round($this->karnet->getValue('satiPrekovremeno') *
              $this->payment->getValue('koefRadPrekovremeno') * $this->vrednostSata(), 2);
   }
   
   /**
   * 1.3.5. Minuli rad.
   * @return float
   */
   function minuliRad() {
      return round($this->karnet->getValue('koefMinuliRad') * $this->osnovnaZarada() / 100, 2);
   }

   /**
   * 2. Zarada po osnovu doprinosa zaposlenog poslovnom uspehu poslodavca.
   *    (nagrade, bonusi, "trinaesta plata" i sl.)
   * @return float
   */
   function zaradaDoprinosPoslovnomUspehu() {
      if($this->karnet->getValue('tipNagrada') === 1) {
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
   function drugaPrimanja() {
      return round($this->topliObrok() + $this->karnet->getValue('iznosOtpremnina') +
              $this->karnet->getValue('iznosRegres') + $this->karnet->getValue('iznosTerenskiDodatak') +
              $this->karnet->getValue('iznosPoklonDeci') + $this->karnet->getValue('iznosOstalo'), 2);
   }

   /**
   * Topli obrok: u karnetu je dat iznos po danu.
   * @return float
   */
   function topliObrok() {
//      return round($this->karnet->getValue('iznosTOPoDanu') *
//              $this->karnet->radniSatiOsnovnaZarada() / self::RADNI_SATI_U_DANU, 2);
      return round($this->karnet->getValue('iznosTOPoDanu') *
              $this->karnet->getValue('daniTO'), 2);
   }
  
   /**
   * NAKNADE ZARADE
   * @return float
   */
   function naknadeUkupno() {
      return ($this->zaradaGO() + $this->zaradaPlacenoOdsustvo() + $this->zaradaOdsustvoPraznik() +
              $this->zaradaBolDo30D() + $this->zaradaBolDo30D100Posto() + $this->zaradaPrekid() +
              $this->zaradaVojnaVezba() + $this->zaradaPozivDrzOrgana() );
   }

   /**
   * Godisnji odmor.
   * @return float
   */
   function zaradaGO() {
      return round($this->karnet->getValue('satiGO') * $this->karnet->getValue('prosek3Meseca'), 2);
   }
   
   /**
   * Placeno odsustvo.
   * @return float
   */
   function zaradaPlacenoOdsustvo() {
      return round($this->karnet->getValue('satiPlacenoOdsustvo') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
   * Odsustvovanje sa rada na dan praznika koji je neradni dan.
   * @return float
   */
   function zaradaOdsustvoPraznik() {
      return round($this->karnet->getValue('satiOdsustvoPraznik') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
   * Bolovanje do 30 dana (standardno) - ne manje od minimalne zarade.
   * @return float
   */
   function zaradaBolDo30D() {
      return round($this->karnet->getValue('satiBolDo30D') *
              $this->payment->getValue('koefBolDo30D') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
   * Bolovanje do 30 dana sa isplatom 100%.
   * @return float
   */
   function zaradaBolDo30D100Posto() {
      return round($this->karnet->getValue('satiBolDo30D100Posto') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
   * Prekid rada do kojeg je doslo bez krivice zaposlenog.
   * (najduze 45 radnih dana u kalendarskoj godini - ne manje od minimalne zarade)
   * @return float
   */
   function zaradaPrekid() {
      return round($this->karnet->getValue('satiPrekid') *
              $this->payment->getValue('koefPrekid') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
   * Odsustvovanje zbog vojne vezbe.
   * @return float
   */
   function zaradaVojnaVezba() {
      return round($this->karnet->getValue('satiVojnaVezba') * $this->karnet->getValue('prosek3Meseca'), 2);
   }

   /**
   * Odsustvovanje zbog odazivanja na poziv drzavnog organa.
   * @return float
   */
   function zaradaPozivDrzOrgana() {
      return round($this->karnet->getValue('satiPozivDrzOrgana') * $this->karnet->getValue('prosek3Meseca'), 2);
   }
   
   /**
   * Porez.
   * @return float
   */
   function porez() {
      $ukNeoporezovano = $this->neoporezivo();
      $ukBruto = $this->brutoUkupno();

      if($ukBruto > $ukNeoporezovano) {
         return round( ($ukBruto - $ukNeoporezovano) * $this->payment->getValue('stopaPoreza') / 100, 2);
      } else {
         return 0;
      }
   }

   /**
   * Neoporezivi iznos poreza.
   * @return float
   */
   function neoporezivo() {
      $ukSati = $this->satiUkupno();
      if($ukSati >= $this->payment->getValue('satiUMesecu')) {
         return $this->payment->getValue('iznosNeoporezivo');
      }
      
      $neoporezivoPoSatu = $this->payment->getValue('iznosNeoporezivo') / $this->payment->getValue('satiUMesecu');
      return round($neoporezivoPoSatu * $ukSati, 2);
   }
   
   /**
   * Ukupno radnih sati za racunanje poreza i doprinosa.
   * @return int
   */
   function satiUkupno() {
      $ukSati = $this->karnet->radniSatiOsnovnaZarada() + 
              $this->karnet->radniSatiNaknade();
      return $ukSati;
   }

   /**
   * Ukupna bruto zarada: ukupna zarada plus naknada.
   * @return float
   */
   function brutoUkupno() {
      return ( $this->zaradaUkupno() + $this->naknadeUkupno() );
   }

   /**
   * Doprinos.
   * @return float
   */
   function doprinos() {
      return round($this->osnovicaDoprinosa() * $this->payment->ukStopaDoprinosaZaposleni() / 100, 2);
   }

   /**
   * Osnovica za paymentacun doprinosa.
   * @return float
   */
   function osnovicaDoprinosa() {
      $osnovica = $this->izracunajMinOsnDop();
      $ukBruto = $this->brutoUkupno();
      if($ukBruto > $osnovica) {
         if($ukBruto > $this->payment->maxOsnovicaDoprinosa()) {
            $osnovica = $this->payment->maxOsnovicaDoprinosa();
         } else {
            $osnovica = $ukBruto;
         }
      }
      return round($osnovica, 2);
   }

   /**
   * Minimalna osnovica doprinosa.
   * @return float
   */
   function izracunajMinOsnDop() {
      $ukSati = $this->satiUkupno();
      if($ukSati >= $this->payment->getValue('satiUMesecu')) {
         return $this->payment->getValue('minOsnovicaDoprinosa');
      }

      $osnovica = $this->payment->getValue('minOsnovicaDoprinosa') / $this->payment->getValue('satiUMesecu') * $ukSati;
      return round($osnovica, 2);
   }
   
   /**
   * Doprinos za PIO na teret zaposlenog.
   * @return float
   */
   function doprinosPIO() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaPIO') / 100, 2);
   }

   /**
   * Doprinos za zdravstveno osiguranje na teret zaposlenog.
   * @return float
   */
   function doprinosZdravstvo() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaZdravstvo') / 100, 2);
   }

   /**
   * Doprinos za nezaposlenost na teret zaposlenog.
   * @return float
   */
   function doprinosNezaposlenost() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaNezaposlenost') / 100, 2);
   }

   /**
   * Doprinos za PIO na teret poslodavca.
   * @return float
   */
   function doprinosPIOPoslodavac() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaPIOPoslodavac') / 100, 2);
   }

   /**
   * Doprinos za zdravstveno osiguranje na teret poslodavca.
   * @return float
   */
   function doprinosZdravstvoPoslodavac() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaZdravstvoPoslodavac') / 100, 2);
   }

   /**
   * Doprinos za nezaposlenost na teret poslodavca.
   * @return float
   */
   function doprinosNezaposlenostPoslodavac() {
      return round($this->osnovicaDoprinosa() * $this->payment->getValue('stopaNezaposlenostPoslodavac') / 100, 2);
   }
   
   /**
   * Ukupna neto zarada: ukupan bruto minus porezi i doprinosi.
   * @return float
   */
   function netoUkupno() {
      return ( $this->brutoUkupno() - $this->porez() - $this->doprinos() );
   }

   /**
   * Umanjenje za javna preduzeca.
   * @return float
   */
   function netoUmanjenje() {
      if(NetoUmanjenje::check()) {
         return NetoUmanjenje::get($this->netoUkupno());
      }
      return 0; // ako nije javno preduzece;      
   }

   /**
   * Ukupne obustave za tekuci obracun za datog zaposlenog.
   * @return float
   */
   function ukupnoObustave() {
      $mesec = $this->payment->getValue('mesec');
      $godina = $this->payment->getValue('godina');
      $tipObracuna = $this->payment->getValue('tipObracuna');
      $employeID = $this->employe->getValue('id');
      
      $nizObustava = DeductionDB::ucitajSveZaObracun($employeID, $mesec, $godina, $tipObracuna);
      return $this->sumObustave($nizObustava, $mesec, $godina, $tipObracuna);
   }
   
   function sumObustave($nizObustava, $mesec, $godina, $tipObracuna) {
      $ukObustave = 0;
      foreach( $nizObustava as $obustava ) {
         foreach( $obustava->rate as $rata ) {
            if($rata->getValue('mesec') ===  $mesec && $rata->getValue('godina') === $godina &&
                    $rata->getValue('tipObracuna') === $tipObracuna) {
               $ukObustave += $rata->getValue('iznos');
            }
         }
      }
      return $ukObustave;
   }
   
   /**
   * Ukupna neto zarada za isplatu: ukupan neto minus umanjenje minus obustave.
   * @return float
   */
   function netoZaIsplatu() {
      return ( $this->netoUkupno() - $this->netoUmanjenje() - $this->ukupnoObustave() );
   }
}


class KonReportRender {
   // Numerisanje stampe
   protected $num1 = 1; // prvi nivo numeracije
   protected $num2 = 1; // drugi nivo numeracije
   protected $num3 = 1; // treci nivo numeracije
   
   /**
    * Stampa pojedinacnog obracunskog listica u HTML kod.
    */
   public function display(KonReport $r) {
      $output = "";
      $output .= "<div class=\"listic\">\n";
      $output .= $this->tableHeader($r);

      $output .= $this->tableZarada($r);
      $output .= $this->tableNaknade($r);
      $output .= $this->tablePoreziDoprinosi($r);
      $output .= $this->tableDeduction($r);
      
      // $output .= $this->tableBody($r);
      $output .= "</div>\n";
      return $output;
   }
   
   protected function tableHeader(KonReport $r) {
      $output  = "<table class=\"listic-header\">\n";
      $output .="<tr><th colspan=\"2\">" . Application::getCurrFirmName() . "</th></tr>\n";
      $output .="<tr><th colspan=\"2\">Обрачун: {$r->payment->getName()}</th></tr>\n";
      // $output .= $this->tableHeadRow("Обрачун", $r->payment->getName());
      $output .= $this->tableHeadRow("Запослени", $r->employe->getValue('ime') . " " . $r->employe->getValue('prezime'));
      $output .= $this->tableHeadRow("Коефицијент", $r->karnet->getValue('koeficijent'));
      $tipCeneRada = $r->karnet->getValue('tipCenaRada') === 1 ? "Бруто" : "Нето";
      $output .= $this->tableHeadRow("Тип цене рада", $tipCeneRada);
      $output .= $this->tableHeadRow("Цена рада", $r->karnet->getValue('cenaRada'));
      $output .= $this->tableHeadRow("Број сати у месецу", $r->payment->getValue('satiUMesecu'));
      $output .= $this->tableHeadRow("Вредност сата", $r->vrednostSata());
      $output .= $this->tableHeadRow("Просек предходна 3 месеца", $r->karnet->getValue('prosek3Meseca'));
      $output .= "</table>\n";
      return $output;
   }

   /**
    * Stampa pojedinacnog reda u zaglavlju tabele za obracunski listic.
    */
   private function tableHeadRow($label, $value) {
      if(is_numeric($value)) {
         $displayValue = number_format($value, 2, ",", ".");
      } else {
         $displayValue = $value;
      }
      return "<tr><td>{$label}</td><td>{$value}</td></tr>\n";
   }

   protected function tableBody(KonReport $r) {
      $output  = "<table class=\"listic-body\">\n";
      $output .= "<tr><th>Опис</th><th>Износ</th></tr>\n";
      
      $output .= $this->tableZarada($r);
      $output .= $this->tableNaknade($r);
      $output .= $this->tablePoreziDoprinosi($r);
      $output .= $this->tableDeduction($r);

      $output .= "</table>\n";
      return $output;
   }

   /**
    * Stampa pojedinacnog reda u centralnom delu tabele za obracunski listic.
    */
   private function tableBodyRow($label, $value, $level, $increment = false ,$force = false) {
      if(is_numeric($value)) {
         if( ($value === 0.0 || $value === 0) && !$force ) {
            return "";
         } else {
            $displayValue = number_format($value, 2, ",", ".");
         }
      } else {
         $displayValue = $value;
      }
      $lab = $this->makeLabel($label, $level);
      $this->incrementLabel($level, $increment);
      return "<tr><td>{$lab}</td><td>{$displayValue}</td></tr>\n";
   }
   protected function makeLabel($label, $level) {
      switch ($level) {
         case 1:
            return $this->makeLabel1($label);
         case 2:
            return $this->makeLabel2($label);
         case 3:
            return $this->makeLabel3($label);
         default:
            return $label;
      }
   }
   protected function incrementLabel($level, $increment) {
      if($increment === false) { return; }
      switch ($level) {
         case 1:
            return $this->incrementLabel1();
         case 2:
            return $this->incrementLabel2();
         case 3:
            return $this->incrementLabel3();
      }
   }
   private function makeLabel1($label) {
      $output = "{$this->num1}. {$label}";
      return $output;
   }
   private function incrementLabel1() {
      ++$this->num1; $this->num2 = 1; $this->num3 = 1;
   }
   private function makeLabel2($label) {
      $output = "{$this->num1}.{$this->num2}. {$label}";
      return $output;
   }
   private function incrementLabel2() {
      ++$this->num2; $this->num3 = 1;
   }
   private function makeLabel3($label) {
      $output = "{$this->num1}.{$this->num2}.{$this->num3}. {$label}";
      return $output;
   }
   private function incrementLabel3() {
      ++$this->num3;
   }
   
   protected function tableZarada(KonReport $r) {
      $output  = "<table class=\"listic-zarada\">\n";
      $output .= "<tr><th>Опис</th><th>Износ</th></tr>\n";
      
      $output .= $this->tableBodyRow("I ЗАРАДА УКУПНО", $r->zaradaUkupno(), 0, false, true);
      $output .= $this->tableBodyRow("Зарада за обављени рад", $r->zaradaZaObavljeniRad(), 1);
      $output .= $this->tableBodyRow("Основна зарада [сати {$r->karnet->radniSatiOsnovnaZarada()}]", $r->osnovnaZarada(), 2, true);
      $output .= $this->tableBodyRow("Зарада за радни учинак", $r->zaradaZaRadniUcinak(), 2, true);
      // $predh = $this->num2;
      $output .= $this->tableBodyRow("Увећана зарада", $r->uvecanaZarada(), 2);
      $output .= $this->tableBodyRow("Зарада за рад на празник [сати {$r->karnet->getValue('satiRadPraznik')}]", $r->radNaPraznik(), 3, true);
      $output .= $this->tableBodyRow("Зарада за рад ноћу [сати {$r->karnet->getValue('satiRadNoc')}]", $r->radNocu(), 3, true);
      $output .= $this->tableBodyRow("Зарада за ноћни рад на празник [сати {$r->karnet->getValue('satiRadPraznikNoc')}]", $r->radPraznikNoc(), 3, true);
      $output .= $this->tableBodyRow("Зарада за прековремени рад [сати {$r->karnet->getValue('satiPrekovremeno')}]", $r->radPrekovremeno(), 3, true);
      $output .= $this->tableBodyRow("Минули рад [{$r->karnet->getValue('koefMinuliRad')}%]", $r->minuliRad(), 3, true);

      $this->incrementLabel1();
      $output .= $this->tableBodyRow("Зарада за допринос пословном успеху", $r->zaradaDoprinosPoslovnomUspehu(), 1, true);

      $output .= $this->tableBodyRow("Друга примања", $r->drugaPrimanja(), 1);
      // $toDana = $r->karnet->radniSatiOsnovnaZarada() / KonReport::RADNI_SATI_U_DANU;
      $toDana = $r->karnet->getValue('daniTO');
      $output .= $this->tableBodyRow("Топли оброк [дана {$toDana}]", $r->topliObrok(), 2, true);
      $output .= $this->tableBodyRow("Регрес", $r->karnet->getValue('iznosRegres'), 2, true);
      $output .= $this->tableBodyRow("Отпремнина", $r->karnet->getValue('iznosOtpremnina'), 2, true);
      $output .= $this->tableBodyRow("Теренски додатак", $r->karnet->getValue('iznosTerenskiDodatak'), 2, true);
      $output .= $this->tableBodyRow("Поклон деци", $r->karnet->getValue('iznosPoklonDeci'), 2, true);
      $output .= $this->tableBodyRow("Остало [{$r->karnet->getValue('opisOstalo')}]", $r->karnet->getValue('iznosOstalo'), 2, true);

      // $output .= "</table>\n";
      return $output;
   }
   
   protected function tableNaknade(KonReport $r) {
      $output  = "";
      // $output  = "<table class=\"listic-naknada\">\n";
      // $output .= "<tr><th>Опис</th><th>Износ</th></tr>\n";

      $output .= $this->tableBodyRow("II НАКНАДЕ ЗАРАДЕ УКУПНО", $r->naknadeUkupno(), 0, false, true);
      $this->incrementLabel1();
      $output .= $this->tableBodyRow("Годишњи одмор [сати {$r->karnet->getValue('satiGO')}]", $r->zaradaGO(), 2, true);
      $output .= $this->tableBodyRow("Плаћено одсуство [сати {$r->karnet->getValue('satiPlacenoOdsustvo')}]", $r->zaradaPlacenoOdsustvo(), 2, true);
      $output .= $this->tableBodyRow("Одсуство на држ. празник [сати {$r->karnet->getValue('satiOdsustvoPraznik')}]",
              $r->zaradaOdsustvoPraznik(), 2, true);
      $output .= $this->tableBodyRow("Боловање до 30 дана [сати {$r->karnet->getValue('satiBolDo30D')}]", $r->zaradaBolDo30D(), 2, true);
      $output .= $this->tableBodyRow("Боловање до 30 дана - 100% [сати {$r->karnet->getValue('satiBolDo30D100Posto')}]",
              $r->zaradaBolDo30D100Posto(), 2, true);
      // $output .= $this->tableBodyRow("Боловање преко 30 дана [сати {$r->karnet->getValue('satiBolPreko30D')}]", 0, 2, true);
      $output .= $this->tableBodyRow("Прекид рада без кривице запосленог [сати {$r->karnet->getValue('satiPrekid')}]", $r->zaradaPrekid(), 2, true);
      $output .= $this->tableBodyRow("Војна вежба [сати {$r->karnet->getValue('satiVojnaVezba')}]", $r->zaradaVojnaVezba(), 2, true);
      $output .= $this->tableBodyRow("Позив државног ограна [сати {$r->karnet->getValue('satiPozivDrzOrgana')}]", $r->zaradaPozivDrzOrgana(), 2, true);
      
      $output .= $this->tableBodyRow("УКУПНО ЗАРЕДЕ И НАКНАДЕ [сати {$r->satiUkupno()}]", $r->brutoUkupno(), 0, false, true);
      $output .= "</table>\n";
      return $output;
   }
   
   protected function tablePoreziDoprinosi(KonReport $r) {
      $output  = "<table class=\"listic-pid\">\n";
      $output .= "<tr><th>Опис</th><th>Износ</th></tr>\n";

      $output .= $this->tableBodyRow("Неопорезиво [мах. {$r->payment->getValue('iznosNeoporezivo')}]", $r->neoporezivo(), 0);
      $output .= $this->tableBodyRow("Порез [стопа {$r->payment->getValue('stopaPoreza')}]", $r->porez(), 0);
      $output .= $this->tableBodyRow("Основица доприноса", $r->osnovicaDoprinosa(), 0);
      $output .= $this->tableBodyRow("Допринос [ук. стопа {$r->payment->ukStopaDoprinosaZaposleni()}]", $r->doprinos(), 0);
      $output .= $this->tableBodyRow(" - ПИО [{$r->payment->getValue('stopaPIO')}]", $r->doprinosPIO(), 0);
      $output .= $this->tableBodyRow(" - Здравство [{$r->payment->getValue('stopaZdravstvo')}]", $r->doprinosZdravstvo(), 0);
      $output .= $this->tableBodyRow(" - Незапосленост [{$r->payment->getValue('stopaNezaposlenost')}]", $r->doprinosNezaposlenost(), 0);
      
      $output .= $this->tableBodyRow("УКУПНО ПОРЕЗИ И ДОПРИНОСИ", $r->porez() + $r->doprinos(), 0);
      $netouk = $r->netoUkupno();
      $output .= $this->tableBodyRow("НЕТО УКУПНО", $netouk, 0);
      if(NetoUmanjenje::check()) {
         $umanj = $r->netoUmanjenje();
         $objasnjenje = "";
         if($netouk <= 60000) {
            $objasnjenje = "(" . $netouk . "*1)";
         } else {
            if($netouk <= 100000) {
               $objasnjenje = "(60.000,00*1+" . ($netouk-60000) . "*0.8)";
            } else {
               $objasnjenje = "(60.000,00*1+(100.000,00-60.000,00)*0.8 +" . ($netouk-100000) . "*0.75)";
            }
         }
         $output .= $this->tableBodyRow("УМАЊЕНИ НЕТО " . $objasnjenje, $netouk - $umanj, 0, false, true);
         $output .= $this->tableBodyRow("УМАЊЕЊЕ", $umanj, 0, false, true);
      }
      // $output .= "</table>\n";
      return $output;
   }
   
   protected function tableDeduction(KonReport $r) {
      $mesec = $r->payment->getValue('mesec');
      $godina = $r->payment->getValue('godina');
      $tipObracuna = $r->payment->getValue('tipObracuna');
      
      $nizObustava = DeductionDB::ucitajSveZaObracun($r->employe->getValue('id'), $mesec, $godina, $tipObracuna);
      
      $ukObustave = $r->sumObustave($nizObustava, $mesec, $godina, $tipObracuna);
      
      $output = $this->tableBodyRow("ОБУСТАВЕ УКУПНО", $ukObustave, 0, false, true);
      foreach( $nizObustava as $obustava ) {
         $rbr = 1;
         foreach( $obustava->rate as $rata ) {
            if($rata->getValue('mesec') ===  $mesec &&
                    $rata->getValue('godina') === $godina &&
                    $rata->getValue('tipObracuna') === $tipObracuna) {
               $naziv = ShopFormElement::getName($obustava->getValue('radnjaID'));
               $output .= $this->tableBodyRow(
                       "{$naziv}: рата {$rbr} од {$obustava->getBrRata()}", $rata->getValue('iznos'), 0);
               // $ukObustave += $rata->getValue('iznos');
            }
            ++$rbr;
         }
      }
      $output .= $this->tableBodyRow("НЕТО ЗА ИСПЛАТУ", $r->netoUkupno() - $r->netoUmanjenje() - $ukObustave, 0);
      
      $output .= "</table>\n";
      return $output;
   }
}
