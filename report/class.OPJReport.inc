<?php

require_once 'payment/class.PaymentDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'class.Zbir.inc';
require_once 'class.ZiroRacun.inc';
require_once 'class.ReportPage.inc';
require_once 'report/class.PFE.inc';
require_once 'report/class.PFERender.inc';

class OPJReport extends ReportPage implements RenderHTML {
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("ПП ОПЈ");
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

      $this->display($nizPFE, $payment, 5);
      echo $this->backToListButton();
   }
   
   protected function display($nizPFE, Payment $payment, $indent) {
      echo OPJZaglavlje::display($payment, $indent);
      echo OPJTable::display($nizPFE, $payment, $indent);
      echo OPJFuter::display($payment, $indent);
   }
}

class OPJZaglavlje {
   public static function display(Payment $payment, $indent) {
      $output  = str_repeat("\t", $indent) . "<div class=\"ppopj\">\n";
      $output .= self::goreLevo($indent + 1);
      $output .= self::goreDesno($indent + 1);
      $output .= self::uplatniRacun($indent + 1);
      $output .= self::naslov($indent + 1, $payment->getMonthName(), $payment->getValue('godina'));
      $output .= self::datumIsplate($indent + 1, $payment->getValue('datumIsplate'));
      return $output;
   }

   protected static function goreLevo($indent) {
      $output  = str_repeat("\t", $indent) . "<div class=\"gore-levo\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p>Исплатилац " . Application::getCurrFirmName() . "</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>Седиште исплатиоца " . Application::getCurrFirmProp('currFirmAddress') . ", ";
      $output .= Application::getCurrFirmProp('currFirmCity') . "</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>ПИБ " . Application::getCurrFirmProp('currFirmPIB') . "</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>Матични број " . Application::getCurrFirmProp('currFirmMatbr') . "</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>Шифра делатности исплатиоца " . Application::getCurrFirmProp('currFirmDel') . "</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- gore-levo -->\n";
      return $output;
   }

   protected static function goreDesno($indent) {
      $output  = str_repeat("\t", $indent) . "<div class=\"gore-desno\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"podnaslov\">Република Србија</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"podnaslov\">Министарство финансија и привреде</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"podnaslov\">Пореска управа</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>Организациона јединица _________________</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>Потврда о пријему:</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- gore-desno -->\n";
      return $output;
   }

   protected static function uplatniRacun($indent) {
      $tipFirme = Application::getCurrFirmProp('currFirmType');
      $zr = ZiroRacun::getZR($tipFirme, ZiroRacun::ZR_POREZ);
      
      $output  = str_repeat("\t", $indent) . "<div class=\"upl-racun\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p>Уплатни рачун пореза на зараде</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>{$zr}</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- upl-racun -->\n";
      return $output;
   }
   
   protected static function naslov($indent, $mesec, $godina) {
      $output  = str_repeat("\t", $indent);
      $output .= "<p class=\"glavni-naslov\">Пореска пријава о обрачунатом<br/> и плаћеном порезу на зараде<br/>";
      $output .= "<span>за месец {$mesec} ";
      $output .= "(редни број исплате ____)";
      $output .= "{$godina}. године</span></p>\n";
      return $output;
   }

   protected static function datumIsplate($indent, $datum) {
      $output  = str_repeat("\t", $indent) . "<div class=\"datum-isplate\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p><strong>Исплата извршена: </strong>";
      if($datum == '0000-00-00') {
         $output .= "___________________</p>\n";
      } else {
         $di = new DateTime($datum);
         $output .= $di->format("j.n.Y") . ". године</p>\n";
      }
      $output .= str_repeat("\t", $indent) . "</div> <!-- datum-isplate -->\n";
      return $output;
   }
}

