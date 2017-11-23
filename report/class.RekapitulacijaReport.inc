<?php
require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'class.ReportPage.inc';
require_once 'class.PFE.inc';
require_once 'class.PFERender.inc';

class RekapitulacijaReport extends ReportPage implements RenderHTML {
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Рекапитулација за " . Application::getCurrPaymentName());
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $payment = PaymentDB::load($this->pid);
      $nizKarnet = KarnetDB::loadAll($this->pid);
      
      $nizPFE = array();
      foreach ($nizKarnet as $karnet) {
         $employe = EmployeDB::load($karnet->getValue('zaposleniID'));
         $nizPFE[] = new PaymentForEmploye($employe, $payment, $karnet);
      }

      echo RecapRender::display($nizPFE, 5);
      echo $this->backToListButton();
   }
}

class Recap {
   protected static $indent;
   // Ova dva polja postavljaju RecapZaradeRender i RecapNaknadeRender
   protected static $ukZarade;
   protected static $ukNaknade;
   // Ova dva polja postavljaju se u RecapPIDRender
   protected static $ukPorez;
   protected static $ukDoprinos;

   /**
    * Stampa pojedinacnog reda tabele. Formira se red u tabeli
    * bez obzira da li je vrednost 0.
    * KOPIRANA FUNKCIJA IZ PFERender.
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
    * KOPIRANA FUNKCIJA IZ PFERender.
    * @param string $label Opis polja.
    * @param float $value Vrednost polja.
    * @param string $pad klasa koja se dodaje za potrebe stilizovanja reda u tabeli.
    */
   protected static function makeTableRowGt0($label, $value, $pad = "") {
      if($value > 0) {
         return self::makeTableRow($label, $value, $pad);
      }
      return "";
   }

   public static function getUkupno($nizPFE, $className, $fname) {
      $uk = 0;
      $method = 'get' . $className;
      foreach ($nizPFE as $pfe) {
         $uk += self::$method($pfe, $fname);
      }
      return $uk;
   }
   
   public static function getZarade(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaZarade->$fname();
      } else {
         return $pfe->konacniZarade->$fname();
      }
   }

   public static function getKarnet(PaymentForEmploye $pfe, $fname) {
      return $pfe->karnet->$fname();
   }
   
   public static function getKarnetValue(PaymentForEmploye $pfe, $vname) {
      return $pfe->karnet->getValue($vname);
   }

   public static function getNaknade(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaNaknade->$fname();
      } else {
         return $pfe->konacniNaknade->$fname();
      }
   }

   public static function getPorez(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaPorez->$fname();
      } else {
         return $pfe->konacniPorez->$fname();
      }
   }
   
   public static function getDoprinos(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaDoprinos->$fname();
      } else {
         return $pfe->konacniDoprinos->$fname();
      }
   }

   public static function getPaymentValue(PaymentForEmploye $pfe, $vname) {
      return $pfe->payment->getValue($vname);
   }
   
   public static function getNeto(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaNeto->$fname();
      } else {
         return $pfe->konacniNeto->$fname();
      }
   }
}

class RecapRender extends Recap {
   public static function display($nizPFE, $indent) {
      self::$indent = $indent;
      $output  = "";
      $output .= str_repeat("\t", self::$indent) . "<table class=\"rekapitulacija\">\n";
      $output .= str_repeat("\t", self::$indent + 1) . "<tr><th>Опис</th><th>Износ</th></tr>\n";
      
      $output .= self::displayZIN($nizPFE);
      $output .= RecapPIDRender::display($nizPFE);
      $output .= RecapNetoRender::display($nizPFE);
      
      $output .= str_repeat("\t", self::$indent) . "</table>\n";
      return $output;
   }
   
   public static function displayZIN($nizPFE) {
      $output  = "";
      $output .= RecapZaradeRender::display($nizPFE);
      $output .= RecapNaknadeRender::display($nizPFE);
      $output .= self::makeTableRow("УКУПНО ЗАРЕДЕ И НАКНАДЕ", self::$ukZarade + self::$ukNaknade, "lev0");
      return $output;
   }
}

class RecapZaradeRender extends Recap {
   public static function display($nizPFE) {
      $output  = "";
      
      self::$ukZarade = self::getUkupno($nizPFE, 'Zarade', 'zaradaUkupno');
      $output .= self::makeTableRow("ЗАРАДА УКУПНО", self::$ukZarade, "lev0");
      $output .= self::displayZaradaZaObavljeniRad($nizPFE);
      $output .= self::makeTableRowGt0("Зарада за допринос пословном успеху",
              self::getUkupno($nizPFE, 'Zarade', 'zaradaDoprinosPoslovnomUspehu'), "lev1");
      $output .= self::displayDrugaPrimanja($nizPFE);
      
      return $output;
   }

