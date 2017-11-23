<?php

require_once 'page/class.PageUtil.inc';
require_once 'util/class.FormMaker.inc';


class KarnetPage extends CommonPage {
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
   public function __construct($op, $fid, $pid, $eid, $indent = 1) {
      $this->op = $op;
      $this->fid = $fid;
      $this->pid = $pid;
      $this->eid = $eid;
      self::$indent = $indent;
   }

   protected function backToListButton() {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::LIST_KARNET;
      return $this->button($ref, "Назад на листу");
   }

   protected function newButton() {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::NEW_KARNET;
      return $this->button($ref, "Нови");
   }

   protected function newButtonGore() {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::NEW_KARNET;
      $output  = str_repeat("\t", self::$indent+2);
      $output .= "<p><a href=\"{$ref}\">Нови</a></p>\n";
      
      $ref1 = "karnet.php?opID=" . KarnetPageDispatcher::NEW_OSTALO_KARNET;
      $output .= str_repeat("\t", self::$indent+2);
      $output .= "<p><a href=\"{$ref1}\">Остало</a></p>\n";

      return $output;
   }

   protected function updateButton() {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::UPDATE_KARNET . "&employeID=" . $this->eid;
      return $this->button($ref, "Измени");
   }

   protected function deleteButton() {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::DELETE_KARNET . "&employeID=" . $this->eid;
      return $this->button($ref, "Обриши");
   }
   
   protected function printButton() {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::DETAILS_PRINT . "&employeID=" . $this->eid;
      return $this->button($ref, "Листић");
   }
/*
   protected function ostaloButton() {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::OSTALO_KARNET . "&employeID=" . $this->eid;
      return $this->button($ref, "Остало");
   }
*/
   public function execute() {
      return;
   }
}
