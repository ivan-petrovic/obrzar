<?php

require_once 'class.FirmDB.inc';
require_once 'class.FirmPage.inc';

class FirmList extends FirmPage implements RenderHTML {
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Листа фирми");
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $nizFirmi = FirmDB::loadAll();
      if( count($nizFirmi) === 0) {
         echo str_repeat("\t", self::$indent + 2) . "<p>Нема унетих фирми.</p>\n";
      } else {
         echo $this->renderList($nizFirmi);
      }
      echo $this->newButton();
   }
   
   protected function renderList($nizFirmi) {
      $output  = str_repeat("\t", self::$indent+2) . "<p>Изаберите фирму за коју се ради обрачун зарада:</p>\n";
      
      $output .= $this->openTable("spisak", 2);
      $output .= str_repeat("\t", self::$indent+3) . "<tr><th>Назив</th><th colspan=\"2\">Акција</th>\n";
      foreach ($nizFirmi as $firma) {
         $output .= $this->makeRow($firma->getValue('id'), $firma->getValue('naziv'));
      }
      $output .= $this->closeTable(2);
      return $output;
   }

   protected function makeRow($fid, $fnaz) {
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= $this->makeDetailsLink($fid, $fnaz);
      $output .= $this->makeUpdateLink($fid);
      $output .= $this->makeDeleteLink($fid);
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function makeDetailsLink($fid, $fnaz) {
      $ref = "firm.php?opID=" . FirmPageDispatcher::DETAILS_FIRM . "&firmID=" . $fid;
      return $this->makeLink($ref, $fnaz);
   }

   protected function makeUpdateLink($fid) {
      $ref = "firm.php?opID=" . FirmPageDispatcher::UPDATE_FIRM . "&firmID=" . $fid ;
      return $this->makeLink($ref, "Измени");
   }

   protected function makeDeleteLink($fid) {
      $ref = "firm.php?opID=" . FirmPageDispatcher::DELETE_FIRM . "&firmID=" . $fid;
      return $this->makeLink($ref, "Обриши");
   }
}
