<?php

require_once 'employe/class.Employe.inc';
require_once 'payment/class.Payment.inc';
require_once 'karnet/class.Karnet.inc';
require_once 'deduction/class.Deduction.inc';
require_once 'deduction/class.DeductionDB.inc';
//require_once 'deduction/class.DeductionForm.inc';
require_once 'report/class.PFE.inc';

class PFERender {
   protected static $indent;
   
   /**
    * Stampa pojedinacnog reda tablele. Formira se red u tabeli
    * bez obzira da li je vrednost 0.
    * @param string $label Opis polja.
    * @param float $value Vrednost polja.
    * @param string $pad klasa koja se dodaje za potrebe stilizovanja reda u tabeli.
    */
   protected static function makeTableRow($label, $value, $pad = "") {
      $fvalue = number_format($value, 2, ",", ".");
      $output  = str_repeat("\t", static::$indent + 1);
      if(empty($pad)) {
         $output .= "<tr><td>{$label}</td><td>{$fvalue}</td></tr>\n";
      } else {
         $output .= "<tr class=\"{$pad}\"><td class=\"{$pad}\">{$label}</td><td>{$fvalue}</td></tr>\n";
      }
      return $output;
   }

   /**
    * Stampa pojedinacnog reda tablele. Formira se red u tabeli
    * samo ako je vrednost veca od 0.
    * @param string $label Opis polja.
    * @param float $value Vrednost polja.
    * @param string $pad klasa koja se dodaje za potrebe stilizovanja reda u tabeli.
    */
   protected static function makeTableRowGt0($label, $value, $pad = "") {
      if(abs($value) > 0) {
         return self::makeTableRow($label, $value, $pad);
      }
      return "";
   }
   
   public static function display1(PaymentForEmploye $pfe, $indent = 5) {
      self::$indent = $indent;

      echo str_repeat("\t", self::$indent) . "<div class=\"listic\">\n";
      echo ZaglavljeRender::display($pfe, self::$indent + 1);
      
      if($pfe->payment->getValue('tipObracuna') === 'Akontacija') {
         echo self::displayAkontacija($pfe);
      } else {
         echo self::displayKonacni($pfe);
      }
      echo self::futerDisplay();
      
      echo str_repeat("\t", self::$indent) . "</div> <!-- listic -->\n";
   }

   /**
    * Stampa pojedinacnog obracunskog listica akontacije u HTML kod.
    */
   public static function displayAkontacija(PaymentForEmploye $pfe) {
      $output  = str_repeat("\t", self::$indent + 1) . "<div class=\"listic-kol1\">\n";
      $output .= ZaradeRender::display($pfe->akontacijaZarade, self::$indent + 2);
      $output .= NaknadeRender::display($pfe->akontacijaNaknade, self::$indent + 2);
      $output .= self::makeTableUkZiN($pfe->brutoUkupnoAk(), $pfe->karnet->satiUkupno(), self::$indent + 2);
      $output .= str_repeat("\t", self::$indent + 1) . "</div> <!-- listic-kol1 -->\n";
      
      $output .= str_repeat("\t", self::$indent + 1) . "<div class=\"listic-kol2\">\n";
      $output .= PIDRender::display($pfe->akontacijaPorez, $pfe->akontacijaDoprinos, self::$indent + 2);
      $output .= NetoRender::display($pfe->akontacijaNeto, self::$indent+1);
      $output .=  str_repeat("\t", self::$indent + 1) . "</div> <!-- listic-kol2 -->\n";

      return $output;
   }

