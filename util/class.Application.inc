<?php

class Application {
   /**
    * Pomocna f-ja za proveru da li je u aplikaciji izabrana tekuca firma.
    */
   static public function issetFirm() {
      return isset($_SESSION['currFirmID']);
   }

   /**
    * Pomocna f-ja za setovanje tekuce firme.
    */
   static public function setFirm(Firm $firm) {
      self::unsetPayment();
      $_SESSION['currFirmID'] = $firm->getValue('id');
      $_SESSION['currFirmName'] = $firm->getValue('naziv');
      $_SESSION['currFirmAddress'] = $firm->getValue('adresa');
      $_SESSION['currFirmCity'] = $firm->getValue('mesto');
      $_SESSION['currFirmPhone'] = $firm->getValue('telefon');
      $_SESSION['currFirmType'] = $firm->getValue('tip');
      $_SESSION['currFirmMatbr'] = $firm->getValue('matbr');
      $_SESSION['currFirmPIB'] = $firm->getValue('pib');
      $_SESSION['currFirmDel'] = $firm->getValue('sifdel');
      $_SESSION['currFirmAccountNum'] = $firm->getValue('ziroracun');
      $_SESSION['currFirmBankName'] = $firm->getValue('nazivBanke');
   }
   
   /**
    * Pomocna f-ja koja vraca ID tekuce firme, ili 0 ako nije postavljena.
    */
   static public function getCurrFirmID() {
      if(self::issetFirm() ) {
         return $_SESSION['currFirmID'];
      }
      return 0;
   }
   
   /**
    * Pomocna f-ja koja vraca naziv tekuce firme, ili prazan string, tj. "-".
    */
   static public function getCurrFirmName() {
      if(self::issetFirm() ) {
         return $_SESSION['currFirmName'];
      }
      return "Фирма: /није изабрана/";
   }

   /**
    * Pomocna f-ja koja vraca trazeni podatak o tekucoj firmi, ili prazan string.
    * @param string prop - property koja se trazi
    */
   static public function getCurrFirmProp($prop) {
      if(self::issetFirm() ) {
         if(isset($_SESSION[$prop])) {
            return $_SESSION[$prop];
         }
      }
      return "";
   }

   /**
    * Pomocna f-ja za proveru da li je u aplikaciji izabran tekuci obracun.
    */
   static public function issetPayment() {
      return isset($_SESSION['currPaymentID']);
   }

   /**
    * Pomocna f-ja za setovanje tekuceg obracuna.
    */
   static public function setPayment($paymentID, $paymentName) {
      $_SESSION['currPaymentID'] = $paymentID;
      $_SESSION['currPaymentName'] = $paymentName;
   }

   /**
    * Pomocna f-ja za unsetovanje tekuceg obracuna.
    */
   static public function unsetPayment() {
      if( self::issetPayment() ) {
         unset($_SESSION['currPaymentID']);
         unset($_SESSION['currPaymentName']);
      }
   }

   /**
    * Pomocna f-ja koja vraca ID tekuceg obracuna, ili 0 ako nije postavljen.
    */
   static public function getCurrPaymentID() {
      if(self::issetFirm() ) {
         return $_SESSION['currPaymentID'];
      }
      return 0;
   }
   
   static public function getCurrPaymentName() {
      if(self::issetPayment() ) {
         return $_SESSION['currPaymentName'];
      }
      return "Обрачун: /није изабран/";
   }
   
   static public function getServerAddress() {
      // return "localhost";
      // return "st3";
       return "https://quiet-sierra-71458.herokuapp.com";
   }

   static public function getOp() {
      return filter_input(INPUT_GET, 'opID', FILTER_VALIDATE_INT, array('options' => array('default' => 0)));
   }

   static public function getFirmID() {
      return filter_input(INPUT_GET, 'firmID', FILTER_VALIDATE_INT, array('options' => array('default' => 0)));
   }

   static public function getEmployeID() {
      return filter_input(INPUT_GET, 'employeID', FILTER_VALIDATE_INT, array('options' => array('default' => 0)));
   }

   static public function getOrderEmploye() {
      $order = filter_input(INPUT_GET, 'order', FILTER_SANITIZE_SPECIAL_CHARS);
      $allowed = array('sifra', 'prezime', 'ime');
      return in_array($order, $allowed) ? $order : 'sifra';
   }
   
   static public function getBankID() {
      return filter_input(INPUT_GET, 'bankID', FILTER_VALIDATE_INT, array('options' => array('default' => 0)));
   }

   static public function getDeductionID() {
      return filter_input(INPUT_GET, 'deductionID', FILTER_VALIDATE_INT, array('options' => array('default' => 0)));
   }

   static public function getShopID() {
      return filter_input(INPUT_GET, 'shopID', FILTER_VALIDATE_INT, array('options' => array('default' => 0)));
   }

   static public function getPaymentID() {
      return filter_input(INPUT_GET, 'paymentID', FILTER_VALIDATE_INT, array('options' => array('default' => 0)));
   }
}
