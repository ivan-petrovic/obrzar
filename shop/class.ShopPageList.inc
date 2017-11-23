<?php

require_once 'class.ShopDB.inc';
require_once 'class.ShopPage.inc';

class ShopList extends ShopPage implements RenderHTML {
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Листа радњи");
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $nizRadnji = ShopDB::loadAll();
      if( count($nizRadnji) === 0) {
         echo str_repeat("\t", self::$indent+2) . "<p>Нема унетих радњи.</p>\n";
      } else {
         echo $this->renderList($nizRadnji);
      }
      echo $this->newButton();
   }
   
   protected function renderList($nizRadnji) {
      $output  = str_repeat("\t", self::$indent+2) . "<p>Унете су следеће радње:</p>\n";

      $output .= $this->openTable("spisak", 2);
      $output .= str_repeat("\t", self::$indent+3) . "<tr><th>Назив</th><th colspan=\"2\">Акција</th>\n";
      foreach ($nizRadnji as $shop) {
         $output .= $this->makeRow($shop->getValue('id'), $shop->getValue('naziv'));
      }
      $output .= $this->closeTable(2);
      return $output;
   }

   protected function makeRow($sid, $fnaz) {
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= $this->makeDetailsLink($sid, $fnaz);
      $output .= $this->makeUpdateLink($sid);
      $output .= $this->makeDeleteLink($sid);
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function makeDetailsLink($sid, $fnaz) {
      $ref = "shop.php?opID=" . ShopPageDispatcher::DETAILS_SHOP . "&shopID=" . $sid;
      return $this->makeLink($ref, $fnaz);
   }

   protected function makeUpdateLink($sid) {
      $ref = "shop.php?opID=" . ShopPageDispatcher::UPDATE_SHOP . "&shopID=" . $sid;
      return $this->makeLink($ref, "Измени");
   }

   protected function makeDeleteLink($sid) {
      $ref = "shop.php?opID=" . ShopPageDispatcher::DELETE_SHOP . "&shopID=" . $sid;
      return $this->makeLink($ref, "Обриши");
   }
}