   /**
    * Stampa pojedinacnog obracunskog listica u HTML kod.
    */
   public static function displayKonacni(PaymentForEmploye $pfe) {
      $output  = str_repeat("\t", self::$indent + 1) . "<div class=\"listic-kol1\">\n";
      $output .= ZaradeRender::display($pfe->konacniZarade, self::$indent + 2);
      $output .= NaknadeRender::display($pfe->konacniNaknade, self::$indent + 2);
      $output .= self::makeTableUkZiN($pfe->brutoUkupnoKon(), $pfe->karnet->satiUkupno());
      $output .= str_repeat("\t", self::$indent + 1) . "</div> <!-- listic-kol1 -->\n";
      
      $output .= str_repeat("\t", self::$indent + 1) . "<div class=\"listic-kol2\">\n";
      $output .= PIDRender::display($pfe->konacniPorez, $pfe->konacniDoprinos, self::$indent + 2);
      $output .= NetoRender::display($pfe->konacniNeto, self::$indent + 2);
      $output .=  str_repeat("\t", self::$indent + 1) . "</div> <!-- listic-kol2 -->\n";

      return $output;
   }

   protected static function makeTableUkZiN($ukBruto, $sati) {
      $output = str_repeat("\t", self::$indent + 2) . "<table class=\"listic-ukupno\">\n";
      $output .= self::makeTableRow("3. УКУПНО ЗАРЕДЕ И НАКНАДЕ [сати {$sati}] (1+2)", $ukBruto, "lev0");
      $output .= str_repeat("\t", self::$indent + 2) . "</table> <!-- listic-ukupno -->\n";
      return $output;
   }

   protected static function futerDisplay() {
      $output = str_repeat("\t", self::$indent + 1) . "<div class=\"listic-futer\">\n";
      $output .= str_repeat("\t", self::$indent + 2) . "<p>Неисплаћена нето зарада, односно нето накнада зараде: ____________</p>\n";
      $output .= str_repeat("\t", self::$indent + 2) . "<p>Датум доспелости неисплаћене зараде, односно накнаде зараде: __________</p>\n";
      $output .= str_repeat("\t", self::$indent + 2) . "<table class=\"listic-potpis-dir\">\n";
      $output .= str_repeat("\t", self::$indent + 3) . "<tr>\n";
      $output .= str_repeat("\t", self::$indent + 3) . "<td></td><td>М.П.</td><td>Овлашћено лице</td>\n";
      $output .= str_repeat("\t", self::$indent + 3) . "</tr>\n";
      $output .= str_repeat("\t", self::$indent + 2) . "</table> <!-- listic-potpis-dir -->\n";
      $output .= str_repeat("\t", self::$indent + 1) . "</div> <!-- listic-futer -->\n";
      return $output;
   }
}

class ZaglavljeRender extends PFERender {
   protected static $indent;
   /**
    * Stampa zaglavlja pojedinacnog obracunskog listica u HTML kod.
    */
   public static function display(PaymentForEmploye $payemp, $indent) {
      self::$indent = $indent;
      return self::makeTable($payemp);
   }

   protected static function hederDisplay(PaymentForEmploye $payemp) {
      $output  = str_repeat("\t", self::$indent) . "<h4>Образац ОЗ</h4>\n";
      $cObr = "Обрачун зараде за месец: " . $payemp->payment->getName();
      $cDatum = $payemp->payment->getValue('datumIsplate');
      if($cDatum === '0000-00-00') {
         $cDatum = '____________';
      } else {
         $di = new DateTime($cDatum);
         $cDatum = $di->format("j.n.Y");
      }
      $output .= str_repeat("\t", self::$indent) . "<h5>" . $cObr . ", датум доспелости " . $cDatum . "</h5>\n";
      return $output;
   }

