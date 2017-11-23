<?php

require_once 'payment/class.PaymentDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'class.Zbir.inc';
require_once 'class.ZiroRacun.inc';
require_once 'class.ReportPage.inc';
require_once 'class.PFE.inc';
require_once 'class.PFERender.inc';

class ODReport extends ReportPage implements RenderHTML {
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("ПП ОД");
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
      echo ODZaglavlje::display($payment, $indent);
      echo ODTable::display($nizPFE, $payment, $indent);
      echo ODFuter::display($payment, $indent);
   }
}

class ODTable {
   public static function display($nizPFE, Payment $payment, $indent) {
      $r_01_4 = ZBIR::brutoUkupno($nizPFE);
      $r_02_4 = count($nizPFE);
      $r_02_5 = ZBIR::brZapNaIsplacenuZaradu($nizPFE, $payment);
      $r_02_6 = ZBIR::brZapNaNajnizuOsnovicu($nizPFE, $payment);
      $r_10_4 = ZBIR::osnovicaUkupno($nizPFE);
      $r_10_5 = ZBIR::osnovicaNaIzplacenuZaradu($nizPFE, $payment);
      $r_10_6 = ZBIR::osnovicaNaMinimalnuZaradu($nizPFE, $payment);

      $r_13_4 = round($r_10_4 * $payment->getValue('stopaPIO') / 100);
      $r_14_4 = round($r_10_4 * $payment->getValue('stopaZdravstvo') / 100);
      $r_15_4 = round($r_10_4 * $payment->getValue('stopaNezaposlenost') / 100);
      $r_12_4 = $r_13_4 + $r_14_4 + $r_15_4;

      $r_17_4 = round($r_10_4 * $payment->getValue('stopaPIOPoslodavac') / 100);
      $r_18_4 = round($r_10_4 * $payment->getValue('stopaZdravstvoPoslodavac') / 100);
      $r_19_4 = round($r_10_4 * $payment->getValue('stopaNezaposlenostPoslodavac') / 100);
      $r_16_4 = $r_17_4 + $r_18_4 + $r_19_4;
      
      $r_11_4 = $r_12_4 + $r_16_4;

      $r_13_5 = round($r_10_5 * $payment->getValue('stopaPIO') / 100);
      $r_14_5 = round($r_10_5 * $payment->getValue('stopaZdravstvo') / 100);
      $r_15_5 = round($r_10_5 * $payment->getValue('stopaNezaposlenost') / 100);
      $r_12_5 = $r_13_5 + $r_14_5 + $r_15_5;

      $r_17_5 = round($r_10_5 * $payment->getValue('stopaPIOPoslodavac') / 100);
      $r_18_5 = round($r_10_5 * $payment->getValue('stopaZdravstvoPoslodavac') / 100);
      $r_19_5 = round($r_10_5 * $payment->getValue('stopaNezaposlenostPoslodavac') / 100);
      $r_16_5 = $r_17_5 + $r_18_5 + $r_19_5;
      
      $r_11_5 = $r_12_5 + $r_16_5;

      $r_13_6 = round($r_10_6 * $payment->getValue('stopaPIO') / 100);
      $r_14_6 = round($r_10_6 * $payment->getValue('stopaZdravstvo') / 100);
      $r_15_6 = round($r_10_6 * $payment->getValue('stopaNezaposlenost') / 100);
      $r_12_6 = $r_13_6 + $r_14_6 + $r_15_6;

      $r_17_6 = round($r_10_6 * $payment->getValue('stopaPIOPoslodavac') / 100);
      $r_18_6 = round($r_10_6 * $payment->getValue('stopaZdravstvoPoslodavac') / 100);
      $r_19_6 = round($r_10_6 * $payment->getValue('stopaNezaposlenostPoslodavac') / 100);
      $r_16_6 = $r_17_6 + $r_18_6 + $r_19_6;
      
      $r_11_6 = $r_12_6 + $r_16_6;

      $tipFirme = Application::getCurrFirmProp('currFirmType');
      $r36_8 = ZiroRacun::getZR($tipFirme, ZiroRacun::ZR_PIO_ZAP);
      $r37_8 = ZiroRacun::getZR($tipFirme, ZiroRacun::ZR_ZDR_ZAP);
      $r38_8 = ZiroRacun::getZR($tipFirme, ZiroRacun::ZR_NEZ_ZAP);
      $r40_8 = ZiroRacun::getZR($tipFirme, ZiroRacun::ZR_PIO_POSL);
      $r41_8 = ZiroRacun::getZR($tipFirme, ZiroRacun::ZR_ZDR_POSL);
      $r42_8 = ZiroRacun::getZR($tipFirme, ZiroRacun::ZR_NEZ_POSL);
      
      $oddesc = array(
          array("I", "ОБРАЧУНАТЕ ЗАРАДЕ/НАКНАДЕ, ОСНОВИЦЕ ЗА ОБРАЧУН ДОПРИНОСА И ОБРАЧУН ДОПРИНОСА", "", "", "", "", "", ""),
          array("1.", "Обрачунате зараде/накнаде", "01", $r_01_4, "", "", "", ""),
          array("2.", "Број запослених/прималаца накнада", "02", $r_02_4, $r_02_5, $r_02_6, "", ""),
          array("2.1.", "Запослени са непуним радним временом (садржани у ред.бр.2)", "03", "", "", "", "", ""),
          array("2.2.", "Запослени за које се остварује ослобођење од плаћања доприноса (садржани у ред.бр.2)", "04", "", "", "", "", ""),
          array("2.2.1.", "Запослени по члану 45. став 1. Закона (садржани у ред.бр.2)", "05", "", "", "", "", ""),
          array("2.2.2.", "Запослени по члану 45. став 2. Закона (садржани у ред.бр.2)", "06", "", "", "", "", ""),
          array("2.2.3.", "Запослени по члану 45а став 1. Закона (садржани у ред.бр.2)", "07", "", "", "", "", ""),
          array("2.2.4.", "Запослени по члану 45а став 2. Закона (садржани у ред.бр.2)", "08", "", "", "", "", ""),
          array("2.2.5.", "Запослени по члану 45б став 1. Закона (садржани у ред.бр.2)", "09", "", "", "", "", ""),
          array("3.", "Основица за обрачун доприноса", "10", $r_10_4, $r_10_5, $r_10_6, "", ""),
          array("4.", "Укупно обрачунати доприноси (ред.бр.4.1. плус ред.бр.4.2.)", "11", $r_11_4, $r_11_5, $r_11_6, "", ""),
          array("4.1.", "На терет запослених/прималаца накнаде (ред.бр.4.1.1. до 4.1.3.)", "12", $r_12_4, $r_12_5, $r_12_6, "", ""),
          array("4.1.1.", "За ПИО", "13", $r_13_4, $r_13_5, $r_13_6, "", ""),
          array("4.1.2.", "За здравствено осигурање", "14", $r_14_4, $r_14_5, $r_14_6, "", ""),
          array("4.1.3.", "За осигурање за случај незапослености", "15", $r_15_4, $r_15_5, $r_15_6, "", ""),
          array("4.2.", "На терет послодавца (ред.бр.4.2.1. до ред.бр.4.2.3.)", "16", $r_16_4, $r_16_5, $r_16_6, "", ""),
          array("4.2.1.", "За ПИО", "17", $r_17_4, $r_17_5, $r_17_6, "", ""),
          array("4.2.2.", "За здравствено осигурање", "18", $r_18_4, $r_18_5, $r_18_6, "", ""),
          array("4.2.3.", "За осигурање за случај незапослености", "19", $r_19_4, $r_19_5, $r_19_6, "", ""),
          array("5.", "Број запослених за које се плаћа допринос за стаж осигурања са увећаним трајањем - бенефицирани стаж (садржан у редном броју 2)", "20", "", "", "", "", ""),
          array("6.", "Допринос за ПИО за бенефицирани стаж", "21", "", "", "", "", ""),
          array("II", "РАНИЈЕ ИСПЛАЋЕНИ ДЕО ЗАРАДЕ/НАКНАДЕ И ПЛАЋЕНИ ДОПРИНОСИ", "", "", "", "", "", ""),
          array("7.", "Раније исплаћени део зараде/накнаде", "22", "", "", "", "", ""),
          array("8.", "Укупно плаћени доприноси (ред.бр.8.1. плус ред.бр.8.2.", "23", "", "", "", "", ""),
          array("8.1.", "На терет запослених/прималаца накнаде (ред.бр.8.1.1. до 8.1.3.)", "24", "", "", "", "", ""),
          array("8.1.1.", "За ПИО", "25", "", "", "", "", ""),
          array("8.1.2.", "За здравствено осигурање", "26", "", "", "", "", ""),
          array("8.1.3.", "За осигурање за случај незапослености", "27", "", "", "", "", ""),
          array("8.2.", "На терет послодавца (ред.бр.8.2.1. до ред.бр.8.2.3.)", "28", "", "", "", "", ""),
          array("8.2.1.", "За ПИО", "29", "", "", "", "", ""),
          array("8.2.2.", "За здравствено осигурање", "30", "", "", "", "", ""),
          array("8.2.3.", "За осигурање за случај незапослености", "31", "", "", "", "", ""),
          array("9.", "Плаћени доприноси за ПИО за бенефицирани стаж", "32", "", "", "", "", ""),
          array("III", "ЗАРАДА ЗА ИСПЛАТУ И ДОПРИНОСИ ЗА УПЛАТУ", "", "", "", "", "", ""),
          array("10.", "Зарада за исплату (ред.бр.1. минус ред.бр.7.) већи од 0", "33", $r_01_4, "", "", "", ""),
          array("11.", "Доприноси за уплату (ред.бр.4. минус ред.бр.8.) већи од 0", "34",  $r_11_4, $r_11_5, $r_11_6, "", ""),
          array("11.1.", "На терет запослених/прималаца накнаде (ред.бр.4.1. минус ред.бр.8.1.)", "35", $r_12_4, $r_12_5, $r_12_6, "", ""),
          array("11.1.1.", "За ПИО (ред.бр.4.1.1. минус ред.бр.8.1.1.)", "36", $r_13_4, $r_13_5, $r_13_6, "", $r36_8),
          array("11.1.2.", "За здравствено осигурање (ред.бр.4.1.2. минус ред.бр.8.1.2.)", "37", $r_14_4, $r_14_5, $r_14_6, "", $r37_8),
          array("11.1.3.", "За осигурање за случај незапослености (ред.бр.4.1.3. минус ред.бр.8.1.3.)", "38", $r_15_4, $r_15_5, $r_15_6, "", $r38_8),
          array("11.2.", "На терет послодавца (ред.бр.4.2. минус ред.бр.8.2.)", "39", $r_16_4, $r_16_5, $r_16_6, "", ""),
          array("11.2.1.", "За ПИО (ред.бр.4.2.1. минус ред.бр.8.2.1.)", "40",$r_17_4, $r_17_5, $r_17_6, "", $r40_8),
          array("11.2.2.", "За здравствено осигурање (ред.бр.4.2.2. минус ред.бр.8.2.2.)", "41", $r_18_4, $r_18_5, $r_18_6, "", $r41_8),
          array("11.2.3.", "За осигурање за случај незапослености (ред.бр.4.2.3. минус ред.бр.8.2.3.)", "42", $r_19_4, $r_19_5, $r_19_6, "", $r42_8),
          array("12.", "Допринос за уплату - ПИО за бенефицирани стаж (ред.бр.6. минус ред.бр.9.)", "43", "", "", "", "", ""),
          array("IV", "ВИШЕ ПЛАЋЕНИ ДОПРИНОСИ", "", "", "", "", "", ""),
          array("13.", "Више плаћени доприноси (ред.бр.13.1. плус ред.бр.13.2.)", "44", "", "", "", "", ""),
          array("13.1.", "На терет запослених/прималаца накнаде (ред.бр.8.1. минус ред.бр.4.1.)", "45", "", "", "", "", ""),
          array("13.1.1.", "За ПИО (ред.бр.8.1.1. минус ред.бр.4.1.1.)", "46", "", "", "", "", ""),
          array("13.1.2.", "За здравствено осигурање (ред.бр.8.1.2. минус ред.бр.4.1.2.)", "47", "", "", "", "", ""),
          array("13.1.3.", "За осигурање за случај незапослености (ред.бр.8.1.3. минус ред.бр.4.1.3.)", "48", "", "", "", "", ""),
          array("13.2.", "На терет послодавца (ред.бр.8.2. минус ред.бр.4.2.)", "49", "", "", "", "", ""),
          array("13.2.1.", "За ПИО (ред.бр.8.2.1. минус ред.бр.4.2.1.)", "50", "", "", "", "", ""),
          array("13.2.2.", "За здравствено осигурање (ред.бр.8.2.2. минус ред.бр.4.2.2.)", "51", "", "", "", "", ""),
          array("13.2.3.", "За осигурање за случај незапослености (ред.бр.8.2.3. минус ред.бр.4.2.3.)", "52", "", "", "", "", ""),
          array("V", "ИЗНОС ДОПРИНОСА ЗА УПЛАТУ ПО УМАЊЕЊУ ЗА ВИШЕ ПЛАЋЕНЕ ДОПРИНОСЕ", "", "", "", "", "", ""),
          array("14.", "Доприноси за уплату (ред.бр.11. минус ред.бр.13. из ПП ОД по коначном обрачуну за предходни месец)", "53", $r_11_4, $r_11_5, $r_11_6, "", ""),
          array("14.1.", "На терет запослених/прималаца накнаде (ред.бр.11.1. минус ред.бр.13.1. из ПП ОД по коначном обрачуну за предходни месец)", "54", $r_12_4, $r_12_5, $r_12_6, "", ""),
          array("14.1.1.", "За ПИО (ред.бр.11.1.1. минус ред.бр.13.1.1. из ПП ОД по коначном обрачуну за предходни месец)", "55", $r_13_4, $r_13_5, $r_13_6, "", ""),
          array("14.1.2.", "За здравствено осигурање (ред.бр.11.1.2. минус ред.бр.13.1.2. из ПП ОД по коначном обрачуну за предходни месец)", "56", $r_14_4, $r_14_5, $r_14_6, "", ""),
          array("14.1.3.", "За осигурање за случај незапослености (ред.бр.11.1.3. минус ред.бр.13.1.3. из ПП ОД по коначном обрачуну за предходни месец)", "57", $r_15_4, $r_15_5, $r_15_6, "", ""),
          array("14.2.", "На терет послодавца (ред.бр.11.2. минус ред.бр.13.2. из ПП ОД по коначном обрачуну за предходни месец)", "58", $r_16_4, $r_16_5, $r_16_6, "", ""),
          array("14.2.1.", "За ПИО (ред.бр.11.2.1. минус ред.бр.13.2.1. из ПП ОД по коначном обрачуну за предходни месец)", "59", $r_17_4, $r_17_5, $r_17_6, "", ""),
          array("14.2.2.", "За здравствено осигурање (ред.бр.11.2.2. минус ред.бр.13.2.2. из ПП ОД по коначном обрачуну за предходни месец)", "60", $r_18_4, $r_18_5, $r_18_6, "", ""),
          array("14.2.3.", "За осигурање за случај незапослености (ред.бр.11.2.3. минус ред.бр.13.2.3. из ПП ОД по коначном обрачуну за предходни месец)", "61", $r_19_4, $r_19_5, $r_19_6, "", "")
      );
      
      $output  = self::tabelaUputstvo($indent+1);
      $output .= self::tabelaZaglavlje($indent+1);
      foreach ($oddesc as $red) {
         $output .= self::makeRow($red, $indent+1);
      }
      $output .= self::tabelaFuter($indent+1);
      return $output;
   }
   
