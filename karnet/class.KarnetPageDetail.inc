<?php

require_once 'class.KarnetDB.inc';
require_once 'class.KarnetPage.inc';

class KarnetDetails extends KarnetPage implements RenderHTML {
   // Obracun ciji se detalji pregledaju.
   protected $karnet;
   
   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $pid, $eid, $indent) {
      parent::__construct($op, $fid, $pid, $eid, $indent);
      $this->karnet = KarnetDB::load($this->eid, $this->pid);
   }
   
   function renderHTML() {
      $this->openSection();
      
      $this->sectionHeader("Преглед карнета");
      $this->sectionContent();

      $this->closeSection();
   }
   
   function sectionContent() {
      echo KarnetRender::display($this->karnet, 5);
      echo $this->backToListButton();
      echo $this->updateButton();
   }
}

class KarnetRender extends CommonPage {

   /**
    * Funkcija za prikaz Karneta u HTML-u.
    * @param Karnet - karnet koja se prikazuje
    * @param integer indent - indentacija u ispisu zaposlenog
    * @return string
    */
   public static function display(Karnet $karnet, $indent) {
      $output = self::openTable("detalji", $indent);
      
      $nizel = self::displayElements();
      $cnt = count($nizel);
      for($i = 0; $i < $cnt; $i += 2) {
         $output .= self::makeRow($karnet, $nizel[$i], $nizel[$i+1], $indent+4);
      }
      $output .= self::closeTable($indent);
      
      return $output;
   }

   protected static function displayElements() {
      return array(
          'zaposleniID', 'Шифра запосленог', 'obracunID', 'Шифра обрачуна',
          'koeficijent', 'Коефицијент', 'satiRadni', 'Радни сати', 'tipCenaRada', 'Тип цене рада', 'cenaRada', 'Цена рада',
          'tipRadniUcinak', 'Тип радног учинка', 'iznosRadniUcinak', 'Радни учинак',
          'satiRadPraznik', 'Рад на државни празник сати', 'satiRadNoc', 'Рад ноћу сати',
          'satiRadPraznikNoc', 'Рад ноћу на државни празник', 'satiPrekovremeno', 'Рад прековремено',
          'satiSmena', 'Сменски рад', 'satiNedelja', 'Рад недељом',
          'koefMinuliRad', 'Минули рад', 'tipNagrada', 'Тип награде',
          'iznosNagrada', 'Награда', 'iznosOtpremnina', 'Отпремнина', 'iznosTOPoDanu', 'Топли оброк по дану',
          'daniTO', 'Топли оброк број дана',
          'iznosRegres', 'Регрес', 'iznosTerenskiDodatak', 'Теренски додадак', 'iznosPoklonDeci', 'Поклон деци',
          'opisOstalo', 'Опис ставке остало', 'iznosOstalo', 'Остало износ',
          'prosek3Meseca', 'Просек предходна три месеца', 'satiGO', 'Годишњи одмор сати',
          'satiPlacenoOdsustvo', 'Плаћено одсуство сати', 'satiOdsustvoPraznik', 'Одсустов на државни празник сати',
          'satiBolDo30D', 'Боловање до 30 дана сати', 'satiBolDo30D100Posto', 'Боловање до 30 дана са исплатом 100% сати',
          'satiBolPreko30D', 'Боловање преко 30 дана сати', 'satiPrekid', 'Прекид без кривице запосленог сати',
          'satiVojnaVezba', 'Војна вежба сати', 'satiPozivDrzOrgana', 'Позив државних органа', 'tipUmanjenja', 'Тип умањења (САМО за ЈП)');
   }

   protected static function specialElements() {
      return array(
          'zaposleniID' => "getZaposleniString",
          'obracunID' => "getObracunString",
          'tipRadniUcinak' => "getTipRadniUcinakString",
          'tipNagrada' => "getTipNagradaString",
          'tipCenaRada' => "getTipCenaRadaString",
          'tipUmanjenja' => "getTipUmanjenjaString",
      );
   }

   protected static function makeRow(Karnet $karnet, $el, $label, $indent) {
      $output  = str_repeat("\t", $indent);
      
      $specel = self::specialElements();
      if( array_key_exists($el, $specel) ) {
         $output .= "<tr><th>{$label}</th><td>" . self::$specel[$el]($karnet) . "</td></tr>\n";
      } else {
         $output .= "<tr><th>{$label}</th><td>" . $karnet->getValue($el) . "</td></tr>\n";
      }

      return $output;
   }

   /**
    * Funkcija za prikaz naziva meseca obracuna.
    * @return string
    */
   public static function getZaposleniString(Karnet $karnet) {
      $id = $karnet->getValue('zaposleniID');
      return EmployeDB::name($id);
   }

   /**
    * Funkcija za prikaz naziva banke.
    * @return string
    */
   public static function getObracunString(Karnet $karnet) {
      $id = $karnet->getValue('obracunID');
      // mozda Application::getCurrPaymentName - POGLEDATI
      return PaymentDB::name($id);
   }
   
   /**
    * Funkcija za prikaz naziva tipa radnog ucinka.
    * @return string
    */
   public static function getTipRadniUcinakString(Karnet $karnet) {
      return TipRadniUcinakFormElement::getName($karnet->getValue('tipRadniUcinak'));
   }
   
   /**
    * Funkcija za prikaz naziva tipa umanjenja.
    * @return string
    */
   public static function getTipUmanjenjaString(Karnet $karnet) {
      return TipUmanjenjaFormElement::getName($karnet->getValue('tipUmanjenja'));
   }

   /**
    * Funkcija za prikaz naziva banke.
    * @return string
    */
   public static function getTipNagradaString(Karnet $karnet) {
      return TipNagradaFormElement::getName($karnet->getValue('tipNagrada'));
   }

   /**
    * Funkcija za prikaz tipa cene rada.
    * @return string
    */
   public static function getTipCenaRadaString(Karnet $karnet) {
      return TipCenaRadaFormElement::getName($karnet->getValue('tipCenaRada'));
   }
}