   protected static function makeTable(PaymentForEmploye $payemp) {
      $output  = self::hederDisplay($payemp);

      $output .= str_repeat("\t", self::$indent) . "<table class=\"listic-header\">\n";
      $output .= self::firmData();
      $output .= str_repeat("\t", self::$indent) . "</table> <!-- listic-header -->\n";
      
      $output .= str_repeat("\t", self::$indent) . "<table class=\"listic-header\">\n";
      
      // Prvi red za zaposlenog
      $ime = $payemp->employe->getValue('ime') . " " . $payemp->employe->getValue('prezime');
      $cJMBG = $payemp->employe->getValue('jmbg');
      if($payemp->employe->getValue('interniID') === 0) {
         $cSif = '';
      } else {
         $cSif = $payemp->employe->getValue('interniID');
      }
      
      $output .= str_repeat("\t", self::$indent + 1);
      $output .="<tr><th>Запослени: {$ime}</th>";
      $output .= "<th>ЈМБГ: {$cJMBG}</th>";
      $output .= "<th>Шифра запосленог: {$cSif}</th></tr>\n";

      // Drugi red za zaposlenog
      $cAdr = $payemp->employe->getValue('adresa') . ", " . $payemp->employe->getValue('mesto');
      $cBrRac = $payemp->employe->getValue('racun');
      $cBanka = BankFormElement::getName($payemp->employe->getValue('bankaID'));

      $output .= str_repeat("\t", self::$indent + 1);
      $output .="<tr><td>{$cAdr}</td>";
      $output .= "<td>Бр. рачуна: {$cBrRac}</td>";
      $output .= "<td>Банка: {$cBanka}</td></tr>\n";
      
      // Treci red za zaposlenog
      $tipCeneRada = $payemp->karnet->getValue('tipCenaRada') === 1 ? "Бруто" : "Нето";
      $output .= self::employe($tipCeneRada, $payemp->karnet->getValue('cenaRada'),
              $payemp->karnet->getValue('koeficijent'), $payemp->payment->getValue('satiUMesecu'));
      
      // Cetvrti red za zaposlenog
      $output .= self::vrednostSati($payemp->vrednostSata(),
              $payemp->karnet->getValue('prosek3Meseca'),
              $payemp->payment->getValue('iznosNeoporezivo') / $payemp->payment->getValue('satiUMesecu'));

      $output .= str_repeat("\t", self::$indent) . "</table> <!-- listic-header -->\n";
      return $output;
   }

   protected static function firmData() {
      $output = str_repeat("\t", self::$indent + 1);
      $output .="<tr><th>" . Application::getCurrFirmName() . "</th>";
      $output .= "<th>ПИБ: " . Application::getCurrFirmProp('currFirmPIB') . "</th>";
      $output .= "<th>Матични број: " . Application::getCurrFirmProp('currFirmMatbr') . "</th></tr>\n";

      $cAdr = Application::getCurrFirmProp('currFirmAddress') . ", " . Application::getCurrFirmProp('currFirmCity');
      $output .= str_repeat("\t", self::$indent + 1);
      $output .="<tr><td>" . $cAdr . "</td>";
      $output .= "<td>Број рачуна: " . Application::getCurrFirmProp('currFirmAccountNum') . "</td>";
      $output .= "<td>Банка: " . Application::getCurrFirmProp('currFirmBankName') . "</td></tr>\n";

      return $output;
   }

   protected static function employe($tipCeneRada, $cena, $koef, $brSatiUM) {
      $fcena = number_format($cena, 2, ",", ".");
      $fkoef = number_format($koef, 2, ",", ".");

      $output = str_repeat("\t", self::$indent + 1);
      $output .= "<tr><td>Цена рада [{$tipCeneRada}]: {$fcena}</td>";
      $output .= "<td>Коефицијент: {$fkoef}</td>";
      $output .= "<td>Сати у месецу: {$brSatiUM}</td></tr>\n";

      return $output;
   }

   protected static function vrednostSati($vrsati, $vrprosek3m, $neopPoSatu) {
      $fvrsati = number_format($vrsati, 2, ",", ".");
      $fvrprosek3m = number_format($vrprosek3m, 2, ",", ".");
      $fneopPoSatu = number_format($neopPoSatu, 2, ",", ".");

      $output  = str_repeat("\t", self::$indent + 1);
      $output .= "<tr><td>Вредност радног сата: {$fvrsati}</td>";
      $output .= "<td>Просек предходних 12 месеци по сату: {$fvrprosek3m}</td>";
      $output .= "<td>Неопорезиво по сату: {$fneopPoSatu}</td></tr>\n";

      return $output;
   }
}

