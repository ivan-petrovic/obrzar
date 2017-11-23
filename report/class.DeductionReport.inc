<?php
require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'shop/class.ShopDB.inc';
require_once 'deduction/class.DeductionDB.inc';
require_once 'class.ReportPage.inc';
require_once 'class.Virman.inc';
require_once 'report/class.PFE.inc';

class DeductionReport extends ReportPage implements RenderHTML {
   protected $payment;
   protected $mesec;
   protected $godina;
   protected $tipObracuna;
   protected $ukupno = 0;
   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $pid, $eid, $indent) {
      parent::__construct($op, $fid, $pid, $eid, $indent);
      $this->payment = PaymentDB::load($this->pid);
      $this->mesec = $this->payment->getValue('mesec');
      $this->godina = $this->payment->getValue('godina');
      $this->tipObracuna = $this->payment->getValue('tipObracuna');
   }
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Преглед обустава по радњама");
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $shops = ShopDB::loadAll();
      foreach ($shops as $shop) {
         $deductions = DeductionDB::loadAllForShop($shop->getValue('id'),
                 $this->mesec, $this->godina, $this->tipObracuna);
         $fded = array();
         foreach ($deductions as $deduction) {
            $employe = EmployeDB::load($deduction->getValue('zaposleniID'));
            if($employe->getValue('firmaID') === $this->fid) {
               $fded[] = $deduction;
            }
         }
         if(count($fded) > 0) {
            echo $this->makeTable($shop, $fded);
         }
      }
      $fukupno = number_format($this->ukupno, 2,  ",", ".");
      echo str_repeat("\t", self::$indent+2) . "<div class=\"ukupno\"><p>УКУПНО: {$fukupno}</p></div>\n";
      echo $this->backToListButton();
   }
   
   protected function makeTable(Shop $shop, $deductions) {
      $output = "";
      $output .= $this->makeZaglavlje($shop, 5);
      
      $output .= str_repeat("\t", self::$indent+2) . "<table class=\"lista-shop\">\n";
      $output .= str_repeat("\t", self::$indent+3) . "<tr><th>Име и презиме</th><th>Рата</th><th>Износ</th></tr>\n";
      $ukiznos = 0;
      foreach ($deductions as $deduction) {
         $output .= $this->makeRow($deduction, $ukiznos);
      }
      $this->ukupno += $ukiznos;
      $fukiznos = number_format($ukiznos, 2,  ",", ".");
      $output .= str_repeat("\t", self::$indent+3) . "<tr><td></td><td><strong>Укупно:</strong></td><td><strong>{$fukiznos}</strong></td></tr>\n";
      $output .= str_repeat("\t", self::$indent+2) . "</table>\n";
      $output .= str_repeat("\t", self::$indent+2) . "<hr>\n";
      
      return $output;
   }
   
   protected function makeZaglavlje(Shop $shop, $indent) {
      $output = "";
      
      $output .= str_repeat("\t", $indent) . "<div class=\"firma\">\n";
      $output .= str_repeat("\t", $indent) . "<h4>" . Application::getCurrFirmName() . "</h4>\n";
      $output .= str_repeat("\t", $indent) . "<p>"  . Application::getCurrFirmProp('currFirmAddress') . "</p>\n";
      $output .= str_repeat("\t", $indent) . "<p>"  . Application::getCurrFirmProp('currFirmCity') . "</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- firma -->\n";

      $output .= str_repeat("\t", $indent) . "<div class=\"radnja\">\n";
      $output .= str_repeat("\t", $indent+1) . "<h4>{$shop->getValue('naziv')}</h4>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>{$shop->getValue('adresa')}</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>{$shop->getValue('mesto')}</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- radnja -->\n";

      return $output;
   }
   
   protected function makeRow(Deduction $d, &$ukiznos) {
      $output = "";
      $employe = EmployeDB::load($d->getValue('zaposleniID'));
      $ime = $employe->getValue('ime') . " " . $employe->getValue('prezime');
      $brRata = count($d->rate);
      $rbr = 1;
      $tipObustave = $d->getValue('tipObustave');
      if($tipObustave === 1) {
         foreach ($d->rate as $rata) {
            if($rata->getValue('mesec') === $this->mesec && $rata->getValue('godina') === $this->godina &&
                    $rata->getValue('tipObracuna') === $this->tipObracuna) {
               $ukiznos += $rata->getValue('iznos');
               $iznos = number_format($rata->getValue('iznos'), 2,  ",", ".");
               $output .= str_repeat("\t", self::$indent+3) . "<tr><td>{$ime}</td><td>{$rbr} од {$brRata}</td><td>{$iznos}</td></tr>\n";
            }
            ++$rbr;
         }
      } else {
         foreach ($d->rate as $rata) {
            if ($rata->getValue('mesec') === $this->mesec && $rata->getValue('godina') === $this->godina &&
                    $rata->getValue('tipObracuna') === $this->tipObracuna) {
               $iznos = self::getTip2Iznos($d, $this->payment, $rata->getValue('iznos'));
               $fiznos = number_format($iznos, 2,  ",", ".");
               $ukiznos += $iznos;
               $output .= str_repeat("\t", self::$indent+3) . "<tr><td>{$ime}</td><td></td><td>{$fiznos}</td></tr>\n";
            }
         }
      }
      return $output;
   }
   
   public static function getTip2Iznos(Deduction $obustava, Payment $payment,$procenat) {
      $eid = $obustava->getValue('zaposleniID');
      $employe = EmployeDB::load($eid);
      $karnet = KarnetDB::load1($eid, $payment->getValue('id'));
      if($karnet === false) {
         return 0;
      }
      $pfe = new PaymentForEmploye($employe, $payment, $karnet);
      $netoUkupno = self::getNeto($pfe, 'netoUkupno');
      return round($netoUkupno * $procenat / 100, 2);
   }
   
   public static function getNeto(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaNeto->$fname();
      } else {
         return $pfe->konacniNeto->$fname();
      }
   }
}

