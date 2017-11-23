<?php

require_once 'class.PaymentDB.inc';
require_once 'class.PaymentPage.inc';

class PaymentList extends PaymentPage implements RenderHTML {
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Листа обрачуна");
      $this->sectionContent();

      $this->closeSection();
   }
   
   protected function sectionContent() {
      $nizObracuna = PaymentDB::loadAll($this->fid);
      if( count($nizObracuna) === 0) {
         echo str_repeat("\t", self::$indent+2) . "<p>Нема унетих обрачуна.</p>\n";
      } else {
         echo $this->renderList($nizObracuna);
      }
      echo $this->newButton();
   }

   protected function renderList($nizObracuna) {
      $output  = str_repeat("\t", self::$indent+2) . "<p>Унети су следећи обрачуни:</p>\n";
      $output .= $this->openTable("spisak", 2);
      $output .= str_repeat("\t", self::$indent+3) . "<tr><th>Обрачун</th><th colspan=\"2\">Акција</th>\n";
      
      foreach ($nizObracuna as $payment) {
         $output .= $this->makeRow($payment->getValue('id'), $payment->getName());
      }
      $output .= $this->closeTable(2);
      return $output;
   }

   protected function makeRow($pid, $naz) {
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= $this->makeDetailsLink($pid, $naz);
      $output .= $this->makeUpdateLink($pid);
      $output .= $this->makeDeleteLink($pid);
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }

   protected function makeDetailsLink($pid, $fnaz) {
      $ref = "payment.php?opID=" . PaymentPageDispatcher::DETAILS_PAYMENT . "&paymentID=" . $pid;
      return $this->makeLink($ref, $fnaz);
   }

   protected function makeUpdateLink($pid) {
      $ref = "payment.php?opID=" . PaymentPageDispatcher::UPDATE_PAYMENT . "&paymentID=" . $pid;
      return $this->makeLink($ref, "Измени");
   }

   protected function makeDeleteLink($pid) {
      $ref = "payment.php?opID=" . PaymentPageDispatcher::DELETE_PAYMENT . "&paymentID=" . $pid;
      return $this->makeLink($ref, "Обриши");
   }
}