class ZaradeRender extends PFERender {
   protected static $indent;
   /**
    * Stampa zarada pojedinacnog obracunskog listica u HTML kod.
    */
   public static function display(Zarade $zarade, $indent) {
      self::$indent = $indent;
      return self::makeTable($zarade);
   }

   protected static function makeTable(Zarade $zarade) {
      $output = str_repeat("\t", self::$indent) . "<table class=\"listic-zarada\">\n";
      $output .= str_repeat("\t", self::$indent + 1) . "<tr><th>Опис</th><th>Износ</th></tr>\n";

      $output .= self::makeTableRow("1. ЗАРАДА", $zarade->zaradaUkupno(), "lev0");

      $output .= self::displayZaradaZaObavljeniRad($zarade);
      $output .= self::makeTableRowGt0("Зарада за допринос пословном успеху", $zarade->zaradaDoprinosPoslovnomUspehu(), "lev1");
      $output .= self::displayDrugaPrimanja($zarade);

      $output .= str_repeat("\t", self::$indent) . "</table> <!-- listic-zarada -->\n";
      return $output;
   }
   
   protected static function displayZaradaZaObavljeniRad(Zarade $zarade) {
      $output = "";
      $output .= self::makeTableRow("Зарада за обављени рад", $zarade->zaradaZaObavljeniRad(), "lev1");
      $output .= self::makeTableRowGt0("Основна зарада [сати {$zarade->karnet->radniSatiOsnovnaZarada()}]",
              $zarade->osnovnaZarada(), "lev2");
      $output .= self::makeTableRowGt0("Зарада за радни учинак", $zarade->zaradaZaRadniUcinak(), "lev2");
      $output .= self::displayUvecanaZarada($zarade);
      return $output;
   }
   
   protected static function displayUvecanaZarada(Zarade $zarade) {
      $output = ""; $output .= self::makeTableRowGt0("Увећана зарада", $zarade->uvecanaZarada(), "lev2");
      $output .= self::makeTableRowGt0("Зарада за рад на празник [сати {$zarade->karnet->getValue('satiRadPraznik')}]", $zarade->radNaPraznik(), "lev3");
      $output .= self::makeTableRowGt0("Зарада за рад ноћу [сати {$zarade->karnet->getValue('satiRadNoc')}]",
              $zarade->radNocu(), "lev3");
      $output .= self::makeTableRowGt0("Зарада за ноћни рад на празник [сати {$zarade->karnet->getValue('satiRadPraznikNoc')}]",
              $zarade->radPraznikNoc(), "lev3");
      $output .= self::makeTableRowGt0("Зарада за прековремени рад [сати {$zarade->karnet->getValue('satiPrekovremeno')}]",
              $zarade->radPrekovremeno(), "lev3");
      $output .= self::makeTableRowGt0("Зарада за сменски рад [сати {$zarade->karnet->getValue('satiSmena')}]",
              $zarade->radSmena(), "lev3");
      $output .= self::makeTableRowGt0("Зарада за рад недељом [сати {$zarade->karnet->getValue('satiNedelja')}]",
              $zarade->radNedelja(), "lev3");
      $output .= self::makeTableRowGt0("Минули рад [{$zarade->karnet->getValue('koefMinuliRad')}%]", $zarade->minuliRad(), "lev3");
      return $output;
   }
   