   protected static function displayZaradaZaObavljeniRad($nizPFE) {
      $output = "";
      $output .= self::makeTableRow("Зарада за обављени рад", self::getUkupno($nizPFE, 'Zarade', 'zaradaZaObavljeniRad'), "lev1");
      $sati = self::getUkupno($nizPFE, 'Karnet', 'radniSatiOsnovnaZarada');
      $output .= self::makeTableRowGt0("Основна зарада [сати {$sati}]", self::getUkupno($nizPFE, 'Zarade', 'osnovnaZarada'), "lev2");
      $output .= self::makeTableRowGt0("Зарада за радни учинак", self::getUkupno($nizPFE, 'Zarade', 'zaradaZaRadniUcinak'), "lev2");
      $output .= self::displayUvecanaZarada1($nizPFE);
      $output .= self::displayUvecanaZarada2($nizPFE);
      return $output;
   }
   
   protected static function displayUvecanaZarada1($nizPFE) {
      $output = "";
      $output .= self::makeTableRowGt0("Увећана зарада", self::getUkupno($nizPFE, 'Zarade', 'uvecanaZarada'), "lev2");
      
      $sati1 = self::getUkupno($nizPFE, 'KarnetValue', 'satiRadPraznik');
      $output .= self::makeTableRowGt0("Зарада за рад на празник [сати {$sati1}]", self::getUkupno($nizPFE, 'Zarade', 'radNaPraznik'), "lev3");
      
      $sati2 = self::getUkupno($nizPFE, 'KarnetValue', 'satiRadNoc');
      $output .= self::makeTableRowGt0("Зарада за рад ноћу и рад по сменама [сати {$sati2}]", self::getUkupno($nizPFE, 'Zarade', 'radNocu'), "lev3");
      
      return $output;
   }
   
   protected static function displayUvecanaZarada2($nizPFE) {
      $output = "";
      
      $sati1 = self::getUkupno($nizPFE, 'KarnetValue', 'satiRadPraznikNoc');
      $output .= self::makeTableRowGt0("Зарада за ноћни рад и рад по сменама на празник [сати {$sati1}]",
              self::getUkupno($nizPFE, 'Zarade', 'radPraznikNoc'), "lev3");
      
      $sati2 = self::getUkupno($nizPFE, 'KarnetValue', 'satiPrekovremeno');
      $output .= self::makeTableRowGt0("Зарада за прековремени рад [сати {$sati2}]", self::getUkupno($nizPFE, 'Zarade', 'radPrekovremeno'), "lev3");
      
      $output .= self::makeTableRowGt0("Минули рад", self::getUkupno($nizPFE, 'Zarade', 'minuliRad'), "lev3");
      return $output;
   }
   
   protected static function displayDrugaPrimanja($nizPFE) {
      $output = "";
      $output .= self::makeTableRowGt0("Друга примања", self::getUkupno($nizPFE, 'Zarade', 'drugaPrimanja'), "lev1");
      
      $toDana = self::getUkupno($nizPFE, 'KarnetValue', 'daniTO');
      $output .= self::makeTableRowGt0("Топли оброк [дана {$toDana}]", self::getUkupno($nizPFE, 'Zarade', 'topliObrok'), "lev2");
      $output .= self::makeTableRowGt0("Регрес", self::getUkupno($nizPFE, 'KarnetValue', 'iznosRegres'), "lev2");
      $output .= self::makeTableRowGt0("Отпремнина", self::getUkupno($nizPFE, 'KarnetValue', 'iznosOtpremnina'), "lev2");
      $output .= self::makeTableRowGt0("Теренски додатак", self::getUkupno($nizPFE, 'KarnetValue', 'iznosTerenskiDodatak'), "lev2");
      $output .= self::makeTableRowGt0("Поклон деци", self::getUkupno($nizPFE, 'KarnetValue', 'iznosPoklonDeci'), "lev2");
      $output .= self::makeTableRowGt0("Остало", self::getUkupno($nizPFE, 'KarnetValue', 'iznosOstalo'), "lev2");
      return $output;
   }
}

class RecapNaknadeRender extends Recap {

   public static function display($nizPFE) {
      $output = "";
      
      self::$ukNaknade = self::getUkupno($nizPFE, 'Naknade', 'naknadeUkupno');
      $output .= self::makeTableRow("НАКНАДЕ УКУПНО", self::$ukNaknade, "lev0");

      $output .= self::displayNaknade1($nizPFE);
      $output .= self::displayBolovanja($nizPFE);
      $output .= self::displayOstalo($nizPFE);
      
      return $output;
   }

