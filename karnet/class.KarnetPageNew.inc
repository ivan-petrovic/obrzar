<?php

require_once 'employe/class.EmployeDB.inc';
require_once 'payment/class.PaymentDB.inc';
require_once 'class.KarnetDB.inc';
require_once 'class.KarnetPage.inc';
require_once 'util/class.Validator.inc';

class KarnetNew extends KarnetPage implements RenderHTML {
   // Message generated after processing form;
   protected $message = "";
   
   function renderHTML() {
      $this->openSection();
      $this->sectionContent();
      $this->closeSection();
   }

   public function execute() {
      if($_SERVER['REQUEST_METHOD'] == 'POST') {
         $eid = filter_input(INPUT_POST, 'zaposleni', FILTER_VALIDATE_INT,
                 array('options' => array('default' => 0)));

         if($eid === 0) {
            $this->processForm();
         } else {
            $this->eid = $eid;
         }
      }
   }

   function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
//         $this->eid = filter_input(INPUT_POST, 'zaposleni', FILTER_VALIDATE_INT,
//                 array('options' => array('default' => 0)));

         if($this->eid === 0) {
            echo $this->message;
            echo $this->backToListButton();
         } else {
            $this->renderForm();
         }
      } else {
         $this->sectionHeader("Избор запосленог");
         $this->chooseEmploye();
      }      
   }

   function chooseEmploye() {
      $scriptName = "karnet.php?opID=" . $this->op;
      echo str_repeat("\t", self::$indent+2) .  "<form action=\"$scriptName\" method=\"POST\">\n";
      echo EmployeFormElement::makeAktivni("Запослени", "zaposleni", 0, self::$indent+3);
      echo FormMaker::submitElement("Изабери", self::$indent+3);
      echo str_repeat("\t", self::$indent+2) . "</form>\n";
   }

   function renderForm() {
      $employe = EmployeDB::load($this->eid);
      $ime = $employe->getValue('prezime') . " " . $employe->getValue('ime');
      $payment = PaymentDB::load($this->pid);
      
      if(KarnetDB::exist($this->eid, $this->pid)) {
         $karnet = KarnetDB::load($this->eid, $this->pid);
         $this->sectionHeader("Измена карнета за запосленог: " . $ime);
      } else {
         $karnet = new Karnet($payment, $employe);
         $this->sectionHeader("Унос новог карнета за запосленог: " . $ime);
      }

      echo FormMaker::make($karnet, "karnet.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   function processForm() {
      $karnet = new Karnet();
      $karnet->setData($_POST);

      $v = new Validator;
      $v->validate($karnet);

      if($v->is_valid()) {
         $this->save($karnet);
      } else {
         $this->message .= FormMaker::printErrors($v->getErrors(), self::$indent+2);
         $this->message .= FormMaker::make($karnet, "karnet.php?opID=" . $this->op, "Упис", self::$indent+2);
      }
   }

   protected function save(Karnet $karnet) {
      $eid = $karnet->getValue('zaposleniID');
      $pid = $karnet->getValue('obracunID');
      
      if(KarnetDB::exist($eid, $pid)) {
         KarnetDB::update($karnet);
      } else {
         KarnetDB::insert($karnet);
      }
      
      // KarnetDB::insert($karnet);
      $adr  = "Location: http://" . Application::getServerAddress() . "/karnet.php?opID=" . KarnetPageDispatcher::DETAILS_PRINT;
      $adr .= "&employeID=" . $eid;
     
      header($adr);
      exit;
   }
}
