<?php

require_once 'employe/class.EmployeDB.inc';
require_once 'class.KarnetDB.inc';
require_once 'class.KarnetPage.inc';

class KarnetList extends KarnetPage implements RenderHTML {
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Карнет");
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $nizKarneta = KarnetDB::loadAll($this->pid);
      if( count($nizKarneta) === 0) {
         echo str_repeat("\t", self::$indent+2) . "<p>Нема унетих карнета.</p>\n";
      } else {
         echo $this->newButtonGore();
         echo $this->renderList($nizKarneta);
      }
      echo $this->newButton();
   }


   protected function renderList($nizKarneta) {
      $output  = str_repeat("\t", self::$indent+2) . "<p>Унети су следећи карнети:</p>\n";
      $output .= $this->openTable("spisak", 2);
      
      $output .= str_repeat("\t", self::$indent+3) . "<tr><th>Име и презиме</th><th colspan=\"4\">Акција</th>\n";
      $i = 1;
      foreach ($nizKarneta as $karnet) {
         $output .= $this->makeRow($i, $karnet->getValue('zaposleniID'));
         ++$i;
      }
      $output .= $this->closeTable(2);
      return $output;
   }

   protected function makeRow($rbr,$eid) {
      $ime = EmployeDB::name($eid);
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= $this->makeDetailsLink($eid, $rbr . ". " . $ime);
      $output .= $this->makePrintLink($eid);
      $output .= $this->makeUpdateLink($eid);
      $output .= $this->makeDeleteLink($eid);
      // $output .= $this->makeOstaloLink($eid);
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function makeDetailsLink($eid, $ime) {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::DETAILS_KARNET . "&employeID=" . $eid;
      return $this->makeLink($ref, $ime);
   }

   protected function makePrintLink($eid) {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::DETAILS_PRINT . "&employeID=" . $eid;
      return $this->makeLink($ref, "Листић");
   }

   protected function makeUpdateLink($eid) {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::UPDATE_KARNET . "&employeID=" . $eid;
      return $this->makeLink($ref, "Измени");
   }

   protected function makeDeleteLink($eid) {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::DELETE_KARNET . "&employeID=" . $eid;
      return $this->makeLink($ref, "Обриши");
   }
/*
   protected function makeOstaloLink($eid) {
      $ref = "karnet.php?opID=" . KarnetPageDispatcher::OSTALO_KARNET . "&employeID=" . $eid;
      return $this->makeLink($ref, "Остало");
   }
 */
}
