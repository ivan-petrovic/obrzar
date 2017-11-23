<?php

require_once 'class.BankDB.inc';
require_once 'class.BankPage.inc';

class BankList extends BankPage implements RenderHTML {
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Листа банака");
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $nizBanki = BankDB::loadAll();
      if( count($nizBanki) === 0) {
         echo str_repeat("\t", self::$indent+2) . "<p>Нема унетих банака.</p>\n";
      } else {
         echo $this->renderList($nizBanki);
      }
      echo $this->newButton();
   }
   
   protected function renderList($nizBanki) {
      $output  = str_repeat("\t", self::$indent+2) . "<p>Унете су следеће банке:</p>\n";
      
      $output .= $this->openTable("spisak", 2);
      $output .= str_repeat("\t", self::$indent+3) . "<tr><th>Назив</th><th colspan=\"2\">Акција</th>\n";
      foreach ($nizBanki as $bank) {
         $output .= $this->makeRow($bank->getValue('id'), $bank->getValue('naziv'));
      }
      $output .= $this->closeTable(2);
      return $output;
   }

   protected function makeRow($bid, $fnaz) {
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= $this->makeDetailsLink($bid, $fnaz);
      $output .= $this->makeUpdateLink($bid);
      $output .= $this->makeDeleteLink($bid);
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function makeDetailsLink($bid, $fnaz) {
      $ref = "bank.php?opID=" . BankPageDispatcher::DETAILS_BANK . "&bankID=" . $bid;
      return $this->makeLink($ref, $fnaz);
   }

   protected function makeUpdateLink($bid) {
      $ref = "bank.php?opID=" . BankPageDispatcher::UPDATE_BANK . "&bankID=" . $bid;
      return $this->makeLink($ref, "Измени");
   }

   protected function makeDeleteLink($bid) {
      $ref = "bank.php?opID=" . BankPageDispatcher::DELETE_BANK . "&bankID=" . $bid;
      return $this->makeLink($ref, "Обриши");
   }
}