   protected static function displayDrugaPrimanja(Zarade $zarade) {
      $output = "";
      $output .= self::makeTableRowGt0("Друга примања", $zarade->drugaPrimanja(), "lev1");
      $toDana = $zarade->karnet->getValue('daniTO');
      $output .= self::makeTableRowGt0("Топли оброк [дана {$toDana}]", $zarade->topliObrok(), "lev2");
      $output .= self::makeTableRowGt0("Регрес", $zarade->karnet->getValue('iznosRegres'), "lev2");
      $output .= self::makeTableRowGt0("Отпремнина", $zarade->karnet->getValue('iznosOtpremnina'), "lev2");
      $output .= self::makeTableRowGt0("Теренски додатак", $zarade->karnet->getValue('iznosTerenskiDodatak'), "lev2");
      $output .= self::makeTableRowGt0("Поклон деци", $zarade->karnet->getValue('iznosPoklonDeci'), "lev2");
      $output .= self::makeTableRowGt0("Остало [{$zarade->karnet->getValue('opisOstalo')}]", $zarade->karnet->getValue('iznosOstalo'), "lev2");
      return $output;
   }
}

class NaknadeRender extends PFERender {
   protected static $indent;
   /**
    * Stampa zarada pojedinacnog obracunskog listica u HTML kod.
    */
   public static function display(Naknade $naknade, $indent) {
      self::$indent = $indent;
      return self::makeTable($naknade);
   }

   protected static function makeTable(Naknade $naknade) {
      $output  = "";
      $output .= str_repeat("\t", self::$indent) . "<table class=\"listic-naknade\">\n";

      $output .= self::makeTableRow("2. НАКНАДЕ ЗАРАДЕ", $naknade->naknadeUkupno(), "lev0");

      $output .= self::displayNaknade1($naknade);
      $output .= self::displayBolovanja($naknade);
      $output .= self::displayOstalo($naknade);

      $output .= str_repeat("\t", self::$indent) . "</table> <!-- listic-naknade -->\n";
      return $output;
   }

   protected static function displayNaknade1(Naknade $naknade) {
      $output = "";
      $output .= self::makeTableRowGt0("Годишњи одмор [сати {$naknade->karnet->getValue('satiGO')}]",
              $naknade->naknadaGO(), "lev1");
      $output .= self::makeTableRowGt0("Плаћено одсуство [сати {$naknade->karnet->getValue('satiPlacenoOdsustvo')}]",
              $naknade->naknadaPlacenoOdsustvo(), "lev1");
      $output .= self::makeTableRowGt0("Одсуство на држ. празник [сати {$naknade->karnet->getValue('satiOdsustvoPraznik')}]",
              $naknade->naknadaOdsustvoPraznik(), "lev1");
      return $output;
   }

   protected static function displayBolovanja(Naknade $naknade) {
      $output = "";
      $output .= self::makeTableRowGt0("Боловање до 30 дана [сати {$naknade->karnet->getValue('satiBolDo30D')}]",
              $naknade->naknadaBolDo30D(), "lev1");
      $output .= self::makeTableRowGt0("Боловање до 30 дана - 100% [сати {$naknade->karnet->getValue('satiBolDo30D100Posto')}]",
              $naknade->naknadaBolDo30D100Posto(), "lev1");
      return $output;
   }
   
   protected static function displayOstalo(Naknade $naknade) {
      $output = "";
      $output .= self::makeTableRowGt0("Прекид рада без кривице запосленог [сати {$naknade->karnet->getValue('satiPrekid')}]",
              $naknade->naknadaPrekid(), "lev1");
      $output .= self::makeTableRowGt0("Војна вежба [сати {$naknade->karnet->getValue('satiVojnaVezba')}]",
              $naknade->naknadaVojnaVezba(), "lev1");
      $output .= self::makeTableRowGt0("Позив државног ограна [сати {$naknade->karnet->getValue('satiPozivDrzOrgana')}]",
              $naknade->naknadaPozivDrzOrgana(), "lev1");
      return $output;
   }
}

class PIDRender extends PFERender {
   protected static $indent;
   /**
    * Stampa zarada pojedinacnog obracunskog listica u HTML kod.
    */
   public static function display(Porez $porez, Doprinos $doprinos, $indent) {
      self::$indent = $indent;
      return self::makeTable($porez, $doprinos);
   }

