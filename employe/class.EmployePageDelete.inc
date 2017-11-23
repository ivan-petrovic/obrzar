<?php

require_once 'class.EmployeDB.inc';
require_once 'class.EmployePage.inc';

class EmployeDelete extends EmployePage implements RenderHTML {
   function renderHTML() {
      $this->openSection();
      $this->sectionHeader("Брисање запосленог");
      $this->closeSection();
   }

   public function execute() {
      EmployeDB::delete($this->eid);
      $adr  = "Location: http://" . Application::getServerAddress() . "/employe.php?opID=" . EmployePageDispatcher::LIST_EMPLOYE;
      header($adr);
      exit;
   }
}
