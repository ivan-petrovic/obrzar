<?php
require_once 'payment/class.PaymentDB.inc';
require_once 'employe/class.EmployeDB.inc';
require_once 'karnet/class.KarnetDB.inc';
require_once 'class.ReportPage.inc';
require_once 'report/class.PFE.inc';
require_once 'report/class.PFERender.inc';

class ListicReport extends ReportPage implements RenderHTML {
   protected $brojKopija = 1;
   
   public function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Листићи за запослене");
      $this->sectionContent();

      $this->closeSection();
   }

   protected function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      }
      echo $this->makeForm();
      echo $this->printListici();
   }

   protected function makeForm() {
      $output  = "";
      $scriptName = "report.php?opID=" . $this->op;
      $output .= FormMaker::openForm($scriptName, 5);

      $output .= FormMaker::textElement('Број копија', "brkopija", $this->brojKopija, self::$indent+3);
      $output .= FormMaker::submitElement('Примени', 6);
      
      $output .= FormMaker::closeForm(5);
      return $output;
   }

   protected function processForm() {
      $this->brojKopija = filter_input(INPUT_POST, 'brkopija', FILTER_VALIDATE_INT, array('options' => array('default' => 1)));
   }

   protected function printListici() {
      $payment = PaymentDB::load($this->pid);
      $nizKarnet = KarnetDB::loadAll($this->pid);
      
      $nizPFE = array();
      foreach ($nizKarnet as $karnet) {
         $employe = EmployeDB::load($karnet->getValue('zaposleniID'));
         // $nizPFE[] = new KonReport($employe, $payment, $karnet);
         $nizPFE[] = new PaymentForEmploye($employe, $payment, $karnet);
      }
      $this->renderList($nizPFE);
      echo $this->backToListButton();
   }
   
   protected function renderList($nizPFE) {
      foreach ($nizPFE as $pfe) {
         for($i = 1; $i <= $this->brojKopija; ++$i) {
            PFERender::display1($pfe, 5);
            echo str_repeat("\t", self::$indent + 2) . "<hr>\n";
         }
      }      
   }
}