class DeductionVirmanReport extends ReportPage implements RenderHTML {
   protected $payment;
   protected $mesec;
   protected $godina;
   protected $tipObracuna;
   protected $obrazacBroj = "1";
   
   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $pid, $eid, $indent) {
      parent::__construct($op, $fid, $pid, $eid, $indent);
      $this->payment = PaymentDB::load($this->pid);
      $this->mesec = $this->payment->getValue('mesec');
      $this->godina = $this->payment->getValue('godina');
      $this->tipObracuna = $this->payment->getValue('tipObracuna');
   }
   
   public function renderHTML() {
      $this->openSection();

      $this->sectionHeader("Преглед налога за пренос за обуставе");
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      }
      echo $this->makeForm();

      $shops = ShopDB::loadAll();
      foreach ($shops as $shop) {
         $deductions = DeductionDB::loadAllForShop($shop->getValue('id'),
                 $this->mesec, $this->godina, $this->tipObracuna);
         $fded = array();
         foreach ($deductions as $deduction) {
            $employe = EmployeDB::load($deduction->getValue('zaposleniID'));
            if($employe->getValue('firmaID') === $this->fid) {
               $fded[] = $deduction;
            }
         }
         if(count($fded) > 0) {
            echo $this->makeVirman($shop, $fded);
         }
      }
      echo $this->backToListButton();
   }
   
   protected function makeForm() {
      $output  = "";
      $scriptName = "report.php?opID=" . $this->op;
      $output .= FormMaker::openForm($scriptName, 5);

      $output .= FormMaker::textElement('Образац број', "obrazacbroj", $this->obrazacBroj, self::$indent+3);
      $output .= FormMaker::submitElement('Примени', 6);
      
      $output .= FormMaker::closeForm(5);
      return $output;
   }

   protected function processForm() {
      $this->obrazacBroj = filter_input(INPUT_POST, 'obrazacbroj', FILTER_SANITIZE_STRING);
   }
   
   protected function makeVirman(Shop $shop, $deductions) {
      // $output = str_repeat("\t", self::$indent + 2) . "<h4>{$shop->getValue('naziv')}</h4>\n";
      $output = "";
      $ukIznos = $this->calcTotal($deductions);
      $output .= $this->makeOneVirman($shop, $ukIznos);
      return $output;
   }

   protected function calcTotal($deductions) {
      $uk = 0;
      foreach ($deductions as $d) {
         $tipObustave = $d->getValue('tipObustave');
         if($tipObustave === 1) {
            foreach ($d->rate as $rata) {
               if($rata->getValue('mesec') === $this->mesec && $rata->getValue('godina') === $this->godina &&
                       $rata->getValue('tipObracuna') === $this->tipObracuna) {
                  $uk += $rata->getValue('iznos');
               }
            }
         } else {
            foreach ($d->rate as $rata) {
               if ($rata->getValue('mesec') === $this->mesec && $rata->getValue('godina') === $this->godina &&
                       $rata->getValue('tipObracuna') === $this->tipObracuna) {
                  $iznos = self::getTip2Iznos($d, $this->payment, $rata->getValue('iznos'));
                  $uk += $iznos;
               }
            }
         }
      }
      return $uk;
   }

   protected function makeOneVirman(Shop $shop, $ukiznos) {
      $output = "";
      $poverilac = $shop->getValue('naziv') . ", " . $shop->getValue('adresa');
      $poverilac .=  ", " . $shop->getValue('mesto');
      
      $v = new Virman();
      $vdata = array(
          'duznik' => Application::getCurrFirmName(),
          'svrha' => $this->getSvrha(),
          'poverilac' => $poverilac,
          'mesto' => Application::getCurrFirmProp('currFirmCity'),
          'datum' => $this->getDatum($this->payment->getValue('datumIsplate')),
          'sifraPlacanja' => '241',
          'valuta' => 'РСД',
          'iznos' => number_format($ukiznos, 2, ",", "."),
          'racunDuznika' => Application::getCurrFirmProp('currFirmAccountNum'),
          'racunPoverioca' => $shop->getValue('ziroracun'),
          'obrazacBr' => $this->obrazacBroj
      );
      $v->setData($vdata);
      $output .= VirmanRenderer::display($v, 5);

      return $output;
   }

   protected function getDatum($datum) {
      if($datum === '0000-00-00') {
         $datumString = '';
      } else {
         $di = new DateTime($datum);
         $datumString = $di->format("j.n.Y");
      }
      return $datumString;
   }
   
   protected function getSvrha() {
      return "ОБУСТАВЕ ИЗ ЗАРАДА";
   }

   public static function getTip2Iznos(Deduction $obustava, Payment $payment,$procenat) {
      $eid = $obustava->getValue('zaposleniID');
      $employe = EmployeDB::load($eid);
      $karnet = KarnetDB::load1($eid, $payment->getValue('id'));
      if($karnet === false) {
         return 0;
      }
      $pfe = new PaymentForEmploye($employe, $payment, $karnet);
      $netoUkupno = self::getNeto($pfe, 'netoUkupno');
      return round($netoUkupno * $procenat / 100, 2);
   }
   
   public static function getNeto(PaymentForEmploye $pfe, $fname) {
      if(!$pfe->flagKon) {
         return $pfe->akontacijaNeto->$fname();
      } else {
         return $pfe->konacniNeto->$fname();
      }
   }
}
