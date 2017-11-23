<?php

require_once 'employe/class.EmployeDB.inc';
require_once 'class.KarnetPage.inc';

class KarnetPageNewOstalo extends KarnetPage implements RenderHTML {
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Остали подаци везани за карнет");
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $nizZaposlenih = EmployeDB::loadAll($this->fid);
      if( count($nizZaposlenih) == 0) {
         echo str_repeat("\t", self::$indent+2) . "<p>Нема унетих запослених.</p>\n";
      } else {
         echo $this->renderList($nizZaposlenih);
      }
      echo $this->backToListButton();
   }

   protected function renderList($nizZaposlenih) {
      $output  = str_repeat("\t", self::$indent+2) . "<p>Унети су следећи запослени:</p>\n";
      $output .= $this->openTable("spisak", 2);
      
      $output .= str_repeat("\t", self::$indent+3) . "<tr><th>Име и презиме</th><th>Акција</th>\n";
      $i = 1;
      foreach ($nizZaposlenih as $zaposleni) {
         $ime = $zaposleni->getValue('prezime') . " " . $zaposleni->getValue('ime');
         $output .= $this->makeRow($i, $ime, $zaposleni->getValue('id'));
         ++$i;
      }
      $output .= $this->closeTable(2);
      return $output;
   }

   protected function makeRow($rbr, $ime, $eid) {
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<td>{$rbr}. {$ime}</td>\n";
      $output .= $this->makeOstaloLink($eid);
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function makeOstaloLink($eid) {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::OSTALO_KARNET . "&employeID=" . $eid;
      return $this->makeLink($ref, "Остало");
   }
}
