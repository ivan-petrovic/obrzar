<?php

require_once 'page/class.PageUtil.inc';
require_once 'util/class.FormMaker.inc';

class ShopPage extends CommonPage {
   // Sifra operacije za stranu, prosledjuje je page dipecer
   protected $op;
   // Sifra radnje, tamo gde ima smisla inace 0
   protected $sid;

   /*
    * Konstruktor.
    */
   public function __construct($op, $sid, $indent) {
      $this->op = $op;
      $this->sid = $sid;
      self::$indent = $indent;
   }

   protected function backToListButton() {
      $ref = "shop.php?opID=" . ShopPageDispatcher::LIST_SHOP;
      return $this->button($ref, "Назад на листу");
   }

   protected function newButton() {
      $ref = "shop.php?opID=" . ShopPageDispatcher::NEW_SHOP;
      return $this->button($ref, "Нова");
   }

   protected function updateButton() {
      $ref = "shop.php?opID=" . ShopPageDispatcher::UPDATE_SHOP . "&shopID=" . $this->sid;
      return $this->button($ref, "Измени");
   }

   protected function deleteButton() {
      $ref = "shop.php?opID=" . ShopPageDispatcher::DELETE_SHOP . "&shopID=" . $this->sid;
      return $this->button($ref, "Обриши");      
   }
   
   public function execute() {
      return;
   }
}