   protected static function tabelaUputstvo($indent) {
      $output  = str_repeat("\t", $indent) . "<div class=\"glavni-sadrzaj\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"tabela-uputstvo\">новчани износи уносе се у динарима, ";
      $output .= "заокружени на целе бројеве, а број запослених у целом броју</p>\n";
      return $output;
   }

   protected static function tabelaZaglavlje($indent) {
      $output  = str_repeat("\t", $indent+1) . "<table>\n";
      $output .= str_repeat("\t", $indent+1) . "<thead>\n";
      $output .= str_repeat("\t", $indent+1) . "<tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<th rowspan=\"2\" class=\"col1\">Ред.бр.</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th rowspan=\"2\" class=\"col2\">О П И С</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th rowspan=\"2\" class=\"col3\">Ознака за АОП</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th rowspan=\"2\" class=\"col4\">УКУПНО</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th colspan=\"3\" class=\"col567\">ОБРАЧУН ДОПРИНОСА</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th rowspan=\"2\" class=\"col8\">Уплатни рачун јавних прихода</th>\n";
      $output .= str_repeat("\t", $indent+1) . "</tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>на исплаћену зараду/накнаду</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>на најнижу основицу</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th>на највишу основицу</th>\n";
      $output .= str_repeat("\t", $indent+1) . "</tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<tr>\n";
      $output .= str_repeat("\t", $indent+1) . "<th class=\"col1\">1</th><th class=\"col2\">2</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th class=\"col3\">3</th><th class=\"col4\">4</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th class=\"col5\">5</th><th class=\"col6\">6</th>\n";
      $output .= str_repeat("\t", $indent+1) . "<th class=\"col7\">7</th><th class=\"col8\">8</th>\n";
      $output .= str_repeat("\t", $indent+1) . "</tr>\n";
      $output .= str_repeat("\t", $indent+1) . "</thead>\n";
      
      $output .= str_repeat("\t", $indent+1) . "<tbody>\n";
      return $output;
   }

