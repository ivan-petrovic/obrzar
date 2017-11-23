<?php

require_once 'class.ReportPage.inc';

class ReportPageDispatcher {
   const LISTICI_REPORT = 8000;
   const PPOD_REPORT = 8100;
   const OPJ_REPORT = 8200;
   const PPP_REPORT = 8250;
   const REKAPITULACIJA_REPORT = 8300;
   const REKAPITULACIJA_VIRMAN = 8350;
   const LISTA_REPORT = 8400;
   const SPECIFIKACIJA_REPORT = 8450;
   const BANKE_REPORT = 8500;
   const BANKE_VIRMAN_REPORT = 8600;
   const OBUSTAVE_REPORT = 8700;
   const OBUSTAVE_VIRMAN_REPORT = 8800;
   const OBUSTAVE_UKUPNO_REPORT = 8850;
   const PREGLED_ZARADE_REPORT = 8900;
   const PREGLED_M4_REPORT = 8950;

   /*
    * Na sifre osnovu operacije izaberi podstranu izvestaja
    * @param integer $op - sifra operacije
    * @param integer $fid - sifra tekuce firme
    * @param integer $pid - sifra tekuceg obracuna (izabranog obracuna)
    * @param integer $eid - sifra zaposlenog tamo gde ima smisla, 0 inace
    * @param integer $indent - pocetna indentacija sadrzaja strane
    */
   static public function choosePage($op, $fid, $pid, $eid, $indent) {
      switch ($op) {
         case ReportPageDispatcher::LISTICI_REPORT:
            require_once 'report/class.ListicReport.inc';
            return new ListicReport($op, $fid, $pid, 0, $indent);
         case ReportPageDispatcher::PPOD_REPORT:
            require_once 'report/class.ODReport.inc';
            return new ODReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::OPJ_REPORT:
            require_once 'report/class.OPJReport.inc';
            return new OPJReport($op, $fid, $pid, 0, $indent);
         case ReportPageDispatcher::REKAPITULACIJA_REPORT:
            require_once 'report/class.RekapitulacijaReport.inc';
            return new RekapitulacijaReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::REKAPITULACIJA_VIRMAN:
            require_once 'report/class.RecapVirman.inc';
            return new RecapVirman($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::LISTA_REPORT:
            require_once 'report/class.ListaReport.inc';
            return new ListaReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::SPECIFIKACIJA_REPORT:
            require_once 'report/class.SpecReport.inc';
            return new SpecReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::BANKE_REPORT:
            require_once 'report/class.BankReport.inc';
            return new BankReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::BANKE_VIRMAN_REPORT:
            require_once 'report/class.BankReport.inc';
            return new BankVirmanReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::OBUSTAVE_REPORT:
            require_once 'report/class.DeductionReport.inc';
            return new DeductionReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::OBUSTAVE_VIRMAN_REPORT:
            require_once 'report/class.DeductionReport.inc';
            return new DeductionVirmanReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::OBUSTAVE_UKUPNO_REPORT:
            require_once 'report/class.DeductionUkupnoReport.inc';
            return new DeductionUkupnoReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::PREGLED_ZARADE_REPORT:
            require_once 'report/class.PregledZaradeReport.inc';
            return new PregledZaradeReport($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::PREGLED_M4_REPORT:
            require_once 'report/class.PregledM4Report.inc';
            return new PregledM4Report($op, $fid, $pid, $eid, $indent);
         case ReportPageDispatcher::PPP_REPORT:
            require_once 'report/class.PPPReport.inc';
            return new PPPReport($op, $fid, $pid, $eid, $indent);
            default:
            return new ReportList($op, $fid, $pid, 0, $indent);
      }
   }

   /*
    * Na sifre osnovu operacije bira CSS fajl koji treba da ucita zaglavlje strane.
    * @param integer $op - sifra operacije
    */
   static public function getCSSFileName($op) {
      $cssFiles = self::CSSFileNamesMap();
      if(array_key_exists($op, $cssFiles)) {
         return $cssFiles[$op];
      }
      return "rspisak.css";
   }
   
   /*
    * Funkcija vraca niz koji mapira sifru operacije na naziv css fajla
    * pridruzenog toj operaciji.
    */
   static protected function CSSFileNamesMap() {
      return array(
         ReportPageDispatcher::LISTICI_REPORT => "listic.css",
         ReportPageDispatcher::PREGLED_ZARADE_REPORT => 'pregzar.css',
         ReportPageDispatcher::PREGLED_M4_REPORT => 'pregm4.css',
         ReportPageDispatcher::PPOD_REPORT => "ppod.css",
         ReportPageDispatcher::OPJ_REPORT => "opj.css",
         ReportPageDispatcher::PPP_REPORT => "pregzar.css",
         ReportPageDispatcher::REKAPITULACIJA_REPORT => "rekapitulacija.css",
         ReportPageDispatcher::REKAPITULACIJA_VIRMAN => "nalog1.css",
         ReportPageDispatcher::LISTA_REPORT => "isplatnalista.css",
         ReportPageDispatcher::SPECIFIKACIJA_REPORT => "specifikacija.css",
         ReportPageDispatcher::BANKE_REPORT => "banke.css",
         ReportPageDispatcher::BANKE_VIRMAN_REPORT => "nalog1.css",
         ReportPageDispatcher::OBUSTAVE_REPORT => "radnje.css",
         ReportPageDispatcher::OBUSTAVE_VIRMAN_REPORT => "nalog1.css",
         ReportPageDispatcher::OBUSTAVE_UKUPNO_REPORT => "radnje.css"
      );
   }
}