   protected static function makeTable(Porez $porez, Doprinos $doprinos) {
      $output  = "";
      $output .= str_repeat("\t", self::$indent) . "<table class=\"listic-pid\">\n";
      $output .= str_repeat("\t", self::$indent + 1) . "<tr><th>Опис</th><th>Износ</th></tr>\n";

      $output .= self::makeTableRow("4. ПОРЕЗ И ДОПРИНОСИ ИЗ ЗАРАДЕ", $porez->porez() + $doprinos->doprinos(), "lev0");
      $output .= self::makePorezRows($porez);
      $output .= self::makeDoprinosRows($doprinos);
      $output .= self::makeDoprinosPoslRows($doprinos);
      
      $output .= str_repeat("\t", self::$indent) . "</table> <!-- listic-pid -->\n";
      return $output;
   }
   
   protected static function makePorezRows(Porez $porez) {
      $output  = "";

      $fstopa = number_format($porez->stopa(), 2, ",", ".");
      $fneoporezivo = number_format($porez->neoporezivo(), 2, ",", ".");
      $output .= self::makeTableRow("Порез {$fstopa}% [неопорезиво {$fneoporezivo}]", $porez->porez(), "lev1");

      return $output;
   }
   
   protected static function makeDoprinosRows(Doprinos $doprinos) {
      $output  = "";

      $fosnovica = number_format($doprinos->osnovicaDoprinosa(), 2, ",", ".");
      $fstopaUk = number_format($doprinos->payment->ukStopaDoprinosaZaposleni(), 2, ",", ".");
      $fstopaPIO = number_format($doprinos->payment->getValue('stopaPIO'), 2, ",", ".");
      $fstopaZdr = number_format($doprinos->payment->getValue('stopaZdravstvo'), 2, ",", ".");
      $fstopaNez = number_format($doprinos->payment->getValue('stopaNezaposlenost'), 2, ",", ".");
      
      $bullet = "&#8226;";
      $output .= self::makeTableRow("Доприноси {$fstopaUk}% [основица {$fosnovica}]", $doprinos->doprinos(), "lev1");
      $output .= self::makeTableRow("{$bullet} ПИО {$fstopaPIO}%", $doprinos->doprinosPIO(), "lev2");
      $output .= self::makeTableRow("{$bullet} за здравствено осигурање {$fstopaZdr}%", $doprinos->doprinosZdravstvo(), "lev2");
      $output .= self::makeTableRow("{$bullet} осигурање за незапосленост {$fstopaNez}%", $doprinos->doprinosNezaposlenost(), "lev2");
      
      return $output;
   }

   protected static function makeDoprinosPoslRows(Doprinos $doprinos) {
      $output  = "";

      // $fosnovica = number_format($doprinos->osnovicaDoprinosa(), 2, ",", ".");
      $fstopaUk = number_format($doprinos->payment->ukStopaDoprinosaPoslodavac(), 2, ",", ".");
      $fstopaPIO = number_format($doprinos->payment->getValue('stopaPIOPoslodavac'), 2, ",", ".");
      $fstopaZdr = number_format($doprinos->payment->getValue('stopaZdravstvoPoslodavac'), 2, ",", ".");
      $fstopaNez = number_format($doprinos->payment->getValue('stopaNezaposlenostPoslodavac'), 2, ",", ".");
      
      $bullet = "&#8226;";
      $output .= self::makeTableRow("5. Доприноси на зараду {$fstopaUk}%", $doprinos->doprinosPoslodavac(), "lev0");
      $output .= self::makeTableRow("{$bullet} ПИО {$fstopaPIO}%", $doprinos->doprinosPIOPoslodavac(), "lev2");
      $output .= self::makeTableRow("{$bullet} за здравствено осигурање {$fstopaZdr}%", $doprinos->doprinosZdravstvoPoslodavac(), "lev2");
      $output .= self::makeTableRow("{$bullet} осигурање за незапосленост {$fstopaNez}%", $doprinos->doprinosNezaposlenostPoslodavac(), "lev2");
      
      return $output;
   }
}