   protected static function tabelaFuter($indent) {
      $output  = str_repeat("\t", $indent+2) . "</tbody>\n";
      $output .= str_repeat("\t", $indent+1) . "</table>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>*)Ако се зарада/накнада исплаћује у деловима, ";
      $output .= "уноси се број дела исплате зараде која се исплаћује за месец; ";
      $output .= "ако је део зараде који се исплаћује и коначан обрачун за месец треба подвући и реч коначна исплата. ";
      $output .= "<span class=\"podvuceno\">КОНАЧАН ОБРАЧУН ЗА ИСПЛАТУ ЗАРАДЕ МОЖЕ ДА СЕ ИЗВРШИ САМО ЈЕДНОМ ЗА МЕСЕЦ.</span></p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- glavni-sadrzaj -->\n";
      return $output;
   }
   
   protected static function makeRow($red, $indent) {
      $output  = str_repeat("\t", $indent+1) . "<tr>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col1\">{$red[0]}</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col2\">{$red[1]}</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col3\">{$red[2]}</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col4\">" . ($red[3] > 0 ? number_format($red[3], 0, ",", "."): "") . "</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col5\">" . ($red[4] > 0 ? number_format($red[4], 0, ",", "."): "") . "</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col6\">" . ($red[5] > 0 ? number_format($red[5], 0, ",", "."): "") . "</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col7\">" . ($red[6] > 0 ? number_format($red[6], 0, ",", "."): "") . "</td>\n";
      $output .= str_repeat("\t", $indent+2) . "<td class=\"col8\">" . $red[7] . "</td>\n";
      $output .= str_repeat("\t", $indent+1) . "</tr>\n";
      return $output;
   }
}

