<?php

require_once 'class.PaymentDB.inc';
require_once 'class.PaymentPage.inc';

class PaymentDetails extends PaymentPage implements RenderHTML {
   // Obracun ciji se detalji pregledaju.
   protected $payment;
   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $pid, $indent) {
      parent::__construct($op, $fid, $pid, $indent);
      $this->payment = PaymentDB::load($this->pid);

      Application::setPayment($pid, $this->payment->getName());
   }
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Преглед обрачуна");
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      echo PaymentRender::display($this->payment, 5);
      echo $this->backToListButton();
      echo $this->updateButton();
      echo $this->updateCenaRadaUKarnetu();
   }
}

class PaymentRender extends CommonPage {

   /**
    * Funkcija za prikaz Obracuna u HTML-u.
    * @param Payment - obracun koja se prikazuje
    * @param integer indent - indentacija u ispisu zaposlenog
    * @return string
    */
   public static function display(Payment $payment, $indent) {
      $output = self::openTable("detalji", $indent);
      
      $nizel = self::displayElements();
      $cnt = count($nizel);
      for($i = 0; $i < $cnt; $i += 2) {
         $output .= self::makeRow($payment, $nizel[$i], $nizel[$i+1], $indent+4);
      }
      $output .= self::closeTable($indent);
      
      return $output;
   }
   
//   id, mesec, godina, tipObracuna, firmaID, datumIsplate, cenaRada, satiUMesecu,
//   koefRadPraznik, koefRadNoc, koefRadPrekovremeno, iznosTOPoDanu, iznosRegres,
//   iznosPoklonDeci, koefBolDo30D, koefPrekid, stopaPoreza, iznosNeoporezivo, stopaPIO,
//   stopaZdravstvo, stopaNezaposlenost, stopaPIOPoslodavac, stopaZdravstvoPoslodavac,
//   stopaNezaposlenostPoslodavac, minOsnovicaDoprinosa, iznosBrutoZarada, iznosMinZarada
   protected static function displayElements() {
      return array(
          'id', 'Шифра', 'mesec', 'Месец', 'godina', 'Година', 'tipObracuna', 'Тип обрачуна',
          'firmaID', 'Шифра фирме', 'datumIsplate', 'Датум исплате','tipCenaRada', 'Тип цене рада',
          'cenaRada', 'Цена рада', 'satiUMesecu', 'Број сати у месецу',
          'koefRadPraznik', 'Коефицијент за рад на државни празник', 'koefRadNoc', 'Коефицијент за ноћни рад',
          'koefRadPrekovremeno', 'Коефицијент за прековремени рад', 'koefRadSmena', 'Коефицијент за сменски рад',
          'koefRadNedelja', 'Коефицијент за рад недељом', 'iznosTOPoDanu', 'Топли оброк по дану',
          'iznosRegres', 'Регрес', 'iznosPoklonDeci', 'Поклон деци',
          'koefBolDo30D', 'Коефицијент за боловање до 30 дана', 'koefPrekid', 'Коефицијент за прекид рада',
          'stopaPoreza', 'Стопа пореза', 'iznosNeoporezivo', 'Неопорезиви износ', 'stopaPIO', 'Стопа доприноса за ПИО',
          'stopaZdravstvo', 'Стопа доприноса за здравство', 'stopaNezaposlenost', 'Стопа доприноса за незапосленост',
          'stopaPIOPoslodavac', 'Стопа доприноса за ПИО на терет послодавца',
          'stopaZdravstvoPoslodavac', 'Стопа доприноса за здравство на терет послодавца',
          'stopaNezaposlenostPoslodavac', 'Стопа доприноса за незапосленост на терет послодавца',
          'minOsnovicaDoprinosa', 'Минимална основица доприноса',
          'iznosBrutoZarada', 'Бруто зарада у републици', 'iznosMinZarada', 'Минимална зарада по сату');
   }

   protected static function specialElements() {
      return array(
          'mesec' => "getMesecString",
          'tipObracuna' => "getTipObracunaString",
          'tipCenaRada' => "getTipCenaRadaString"
      );
   }
   
   protected static function makeRow(Payment $payment, $el, $label, $indent) {
      $output  = str_repeat("\t", $indent);
      
      $specel = self::specialElements();
      if( array_key_exists($el, $specel) ) {
         $output .= "<tr><th>{$label}</th><td>" . self::$specel[$el]($payment) . "</td></tr>\n";
      } else {
         $output .= "<tr><th>{$label}</th><td>" . $payment->getValue($el) . "</td></tr>\n";
      }

      return $output;
   }

   /**
    * Funkcija za prikaz naziva meseca obracuna.
    * @return string
    */
   public static function getMesecString(Payment $payment) {
      return MesecFormElement::getName($payment->getValue('mesec'));
   }

   /**
    * Funkcija za prikaz naziva banke.
    * @return string
    */
   public static function getTipObracunaString(Payment $payment) {
      return TipObracunaFormElement::getName($payment->getValue('tipObracuna'));
   }

   /**
    * Funkcija za prikaz tipa cene rada.
    * @return string
    */
   public static function getTipCenaRadaString(Payment $payment) {
      return TipCenaRadaFormElement::getName($payment->getValue('tipCenaRada'));
   }
}
