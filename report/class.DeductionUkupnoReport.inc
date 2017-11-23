<?php
require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'shop/class.ShopDB.inc';
require_once 'deduction/class.DeductionDB.inc';
require_once 'class.ReportPage.inc';
require_once 'class.Virman.inc';
require_once 'report/class.PFE.inc';

class DeductionUkupnoReport extends ReportPage implements RenderHTML {
   protected $payment;
   protected $mesec;
   protected $godina;
   protected $tipObracuna;

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
      
      $this->sectionHeader("Картица обустава за радње");
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
      echo $this->backToListButton();
   }
   
   protected function makeTable(Shop $shop, $deductions) {
      $output = "";
      $output .= $this->makeZaglavlje($shop, 5);
      
      $output .= str_repeat("\t", self::$indent+2) . "<table class=\"lista-shop\">\n";
      $output .= str_repeat("\t", self::$indent+3) . "<tr><th>Име и презиме</th><th>Плаћено</th><th>За наплату</th><th>Укупно</th></tr>\n";
      
      $ukplaceno = 0;
      $uknaplata = 0;
      foreach ($deductions as $deduction) {
         $output .= $this->makeRow($deduction, $ukplaceno, $uknaplata);
      }
      
      $fuk_placeno = number_format($ukplaceno, 2,  ",", ".");
      $fuk_naplata = number_format($uknaplata, 2,  ",", ".");
      $fuk_pn = number_format($ukplaceno + $uknaplata, 2,  ",", ".");
      $output .= str_repeat("\t", self::$indent+3) . "<tr><td><strong>Укупно:</strong></td><td><strong>{$fuk_placeno}</strong></td>";
      $output .= "<td><strong>{$fuk_naplata}</strong></td>\n";
      $output .= "<td><strong>{$fuk_pn}</strong></td></tr>\n";
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
   
   protected function makeRow(Deduction $d, &$ukplac, &$uknapl) {
      $output = "";
      $employe = EmployeDB::load($d->getValue('zaposleniID'));
      $ime = $employe->getValue('ime') . " " . $employe->getValue('prezime');

      $placeno = 0;
      $zanapl = 0;
      $tipObustave = $d->getValue('tipObustave');

      // samo klasicna obustava - ne sindikat
      if($tipObustave === 1) {
         foreach ($d->rate as $rata) {
            if( ($rata->getValue('godina') < $this->godina) ||
                ($rata->getValue('godina') === $this->godina && $rata->getValue('mesec') <= $this->mesec) ) {
               
               // ako se uzima u obzir da li je rata na ak ili kon, onda u gornjem redu ne ide <= nego <, i dodaje se
               // || ($rata->getValue('godina') === $this->godina && $rata->getValue('mesec') === $this->mesec
               // && ($this->tipObracuna === "Konacni" || $rata->getValue('tipObracuna') === $this->tipObracuna) )) {
               $placeno += $rata->getValue('iznos');
            }
         }
         $zanapl = $d->getTotalAmount() - $placeno;

         if($zanapl > 0) {
            $placenof = number_format($placeno, 2,  ",", ".");
            $zanaplf = number_format($zanapl, 2,  ",", ".");
            $ukf = number_format($placeno + $zanapl, 2,  ",", ".");
            
            $ukplac += $placeno;
            $uknapl += $zanapl;
            $output .= str_repeat("\t", self::$indent+3) . "<tr><td>{$ime}</td><td>{$placenof}</td><td>{$zanaplf}</td><td>{$ukf}</td></tr>\n";
         }
      }
      return $output;
   }
}

