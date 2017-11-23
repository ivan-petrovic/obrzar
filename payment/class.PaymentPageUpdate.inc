<?php

require_once 'class.PaymentDB.inc';
require_once 'class.PaymentPage.inc';
require_once 'util/class.Validator.inc';

class PaymentUpdate extends PaymentPage implements RenderHTML {
   // Message generated after processing form;
   protected $message = "";
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Измена обрачуна");
      $this->sectionContent();

      $this->closeSection();
   }

   protected function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         echo $this->message;
         echo $this->backToListButton();
      } else {
         $this->renderForm();
      }
   }

   public function execute() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      }
   }
   
   function renderForm() {
      $payment = PaymentDB::load($this->pid);
      echo FormMaker::make($payment, "payment.php?opID=" . $this->op, "Упис", self::$indent+2);
      echo $this->backToListButton();
   }

   function processForm() {
      $payment = new Payment();
      $payment->setData($_POST);

      $v = new Validator;
      $v->validate($payment);

      if($v->is_valid()) {
         $this->save($payment);
         // echo $this->backToListButton();
      } else {
         $this->message .= FormMaker::printErrors($v->getErrors(), self::$indent + 2);
         $this->message .= FormMaker::make($payment, "payment.php?opID=" . $this->op, "Упис", self::$indent + 2);
      }
   }

   protected function save(Payment $payment) {
      $pid = $payment->getValue('id');
      PaymentDB::update($payment);
      $adr  = "Location: http://" . Application::getServerAddress() . "/payment.php?opID=" . PaymentPageDispatcher::DETAILS_PAYMENT;
      $adr .= "&paymentID=" . $pid;
      header($adr);
      exit;
   }
}