class ODZaglavlje {
   public static function display(Payment $payment, $indent) {
      $output  = str_repeat("\t", $indent) . "<div class=\"ppod\">\n";
      $output .= self::goreLevo($indent + 1);
      $output .= self::goreDesno($indent + 1);
      $output .= self::naslov($indent + 1, $payment->getMonthName(), $payment->getValue('godina'));
      $output .= self::sredinaLevo($indent + 1);
      $output .= self::sredinaDesno($indent + 1);
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

   protected static function naslov($indent, $mesec, $godina) {
      $output  = str_repeat("\t", $indent);
      $output .= "<p class=\"glavni-naslov\">Пореска пријава о обрачунатим и плаћеним доприоносима<br/>";
      $output .= "за обавезно социјално осигурање на зараде/накнаде<br/>";
      $output .= "за месец {$mesec} ";
      $output .= "<span>(<strong>коначна исплата</strong>/део*____)</span>";
      $output .= "{$godina}. године</p>\n";
      return $output;
   }

   protected static function sredinaLevo($indent) {
      $output  = str_repeat("\t", $indent) . "<div class=\"sredina-levo\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p><strong>Исплатилац je:</strong></p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>(заокружити само један од понуђених редних бројева)</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>1. Правно лице које се не финансира из буџета</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>2. Правно лице које се финансира из буџета</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>3. Страно представништво</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>4. Предузетник</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>5. Физичко лице</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- sredina-levo -->\n";
      return $output;
   }

   protected static function sredinaDesno($indent) {
      $output  = str_repeat("\t", $indent) . "<div class=\"sredina-desno\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p><strong>Обрачун и плаћање доприноса врши се за исплату:</strong></p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>(заокружити само један од понуђених редних бројева)</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>1. Зарада запослених</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>2. Зарада оснивача и чланова предузећа запослених у свом предузећу</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>3. Разлике зарада изабраних, именованих и постављених лица</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>4. Накнада за привремене и повремене послове</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p>5. Накнада за привремене и повремене послове члану задруге</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- sredina-desno -->\n";
      return $output;
   }

   protected static function datumIsplate($indent, $datum) {
      $output  = str_repeat("\t", $indent) . "<div class=\"datum-isplate\">\n";
      $output .= str_repeat("\t", $indent+1) . "<p><strong>Датум исплате:</strong></p>\n";
      if($datum == '0000-00-00') {
         $output .= str_repeat("\t", $indent+1) . "<p>___________________</p>\n";
      } else {
         $di = new DateTime($datum);
         $output .= str_repeat("\t", $indent+1) . "<p>" . $di->format("j.n.Y") . "</p>\n";
      }
      $output .= str_repeat("\t", $indent) . "</div> <!-- datum-isplate -->\n";
      return $output;
   }
}

class ODFuter {
   public static function display(Payment $payment, $indent) {
      $output  = self::futer1($indent + 1, $payment->getValue('datumIsplate'));
      $output .= self::futer2($indent + 1);
      $output .= str_repeat("\t", $indent) . "</div> <!-- ppod -->\n";
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
      $output  = str_repeat("\t", $indent+1) . "<p class=\"uvuceno\">Да су исказани подаци у овој пореској пријави тачни, тврди и<br/>оверава:</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"futer-levo potpis\">ПОРЕСКУ ПРИЈАВУ ПОПУНИО</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"futer-levo\">(М.П.)</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"futer-levo potpis\">ОДГОВОРНО ЛИЦЕ ИСПЛАТИОЦА</p>\n";
      $output .= str_repeat("\t", $indent+1) . "<p class=\"futer-desno\">Пореску пријаву у Пореској управи<br/> контролисали:</p>\n";
      $output .= str_repeat("\t", $indent) . "</div> <!-- futer -->\n";
      $output .= str_repeat("\t", $indent) . "<ol><li>1. _________________________</li><li>2. _________________________</li></ol>\n";      
      return $output;
   }
}