   protected static function displayNaknade1($nizPFE) {
      $output = "";
      
      $sati1 = self::getUkupno($nizPFE, 'KarnetValue', 'satiGO');
      $output .= self::makeTableRowGt0("Годишњи одмор [сати {$sati1}]", self::getUkupno($nizPFE, 'Naknade', 'naknadaGO'), "lev2");
      
      $sati2 = self::getUkupno($nizPFE, 'KarnetValue', 'satiPlacenoOdsustvo');
      $output .= self::makeTableRowGt0("Плаћено одсуство [сати {$sati2}]",
              self::getUkupno($nizPFE, 'Naknade', 'naknadaPlacenoOdsustvo'), "lev2");
      
      $sati3 = self::getUkupno($nizPFE, 'KarnetValue', 'satiOdsustvoPraznik');
      $output .= self::makeTableRowGt0("Одсуство на држ. празник [сати {$sati3}]",
              self::getUkupno($nizPFE, 'Naknade', 'naknadaOdsustvoPraznik'), "lev2");
      return $output;
   }

   protected static function displayBolovanja($nizPFE) {
      $output = "";
      
      $sati1 = self::getUkupno($nizPFE, 'KarnetValue', 'satiBolDo30D');
      $output .= self::makeTableRowGt0("Боловање до 30 дана [сати {$sati1}]",
              self::getUkupno($nizPFE, 'Naknade', 'naknadaBolDo30D'), "lev2");
      
      $sati2 = self::getUkupno($nizPFE, 'KarnetValue', 'satiBolDo30D100Posto');
      $output .= self::makeTableRowGt0("Боловање до 30 дана - 100% [сати {$sati2}]",
              self::getUkupno($nizPFE, 'Naknade', 'naknadaBolDo30D100Posto'), "lev2");
      return $output;
   }
   
   protected static function displayOstalo($nizPFE) {
      $output = "";
      
      $sati1 = self::getUkupno($nizPFE, 'KarnetValue', 'satiPrekid');
      $output .= self::makeTableRowGt0("Прекид рада без кривице запосленог [сати {$sati1}]",
              self::getUkupno($nizPFE, 'Naknade', 'naknadaPrekid'), "lev2");
      
      $sati2 = self::getUkupno($nizPFE, 'KarnetValue', 'satiVojnaVezba');
      $output .= self::makeTableRowGt0("Војна вежба [сати {$sati2}]", self::getUkupno($nizPFE, 'Naknade', 'naknadaVojnaVezba'), "lev2");
      
      $sati3 = self::getUkupno($nizPFE, 'KarnetValue', 'satiPozivDrzOrgana');
      $output .= self::makeTableRowGt0("Позив државног ограна [сати {$sati3}]",
              self::getUkupno($nizPFE, 'Naknade', 'naknadaPozivDrzOrgana'), "lev2");
      return $output;
   }
}

class RecapPIDRender extends Recap {
   public static function display($nizPFE) {
      $output  = "";
      
      $output .= self::makePorezRows($nizPFE);
      $output .= self::makeDoprinosRows($nizPFE);
      // $output .= self::makeTableRow("ПОРЕЗ И ДОПРИНОС УКУПНО", self::$ukPorez + self::$ukDoprinos, "lev0");
      
      return $output;
   }

   protected static function makePorezRows($nizPFE) {
      $output  = "";

      self::$ukPorez = self::getUkupno($nizPFE, 'Porez', 'porez');
      $output .= self::makeTableRow("Порез", self::$ukPorez, "lev0");
      $output .= self::makeTableRow("Пореска олакшица", self::getUkupno($nizPFE, 'Porez', 'neoporezivo'), "lev2");

      return $output;
   }
   
   protected static function makeDoprinosRows($nizPFE) {
      $output  = "";

      $osnovica = self::getUkupno($nizPFE, 'Doprinos', 'osnovicaDoprinosa');
      $output .= self::makeDoprinosRowsZap($nizPFE, $osnovica);
      $output .= self::makeDoprinosRowsPosl($nizPFE, $osnovica);

      return $output;
   }
   
