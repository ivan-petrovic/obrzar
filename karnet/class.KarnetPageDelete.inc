<?php

require_once 'class.KarnetDB.inc';
require_once 'class.KarnetPage.inc';


class KarnetDelete extends KarnetPage implements RenderHTML {
   function renderHTML() {
      $this->openSection();
      $this->sectionContent();
      $this->closeSection();
   }

   public function execute() {
      KarnetDB::delete($this->eid, $this->pid);
      $adr  = "Location: http://" . Application::getServerAddress() . "/oz/karnet.php?opID=" . KarnetPageDispatcher::LIST_KARNET;
      header($adr);
      exit;
   }
}
