<?php

require_once 'class.EmployeDB.inc';
require_once 'class.EmployePage.inc';

class EmployeList extends EmployePage implements RenderHTML {
   const AKTIVNI_ZAPOSLENI = 2050;
   const PASIVNI_ZAPOSLENI = 2060;
   const SVI_ZAPOSLENI = 2070;
   
   protected $aktivan;

   /*
    * Konstruktor.
    */
   public function __construct($op, $fid, $eid, $indent) {
      parent::__construct($op, $fid, $eid, $indent);
      $this->aktivan = self::AKTIVNI_ZAPOSLENI;
   }

   public function renderHTML() {
      $this->openSection();

      $this->sectionHeader("Листа запослених");
      $this->sectionContent();

      $this->closeSection();
   }

   protected function sectionContent() {
      if ($_SERVER['REQUEST_METHOD'] == 'POST') {
         $this->processForm();
      }
      $this->renderForm();
   }

   protected function renderForm() {
      $nizZaposlenih = $this->getZaposleni();
      if( count($nizZaposlenih) === 0) {
         echo $this->makeForm();
         echo $this->getNemaZaposlenihString();
         echo $this->backToListButton();
      } else {
         echo $this->renderList($nizZaposlenih);
      }
      echo $this->newButton();
   }

   protected function getZaposleni() {
      if($this->aktivan === self::AKTIVNI_ZAPOSLENI) {
         return EmployeDB::loadAllAktivni($this->fid, Application::getOrderEmploye());
      }
      if($this->aktivan === self::PASIVNI_ZAPOSLENI) {
         return EmployeDB::loadAllPasivni($this->fid, Application::getOrderEmploye());
      }
      return EmployeDB::loadAll($this->fid, Application::getOrderEmploye());
   }

   protected function processForm() {
      $this->aktivan = filter_input(INPUT_POST, 'aktivni', FILTER_VALIDATE_INT);
   }
   
   protected function renderList($nizZaposlenih) {
      $output  = "";
      $output .= $this->makeForm();
      echo $this->newButtonGore(); // "<div>" . $this->newButton() . "</div>\n";
      $output .= $this->makeZaglavlje();
      
      foreach ($nizZaposlenih as $employe) {
         $output .= $this->makeRow($employe->getValue('id'), $employe->getValue('interniID'),
                 $employe->getValue('prezime'), $employe->getValue('ime'));
      }

      $output .= $this->closeTable(2);
      return $output;
   }

   protected function makeZaglavlje() {
      $output  = "";
      $output .= $this->getImaZaposlenihString();
      $output .= $this->openTable("spisak", 2);
      $output .= $this->makeTableZaglavlje();
      return $output;
   }

   protected function getNemaZaposlenihString() {
      if($this->aktivan === self::AKTIVNI_ZAPOSLENI) {
         return str_repeat("\t", self::$indent+2) . "<p>Нема унетих активних запослених.</p>\n";
      }
      if($this->aktivan === self::PASIVNI_ZAPOSLENI) {
         return str_repeat("\t", self::$indent+2) . "<p>Нема унетих пасивизираних запослених.</p>\n";
      }
      return str_repeat("\t", self::$indent+2) . "<p>Нема унетих запослених.</p>\n";
   }

   protected function getImaZaposlenihString() {
      if($this->aktivan === self::AKTIVNI_ZAPOSLENI) {
         return str_repeat("\t", self::$indent+2) . "<p>Унети су следећи запослени:</p>\n";
      }
      if($this->aktivan === self::PASIVNI_ZAPOSLENI) {
         return str_repeat("\t", self::$indent+2) . "<p>Унети су следећи пасивизирани запослени:</p>\n";
      }
      return str_repeat("\t", self::$indent+2) . "<p>Унети су следећи запослени:</p>\n";
   }

   protected function makeForm() {
      $output  = "";
      $scriptName = "employe.php?opID=" . $this->op . "&order=" . Application::getOrderEmploye();
      $output .= FormMaker::openForm($scriptName, 5);

      $output .= self::makeRadio($this->aktivan);

      $output .= FormMaker::submitElement('Примени', 6);
      
      $output .= FormMaker::closeForm(5);
      return $output;
   }

   protected static function makeRadio($aktivan) {
      $output  = "";
      $output .= str_repeat("\t", self::$indent+3);
      $output .= "<label><input type=\"radio\" name=\"aktivni\" value=\"" . self::AKTIVNI_ZAPOSLENI . "\"";
      $output .= ($aktivan === self::AKTIVNI_ZAPOSLENI ? " checked" : "") . ">Активни</label>\n";

      $output .= str_repeat("\t", self::$indent+3);
      $output .= "<label><input type=\"radio\" name=\"aktivni\" value=\"" . self::PASIVNI_ZAPOSLENI . "\"";
      $output .= ($aktivan === self::PASIVNI_ZAPOSLENI ? " checked" : "") . ">Пасвизирани</label>\n";
      
      $output .= str_repeat("\t", self::$indent+3);
      $output .= "<label><input type=\"radio\" name=\"aktivni\" value=\"" . self::SVI_ZAPOSLENI . "\"";
      $output .= $aktivan === self::SVI_ZAPOSLENI ? " checked>Сви</label>\n" : ">Сви</label>\n";
              
      return $output;
   }

   protected function makeTableZaglavlje() {
      $output  = "";
      $output .= str_repeat("\t", self::$indent+3) . "<tr>\n";
      
      $refsifra = "employe.php?opID=" . EmployePageDispatcher::LIST_EMPLOYE . "&order=sifra";
      $refprezime = "employe.php?opID=" . EmployePageDispatcher::LIST_EMPLOYE . "&order=prezime";
      $refime = "employe.php?opID=" . EmployePageDispatcher::LIST_EMPLOYE . "&order=ime";
      
      $output .= str_repeat("\t", self::$indent+3) . "<th><a href=\"{$refsifra}\">Шифра</a></th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th><a href=\"{$refprezime}\">Презиме</a></th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th><a href=\"{$refime}\">Име</a></th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "<th colspan=\"2\" class=\"non_print\">Акција</th>\n";
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }
   
   protected function makeRow($eid, $intID, $prezime, $ime) {
      $output  = str_repeat("\t", self::$indent+3) . "<tr>\n";
      $output .= $this->makeDetailsLink($eid, $intID);
      $output .= $this->makeDetailsLink($eid, $prezime);
      $output .= $this->makeDetailsLink($eid, $ime);
      $output .= $this->makeUpdateLink($eid);
      $output .= $this->makeDeleteLink($eid);
      $output .= str_repeat("\t", self::$indent+3) . "</tr>\n";
      return $output;
   }
   
   protected function makeDetailsLink($eid, $fnaz) {
      $ref = "employe.php?opID=" . EmployePageDispatcher::DETAILS_EMPLOYE . "&employeID=" . $eid;
      return $this->makeLink($ref, $fnaz);
   }

   protected function makeUpdateLink($eid) {
      $ref = "employe.php?opID=" . EmployePageDispatcher::UPDATE_EMPLOYE . "&employeID=" . $eid;
      return $this->makeLinkNonPrint($ref, "Измени");
   }

   protected function makeDeleteLink($eid) {
      $ref = "employe.php?opID=" . EmployePageDispatcher::DELETE_EMPLOYE . "&employeID=" . $eid;
      return $this->makeLinkNonPrint($ref, "Обриши");
   }
}