   protected static function makeDoprinosRowsZap($nizPFE, $osnovica) {
      $output  = "";
      $num = count($nizPFE);
      $fosnovica = number_format($osnovica, 2, ",", ".");

      $stopaPIO = $num > 0 ? self::getPaymentValue($nizPFE[0], 'stopaPIO') : 0;
      $stopaZdr = $num > 0 ? self::getPaymentValue($nizPFE[0], 'stopaZdravstvo') : 0;
      $stopaNez = $num > 0 ? self::getPaymentValue($nizPFE[0], 'stopaNezaposlenost') : 0;
      $stopaUk = $stopaPIO + $stopaZdr + $stopaNez;
      
      $fstopaPIO = number_format($stopaPIO, 2, ",", ".");
      $fstopaZdr = number_format($stopaZdr, 2, ",", ".");
      $fstopaNez = number_format($stopaNez, 2, ",", ".");
      $fstopaUk = number_format($stopaUk, 2, ",", ".");

      self::$ukDoprinos = self::getUkupno($nizPFE, 'Doprinos', 'doprinos');
      $bullet = "&#8226;";
      $output .= self::makeTableRow("Допринос на терет запосленог {$fstopaUk}% [основица {$fosnovica}]", self::$ukDoprinos, "lev0");
      $output .= self::makeTableRow("{$bullet} ПИО {$fstopaPIO}%", self::getUkupno($nizPFE, 'Doprinos', 'doprinosPIO'), "lev2");
      $output .= self::makeTableRow("{$bullet} за здравствено осигурање {$fstopaZdr}%",
              self::getUkupno($nizPFE, 'Doprinos', 'doprinosZdravstvo'), "lev2");
      $output .= self::makeTableRow("{$bullet} осигурање за незапосленост {$fstopaNez}%",
              self::getUkupno($nizPFE, 'Doprinos', 'doprinosNezaposlenost'), "lev2");
      
      return $output;
   }
   
   protected static function makeDoprinosRowsPosl($nizPFE, $osnovica) {
      $output  = "";
      $num = count($nizPFE);
      $fosnovica = number_format($osnovica, 2, ",", ".");

      $stopaPIO = $num > 0 ? self::getPaymentValue($nizPFE[0], 'stopaPIOPoslodavac') : 0;
      $stopaZdr = $num > 0 ? self::getPaymentValue($nizPFE[0], 'stopaZdravstvoPoslodavac') : 0;
      $stopaNez = $num > 0 ? self::getPaymentValue($nizPFE[0], 'stopaNezaposlenostPoslodavac') : 0;
      $stopaUk = $stopaPIO + $stopaZdr + $stopaNez;
      
      $fstopaPIO = number_format($stopaPIO, 2, ",", ".");
      $fstopaZdr = number_format($stopaZdr, 2, ",", ".");
      $fstopaNez = number_format($stopaNez, 2, ",", ".");
      $fstopaUk = number_format($stopaUk, 2, ",", ".");
      
      $bullet = "&#8226;";
      $output .= self::makeTableRow("Допринос на терет послодавца {$fstopaUk}% [основица {$fosnovica}]",
              self::getUkupno($nizPFE, 'Doprinos', 'doprinosPoslodavac'), "lev0");
      $output .= self::makeTableRow("{$bullet} ПИО {$fstopaPIO}%",
              self::getUkupno($nizPFE, 'Doprinos', 'doprinosPIOPoslodavac'), "lev2");
      $output .= self::makeTableRow("{$bullet} за здравствено осигурање {$fstopaZdr}%",
              self::getUkupno($nizPFE, 'Doprinos', 'doprinosZdravstvoPoslodavac'), "lev2");
      $output .= self::makeTableRow("{$bullet} осигурање за незапосленост {$fstopaNez}%",
              self::getUkupno($nizPFE, 'Doprinos', 'doprinosNezaposlenostPoslodavac'), "lev2");
      
      return $output;
   }
}

class RecapNetoRender extends Recap {
   public static function display($nizPFE) {
      $output  = "";
      
      $ukNeto = self::getUkupno($nizPFE, 'Neto', 'netoUkupno');
      $ukUmanjenje = self::getUkupno($nizPFE, 'Neto', 'umanjenje');
              
      $output .= self::makeTableRow("НЕТО УКУПНО", $ukNeto, "lev0");
      // ako se radi o javnom preduzecu
      if(Application::getCurrFirmProp('currFirmType') === 1 || Application::getCurrFirmProp('currFirmType') === 3) {
         $output .= self::makeTableRow("Умањени нето", $ukNeto - $ukUmanjenje, "lev0");
         $output .= self::makeTableRow("Нето умањење", $ukUmanjenje, "lev0");
      }
      
      return $output;
   }
}
/*
$output .= self::makeTableRowGt0("Укупно обуставе", $recap->ukupnoObustave);

class Recapitulation {
   public $ukupnoObustave = 0.0;
   
   public function init($nizPFE) {
      foreach ($nizPFE as $pfe) {
         $this->ukupnoObustave += $pfe->ukupnoObustave();
      }
   }
}
*/
