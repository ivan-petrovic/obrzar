<?php

require_once 'page/class.PageUtil.inc';
require_once 'util/class.FormMaker.inc';

class ReportPage extends CommonPage {
   // Sifra operacije za stranu, prosledjuje je page dipecer
   protected $op;
   // Sifra trenutno izabrane firme
   protected $fid;
   // sifra trenutno izabranog obracuna
   protected $pid;
   // Sifra zaposlenog tamo gde ima smisla, inace 0
   protected $eid;

   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $pid, $eid, $indent) {
      $this->op = $op;
      $this->fid = $fid;
      $this->pid = $pid;
      $this->eid = $eid;
      self::$indent = $indent;
   }

   protected function backToListButton() {
      $ref = "report.php";
      return $this->button($ref, "Назад на листу");
   }
}

class ReportList extends ReportPage implements RenderHTML {
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Извештаји");
      echo $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $output = $this->openTable("spisak", 2);
      $output .= str_repeat("\t", self::$indent+3) . "<tr><th>Назив извештаја</th><th>Вирмани за извештај</th>\n";

      $nizIzvestaja = $this->reportLinks();
      for ($i = 0; $i < count($nizIzvestaja); $i += 4) {
         $output .= $this->makeRow($nizIzvestaja[$i], $nizIzvestaja[$i+1], $nizIzvestaja[$i+2], $nizIzvestaja[$i+3]);
      }
      $output .= $this->closeTable(2);
      return $output;
   }

   protected function reportLinks() {
      return array(
          ReportPageDispatcher::LISTICI_REPORT, 'Листићи', 0, '',
          ReportPageDispatcher::PREGLED_ZARADE_REPORT, 'Карнет зараде', ReportPageDispatcher::PREGLED_M4_REPORT, 'Преглед за М4',
          ReportPageDispatcher::PPOD_REPORT, 'Образац ПП ОД', 0, '',
          ReportPageDispatcher::OPJ_REPORT, 'Образац ОПЈ',  0, '',
          ReportPageDispatcher::REKAPITULACIJA_REPORT, 'Рекапитулација', ReportPageDispatcher::REKAPITULACIJA_VIRMAN, 'Вирман за обједињену наплату',
          ReportPageDispatcher::LISTA_REPORT, 'Исплатна листа', 0, '',
          ReportPageDispatcher::SPECIFIKACIJA_REPORT, 'Спецификација пореза и доприноса', ReportPageDispatcher::PPP_REPORT, 'Спецификација за ППП образац',
          ReportPageDispatcher::BANKE_REPORT, 'Запослени по банкама', ReportPageDispatcher::BANKE_VIRMAN_REPORT, 'Вирмани за банке',
          ReportPageDispatcher::OBUSTAVE_REPORT, 'Преглед обустава', ReportPageDispatcher::OBUSTAVE_VIRMAN_REPORT, 'Вирмани за обуставе',
          ReportPageDispatcher::OBUSTAVE_UKUPNO_REPORT, 'Картица радњи', 0, '');
   }
   
   protected function makeRow($izvestaj1, $opis1, $izvestaj2, $opis2) {
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td><a href=\"report.php?opID={$izvestaj1}\">{$opis1}</a></td>\n";
      
      if($izvestaj2 !== 0) {
         $output .= str_repeat("\t", self::$indent+3) . "<td><a href=\"report.php?opID={$izvestaj2}\">{$opis2}</a></td>\n";
      } else {
         $output .= str_repeat("\t", self::$indent+3) . "<td></td>\n";
      }
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      
      return $output;
   }
}