class NetoRender extends PFERender {
   protected static $indent;
   /**
    * Stampa zaglavlja pojedinacnog obracunskog listica u HTML kod.
    */
   public static function display(Neto $neto, $indent) {
      self::$indent = $indent;
      return self::makeTable($neto);
   }

   protected static function makeTable(Neto $neto) {
      $output  = "";
      $output .= str_repeat("\t", self::$indent) . "<table class=\"listic-neto\">\n";

      $netoUkupno = $neto->netoUkupno();
      $output .= self::makeTableRow("6. ИЗНОС НЕТО ЗАРАДЕ (3-4)", $netoUkupno, "lev0");
      $cUmanjCheck = NetoUmanjenjeRender::display($netoUkupno, $neto->ukUmanjenje, self::$indent);
      if ($cUmanjCheck === "") {
         $ukObustave = Obustave::ukupnoObustave($neto->eid, $neto->payment, $netoUkupno);
         $output .= self::makeTableRow("7. ОБУСТАВЕ УКУПНО", $ukObustave, "lev0");
         $output .= Obustave::display($neto->eid, $neto->payment, $netoUkupno, self::$indent);

         $output .= self::makeTableRow("8. НЕТО ЗА ИСПЛАТУ (6-7)", $neto->netoZaIsplatu($ukObustave), "lev0");

         $output .= str_repeat("\t", self::$indent) . "</table> <!-- listic-neto -->\n";
      } else {
         // $output .= NetoUmanjenjeRender::display($netoUkupno, $neto->ukUmanjenje, self::$indent);
         $output .= $cUmanjCheck;

         $ukObustave = Obustave::ukupnoObustave($neto->eid, $neto->payment, $netoUkupno);
         $output .= self::makeTableRow("8. ОБУСТАВЕ УКУПНО", $ukObustave, "lev0");
         $output .= Obustave::display($neto->eid, $neto->payment, $netoUkupno, self::$indent);

         $output .= self::makeTableRow("9. НЕТО ЗА ИСПЛАТУ (6-7-8)", $neto->netoZaIsplatu($ukObustave), "lev0");

         $output .= str_repeat("\t", self::$indent) . "</table> <!-- listic-neto -->\n";
      }
      return $output;
   }
}

class Obustave extends PFERender {
   protected static $indent;
   /**
    * Stampa zaglavlja pojedinacnog obracunskog listica u HTML kod.
    */
   public static function display($eid, Payment $payment, $netoUkupno, $indent) {
      self::$indent = $indent;
      return self::makeTable($eid, $payment, $netoUkupno);
   }

   protected static function makeTable($eid, Payment $payment, $netoUkupno) {
      $mesec = $payment->getValue('mesec');
      $godina = $payment->getValue('godina');
      $tipObracuna = $payment->getValue('tipObracuna');

      $nizObustava = DeductionDB::ucitajSveZaObracun($eid, $mesec, $godina, $tipObracuna);

      $output = "";
      foreach ($nizObustava as $obustava) {
         $rbr = 1;
         $tipObustave = $obustava->getValue('tipObustave');
         if($tipObustave === 1) {
            foreach ($obustava->rate as $rata) {
               if ($rata->getValue('mesec') === $mesec &&
                       $rata->getValue('godina') === $godina &&
                       $rata->getValue('tipObracuna') === $tipObracuna) {
                  $naziv = ShopFormElement::getName($obustava->getValue('radnjaID'));
                  $output .= self::makeTableRowGt0(
                          "{$naziv}: рата {$rbr} од {$obustava->getBrRata()}", $rata->getValue('iznos'), "lev1");
               }
               ++$rbr;
            }
         } else {
            foreach ($obustava->rate as $rata) {
               if ($rata->getValue('mesec') === $mesec &&
                       $rata->getValue('godina') === $godina &&
                       $rata->getValue('tipObracuna') === $tipObracuna) {
                  $naziv = ShopFormElement::getName($obustava->getValue('radnjaID'));
                  $iznos = self::getTip2Iznos($netoUkupno, $rata->getValue('iznos'));
                  $output .= self::makeTableRowGt0(
                     "{$naziv} [{$rata->getValue('iznos')}%]", $iznos, "lev1");
                     // "{$naziv}: [проценат од нето: {$netoUkupno}]", $iznos, "lev1");
               }
            }
         }
      }
      return $output;
   }
   
