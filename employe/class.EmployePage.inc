<?php

require_once 'page/class.PageUtil.inc';
require_once 'util/class.FormMaker.inc';

class EmployePage extends CommonPage {
   // Sifra operacije za stranu, prosledjuje je page dipecer
   protected $op;
   // Sifra trenutno izabrane firme
   protected $fid;
   // sifra zaposlenog, tamo gde ima smisla inace 0
   protected $eid;

   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $eid, $indent) {
      $this->op = $op;
      $this->fid = $fid;
      $this->eid = $eid;
      self::$indent = $indent;
   }

   protected function backToListButton() {
      $ref = "employe.php?opID=" . EmployePageDispatcher::LIST_EMPLOYE;
      return $this->button($ref, "Назад на листу");
   }

   protected function newButton() {
      $ref = "employe.php?opID=" . EmployePageDispatcher::NEW_EMPLOYE;
      return $this->button($ref, "Нови");
   }

   protected function newButtonGore() {
      $ref = "employe.php?opID=" . EmployePageDispatcher::NEW_EMPLOYE;
      $output  = str_repeat("\t", self::$indent+2);
      $output .= "<p><a href=\"{$ref}\">Нови</a></p>\n";
      return $output;
   }

   protected function updateButton() {
      $ref = "employe.php?opID=" . EmployePageDispatcher::UPDATE_EMPLOYE . "&employeID=" . $this->eid;
      return $this->button($ref, "Измени");
   }

   protected function deleteButton() {
      $ref = "employe.php?opID=" . EmployePageDispatcher::DELETE_EMPLOYE . "&employeID=" . $this->eid;
      return $this->button($ref, "Обриши");      
   }
   
   public function execute() {
      return;
   }
}