class OPJTable {
   public static function display($nizPFE, Payment $payment, $indent) {
      $r_1_3 = count($nizPFE);
      $r_1_4 = round(ZBIR::brutoUkupno($nizPFE));
      $r_21_4 = round(ZBIR::olaksicaUkupno($nizPFE));
      $r_2_4 = $r_21_4;
      $r_3_4 = $r_1_4 - $r_2_4;
      $r_5_4 = round($r_3_4 * $payment->getValue('stopaPoreza') / 100);
      $r_8_4 = $r_5_4;
      $r_15_4 = $r_8_4;

      $opjdesc = array(
          array("I", "ЗАРАДА И ДРУГА ПРИМАЊА ЗАПОСЛЕНИХ", 0, 0),
          array("1.", "Исплаћена зарада запосленима-основица пореза (члан 13. став 1. Закона", $r_1_3, $r_1_4),
          array("2.", "Износ зараде ослобођен плаћања пореза (2.1 + 2.2 + 2.3)", $r_1_3, $r_2_4),
          array("2.1.", "Пореско ослобођење од {$payment->getValue('iznosNeoporezivo')} динара (члан 15а став 2. Закона)", $r_1_3, $r_21_4),
          array("2.2.", "Премија за добровољно здравствено осигурање, односно пензијски допринос у добровољни пензијски фонд (Члан 21а Закона)", 0, 0),
          array("2.3.", "Зарада особа са инвалидитетом запослених у предузећу за радно оспособљавање и запошљавање особа са инвалидитетом (Члан 21. Закона",
              0, 0),
          array("3.", "Опорезиви износ зараде - основица пореза (1 - 2)", $r_1_3, $r_3_4),
          array("4.", "Остала примања запослених - основица пореза (чл. 14. до 14б Закона)", 0, 0),
          array("5.", "Обрачунати порез на зараде запослених (3 + 4) x {$payment->getValue('stopaPoreza')}%", $r_1_3, $r_5_4),
          array("6.", "Умањење обрачунатог пореза по основу новозапослених радника (6.1 + 6.2 + 6.3)", 0, 0),
          array("6.1.", "Умањење обрачунатог пореза по основу члана 21в Закона", 0, 0),
          array("6.2.", "Умањење обрачунатог пореза по основу члана 21г Закона", 0, 0),
          array("6.3.", "Умањење обрачунатог пореза по основу члана 21д Закона", 0, 0),
          array("7.", "Порез на зараде плаћен у другој држави", 0, 0),
          array("8.", "Плаћени порез на зараде запослених (5 - 6 - 7)", $r_1_3, $r_8_4),
          array("II", "УГОВОРЕНА НАКНАДА И ДРУГА ПРИМАЊА ЗА ПРИВРЕМЕНЕ И ПОВРЕМЕНЕ ПОСЛОВЕ", 0, 0),
          array("9.", "Зарада - уговорена накнада и друга примања за привремене и повремене послове - основица пореза (члан 13. став 2. Закона)", 0, 0),
          array("10.", "Обрачунати и плаћени порез на зараде за привремене и повремене послове (9 x _____%)", 0, 0),
          array("III", "ЛИЧНА ЗАРАДА ПРЕДУЗЕТНИКА", 0, 0),
          array("11.", "Исплаћена лична зарада предузетника (члан 13. став 3. Закона)", 0, 0),
          array("12.", "Износ личне зараде ослобођен плаћања пореза (12.1 + 12.2)", 0, 0),
          array("12.1.", "Пореско ослобођење од ________ динара (члан 15а став 2. Закона)", 0, 0),
          array("12.2.", "Премија за добровољно здравствено осигурање, односно пензијски допринос у добровољни пензијски фонд (члан 21а Закона)", 0, 0),
          array("13.", "Опорезиви износ личне зараде - основица пореза (11 - 12)", 0, 0),
          array("14.", "Обрачунати порез на личну зараду предузетника (13 x _____%)", 0, 0),
          array("IV", "УКУПНО ПЛАЋЕНИ ПОРЕЗ", 0, 0),
          array("15.", "Укупно плаћени порез на зараде (8 + 10 + 14)", $r_1_3, $r_15_4)
      );

      $output  = self::tabelaUputstvo($indent+1);
      $output .= self::tabelaZaglavlje($indent+1);
      foreach ($opjdesc as $red) {
         $output .= self::makeRow($red, $indent+1);
      }
      $output .= self::tabelaFuter($indent+1);
      return $output;
   }

   protected static function tabelaUputstvo($indent) {
      $output  = str_repeat("\t", $indent) . "<div class=\"glavni-sadrzaj\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"tabela-uputstvo\">Уносе се цели бројеви, без децимала</p>\n";
      return $output;
   }

   protected static function tabelaZaglavlje($indent) {
      $output  = str_repeat("\t", $indent+1) . "<table>\n";
      $output .= str_repeat("\t", $indent+1) . "<thead>\n";
      $output .= str_repeat("\t", $indent+1) . "<tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>Ред.бр.</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>О П И С</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>Број запослених/<br/>ангажованих лица</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>Износ</th>\n";
      $output .= str_repeat("\t", $indent+1) . "</tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<tr><th>1</th><th>2</th><th>3</th><th>4</th></tr>\n";
      $output .= str_repeat("\t", $indent+1) . "</thead>\n";
      
      $output .= str_repeat("\t", $indent+1) . "<tbody>\n";
      return $output;
   }

   protected static function tabelaFuter($indent) {
      $output  = str_repeat("\t", $indent+2) . "</tbody>\n";
      $output .= str_repeat("\t", $indent+1) . "</table>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- glavni-sadrzaj -->\n";
      return $output;
   }
   
   protected static function makeRow($red, $indent) {
      $output  = str_repeat("\t", $indent+1) . "<tr>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col1\">{$red[0]}</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col2\">{$red[1]}</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col3\">" . ($red[2] > 0 ? number_format($red[2], 0, ",", "."): "") . "</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col4\">" . ($red[3] > 0 ? number_format($red[3], 0, ",", "."): "") . "</td>\n";
      $output .= str_repeat("\t", $indent+1) . "</tr>\n";
      return $output;
   }
}

class OPJFuter {
   public static function display(Payment $payment, $indent) {
      $output  = self::futer1($indent + 1, $payment->getValue('datumIsplate'));
      $output .= self::futer2($indent + 1);
      $output .= str_repeat("\t", $indent) . "</div> <!-- ppopj -->\n";
      return $output;
   }

   protected static function futer1($indent, $datum) {
      $output  = str_repeat("\t", $indent) . "<div class=\"futer\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"datum-mesto\">У " . Application::getCurrFirmProp('currFirmCity');
      if($datum == '0000-00-00') {
         $output .= ", дана _________________. године</p>\n";
      } else {
         $di = new DateTime($datum);
         $output .= ", дана " . $di->format("j.n.Y") . ". године</p>\n";
      }
      return $output;
   }

   protected static function futer2($indent) {
      $output  = str_repeat("\t", $indent+1) . "<p class=\"uvuceno\">Да су исказани подаци у овој пореској пријави тачни, тврди и оверава:</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"futer-levo potpis\">ПОРЕСКУ ПРИЈАВУ ПОПУНИО</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"futer-sredina potpis\">ОДГОВОРНО ЛИЦЕ ИСПЛАТИОЦА</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"futer-desno\">Пореску пријаву у Пореској<br/> управи контролисали:</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<ol><li>1. _________________________</li><li>2. _________________________</li></ol>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- futer -->\n";
      return $output;
   }
}