   /**
    * Ukupne obustave za tekuci obracun za datog zaposlenog.
    * @return float
    */
   public static function ukupnoObustave($eid, Payment $payment, $netoUkupno) {
      $mesec = $payment->getValue('mesec');
      $godina = $payment->getValue('godina');
      $tipObracuna = $payment->getValue('tipObracuna');

      $nizObustava = DeductionDB::ucitajSveZaObracun($eid, $mesec, $godina, $tipObracuna);
      return self::sumObustave($nizObustava, $mesec, $godina, $tipObracuna, $netoUkupno);
   }

   protected static function sumObustave($nizObustava, $mesec, $godina, $tipObracuna, $netoUkupno) {
      $ukObustave = 0;
      foreach ($nizObustava as $obustava) {
         $tipObustave = $obustava->getValue('tipObustave');
         if($tipObustave === 1) {
            foreach ($obustava->rate as $rata) {
               if ($rata->getValue('mesec') === $mesec && $rata->getValue('godina') === $godina &&
                       $rata->getValue('tipObracuna') === $tipObracuna) {
                  $ukObustave += $rata->getValue('iznos');
               }
            }
         } else {
            foreach ($obustava->rate as $rata) {
               if ($rata->getValue('mesec') === $mesec && $rata->getValue('godina') === $godina &&
                       $rata->getValue('tipObracuna') === $tipObracuna) {
                  $ukObustave += self::getTip2Iznos($netoUkupno, $rata->getValue('iznos'));
               }
            }
         }
      }
      return $ukObustave;
   }
   
   public static function getTip2Iznos($netoUkupno, $procenat) {
      return round($netoUkupno * $procenat / 100, 2);
   }
}

class NetoUmanjenjeRender extends PFERender {
   protected static $indent;

   public static function display($netouk, $umanjenje, $indent) {
      self::$indent = $indent;
      $output = "";
      if(!NetoUmanjenje::check()) {
         return $output;
      }

      // $desc = self::descriptionString($netouk);
      // $output .= self::makeTableRow("Умањени нето износ <br/>({$desc})", $netouk - $umanjenje, "lev1");
      $output .= self::makeTableRow("7. Умањење", $umanjenje, "lev1");

      return $output;
   }
   
   protected static function descriptionString($netouk) {
      if ($netouk <= 60000) {
         $objasnjenje = self::desclt60($netouk);
      } else {
         if ($netouk <= 100000) {
            $objasnjenje = self::desclt100($netouk);
         } else {
            $objasnjenje = self::descgt100($netouk);
         }
      }
      return $objasnjenje;
   }
   
   protected static function desclt60($netouk) {
      $mult = "&#215;";
      $fnetouk = number_format($netouk, 2, ",", ".");
      return "{$fnetouk} {$mult} 1";
   }
   
   protected static function desclt100($netouk) {
      $mult = "&#215;";
      $fnetouk = number_format($netouk - 60000, 2, ",", ".");
      return "60.000,00 {$mult} 1 + {$fnetouk} {$mult} 0.8";
   }
   
   protected static function descgt100($netouk) {
      $mult = "&#215;";
      $fnetouk = number_format($netouk - 100000, 2, ",", ".");
      return "60.000,00 {$mult} 1 + (100.000,00 - 60.000,00) {$mult} 0.8 + {$fnetouk} {$mult} 0.75";
   }
}
