<?php

require_once 'class.DeductionDB.inc';
require_once 'class.DeductionPage.inc';

class DeductionDelete extends DeductionPage implements RenderHTML {
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Брисање обуставе");
      // $this->sectionContent();

      $this->closeSection();
   }
   /*
   function sectionContent() {
      echo str_repeat("\t", 5) . "<p>Садржај стране за брисање обуставе. НИЈЕ АКТИВНО!</p>\n";
      echo $this->backToListButton();
   }
   */
   
   public function execute() {
      DeductionDB::delete($this->did);
      $adr  = "Location: http://" . Application::getServerAddress() . "/oz/deduction.php";
      header($adr);
      exit;
   }
}
