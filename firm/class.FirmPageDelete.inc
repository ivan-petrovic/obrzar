<?php

require_once 'class.FirmDB.inc';
require_once 'class.FirmPage.inc';

class FirmDelete extends FirmPage implements RenderHTML {
   public function renderHTML() {
      $this->openSection();
      $this->sectionHeader("Брисање фирме");
      $this->closeSection();
   }
   
   public function execute() {
      FirmDB::delete($this->fid);
      $adr  = "Location: http://" . Application::getServerAddress() . "/firm.php?opID=" . FirmPageDispatcher::LIST_FIRM;
      header($adr);
      exit;
   }
}
