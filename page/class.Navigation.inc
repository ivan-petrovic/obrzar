<?php

require_once 'class.PageUtil.inc';

class Navigation implements RenderHTML {
   const MENU_FIRM = 100;
   const MENU_EMPLOYE = 110;
   const MENU_BANK = 120;
   const MENU_DEDUCTION = 130;
   const MENU_SHOP = 140;
   const MENU_PAYMENT = 150;
   const MENU_KARNET = 160;
   const MENU_REPORT = 170;
   
   // Aktivan menu u navigaciji.
   protected $activeMenu;
   
   /*
    * Konstruktor.
    */
   public function __construct($menu) {
      $this->activeMenu = $menu;
   }
   
   /**
    * Prikaz glavnog menija stranice.
    * Meni za zaposlene i obracune se prikazuje samo ako je izabrana firma.
    * @param string $aktivanMeni - ime menija koji je trenutno izabran.
    */
   public function renderHTML() {
      echo str_repeat("\t",3) . "<nav>\n";
      echo str_repeat("\t",4) . "<ul>\n";

      $menu_items = $this->getMenuDescription();
      $cnt = count($menu_items);
      for ($i = 0; $i < $cnt; $i += 3) {
         echo str_repeat("\t",5) . $this->menuItem($menu_items[$i], $menu_items[$i+1], $menu_items[$i+2]);
      }
      
      echo str_repeat("\t",4) . "</ul>\n";
      echo str_repeat("\t",3) . "</nav>\n";
   }

   protected function getMenuDescription() {
      return array(
          "firm.php", self::MENU_FIRM, "Фирма",
          "employe.php", self::MENU_EMPLOYE, "Запослени",
          "bank.php", self::MENU_BANK, "Банке",
          "deduction.php", self::MENU_DEDUCTION, "Обуставе",
          "shop.php", self::MENU_SHOP, "Радње",
          "payment.php", self::MENU_PAYMENT, "Обрачуни",
          "karnet.php", self::MENU_KARNET, "Карнет",
          "report.php", self::MENU_REPORT, "Извештаји"
      );
   }
   
   /**
    * Formiranje jedne meni stavke.
    * @param string $pageName - npr. firm.php
    * @param integer $menu
    * @param string $description - tekst u okviru linka
    */
   protected function menuItem($pageName, $menu, $description) {
      $output  = "";
      $output .= "<li><a href=\"". $pageName . "\"";
      $output .= ($this->activeMenu === $menu ? " class=\"menu-active\"" : "");
      $output .= ">" . $description . "</a></li>\n";
      return $output;
   }
}
