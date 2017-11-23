<?php

require_once 'class.Data.inc';

class FormMaker {
   public static function make(Data $d, $scriptName, $submitLabel, $indent) {
      $output  = self::openForm($scriptName, $indent);
      $output .= self::formCore($d, $indent + 1);
      $output .= self::submitElement($submitLabel, $indent + 1);
      $output .= self::closeForm($indent);
      
      return $output;
   }
   
   public static function formCore(Data $d, $indent) {
      $output = "";
      
      foreach ($d->getStructure()->getFormDesc() as $fieldName => $fieldDesc) {
         $output .= str_repeat("\t", $indent) . "<p>\n";
         $output .= self::makeElement($fieldDesc[Structure::FORM_ELEMENT],
                 $fieldDesc[Structure::FORM_LABEL], $fieldName,
                 $d->getValue($fieldName), $indent);
         $output .= str_repeat("\t", $indent) . "</p>\n";
      }

      return $output;
   }

   public static function openForm($scriptName, $indent) {
      $output  = str_repeat("\t", $indent);
      $output .= "<form action=\"$scriptName\" method=\"POST\">\n";
      return $output;
   }

   public static function closeForm($indent) {
      $output = str_repeat("\t", $indent) . "</form>\n";
      return $output;
   }

   protected static function makeElement($formElement, $label, $nameAndID, $value, $indent) {
      $method = $formElement . "Element";
      return static::$method($label, $nameAndID, $value, $indent);
   }

   public static function submitElement($label, $indent) {
      return str_repeat("\t", $indent) . "<input type=\"submit\" value=\"{$label}\">\n";
   }

   public static function textElement($label, $nameAndID, $value, $indent) {
      $output  = str_repeat("\t", $indent) . "<label for=\"{$nameAndID}\">{$label}:</label>\n";
      $output .= str_repeat("\t", $indent) . "<input type=\"text\" name=\"{$nameAndID}\" ";
      $output .= "id=\"{$nameAndID}\" value=\"{$value}\" />\n";
      return $output;
   }
   
   public static function hiddenElement($label, $nameAndID, $value, $indent) {
      $output  = str_repeat("\t", $indent);
      $output .= "<input type=\"hidden\" name=\"{$nameAndID}\" ";
      $output .= "id=\"{$nameAndID}\" value=\"{$value}\" />\n";
      return $output;
   }

   public static function statusElement($label, $nameAndID, $value, $indent) {
      return StatusFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function polElement($label, $nameAndID, $value, $indent) {
      return PolFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function tip_firmeElement($label, $nameAndID, $value, $indent) {
      return TipFirmeFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function mesecElement($label, $nameAndID, $value, $indent) {
      return MesecFormElement::make($label, $nameAndID, $value, $indent);
   }
   
   public static function tip_obracunaElement($label, $nameAndID, $value, $indent) {
      return TipObracunaFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function tip_zaposlenjaElement($label, $nameAndID, $value, $indent) {
      return TipZaposlenjaFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function bankaElement($label, $nameAndID, $value, $indent) {
      return BankFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function opstinaElement($label, $nameAndID, $value, $indent) {
      return Municipality::make($label, $nameAndID, $value, $indent);
   }

   public static function strucna_spremaElement($label, $nameAndID, $value, $indent) {
      return EducationFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function tip_cena_radaElement($label, $nameAndID, $value, $indent) {
      return TipCenaRadaFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function tip_radni_ucinakElement($label, $nameAndID, $value, $indent) {
      return TipRadniUcinakFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function tip_umanjenjaElement($label, $nameAndID, $value, $indent) {
      return TipUmanjenjaFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function tip_nagradaElement($label, $nameAndID, $value, $indent) {
      return TipNagradaFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function zaposleniElement($label, $nameAndID, $value, $indent) {
      return EmployeFormElement::make($label, $nameAndID, $value, $indent);
   }

   public static function printErrors($errorsMap, $indent) {
      $output = str_repeat("\t", $indent) . "<div class=\"error\">\n";
      
      foreach ($errorsMap as $fieldName => $errors) {
         $output .= self::printErrorForField($fieldName, $errors, $indent+1);
      }
      
      $output .= str_repeat("\t", $indent) . "</div>\n";
      return $output;
   }
   
   protected static function printErrorForField($fieldName, $errors, $indent) {
      $output = "";
      foreach ($errors as $error) {
         $output .= str_repeat("\t", $indent) . "<p>{$fieldName}: {$error}</p>\n";
      }
      return $output;
   }
}

class FormElement {
   static protected function openSelectElement($label, $nameAndID, $indent) {
      $output  = str_repeat("\t", $indent) . "<label for=\"{$nameAndID}\">{$label}:</label>\n";
      $output .= str_repeat("\t", $indent) . "<select id=\"{$nameAndID}\" name=\"{$nameAndID}\">\n";
      return $output;
   }

   static protected function openSelectElementArr($nameAndID, $indent) {
      $output = str_repeat("\t", $indent) . "<select id=\"{$nameAndID}\" name=\"{$nameAndID}[]\">\n";
      return $output;
   }

   static protected function closeSelectElement($indent) {
      $output  = str_repeat("\t", $indent) . "</select>\n";
      return $output;
   }
}

class Municipality extends FormElement {
   /*
    * Ucitaj sve opstine iz baze.
    * @return msqli $result object
    */
   static protected function loadAll() {
      $select = new Select();
      $select->what(array('id', 'naziv'));
      $select->from('municipality');
      $result = $select->execute();
      return $result;
   }
   
   static public function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      
      $rows = self::loadAll();
      while($row = $rows->fetch_assoc()) {
         $output .= str_repeat("\t", $indent+1);
         $output .= "<option value=\"{$row['id']}\" " . ($value === (int)$row['id'] ? "selected" : "");
         $output .= ">{$row['naziv']}</option>\n";
      }

      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   static public function getName($id) {
      $select = new Select(); $select->what(array('naziv'));
      $select->from('municipality');
      $select->condition("id = ?", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();
      if($row) {
         return $row['naziv'];
      } else {
         trigger_error("Municipality::getName(): $id not found.");
      }
   }
}

class BankFormElement extends FormElement {
   /*
    * Ucitaj sve banke iz baze.
    * @return msqli $result object
    */
   static protected function loadAll() {
      $select = new Select();
      $select->what(array('id', 'naziv'));
      $select->from('bank');
      $result = $select->execute();
      return $result;
   }
   
   static public function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      // default no bank option
      $output .= str_repeat("\t", $indent+1) . "<option value=\"0\" " . ($value === 0 ? "selected" : "");
      $output .= ">Није изабрана</option>\n";
      
      $rows = self::loadAll();
      while($row = $rows->fetch_assoc()) {
         $output .= str_repeat("\t", $indent+1);
         $output .= "<option value=\"{$row['id']}\" " . ($value === (int)$row['id'] ? "selected" : "") . ">{$row['naziv']}</option>\n";
      }

      $output .= self::closeSelectElement($indent);
      return $output;
   }

   static public function getName($id) {
      $select = new Select(); $select->what(array('naziv'));
      $select->from('bank');
      $select->condition("id = ?", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();
      if($row) {
         return $row['naziv'];
      } else {
         // trigger_error("Bank::getName(): $id not found.");
         return "Није изабрана";
      }
   }
}

class EducationFormElement  extends FormElement {
   protected static function educationDescription() {
      return array(
          0 => 'Није изабрана',
          1 => 'I степен',
          2 => 'II степен',
          3 => 'III степен',
          4 => 'IV степен',
          5 => 'V степен',
          6 => 'VI степен',
          7 => 'VII степен',
      );
   }

   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      
      foreach (self::educationDescription() as $key => $option) {
         $output .= str_repeat("\t", $indent+1) . "<option value=\"{$key}\"";
         $output .= ($value === $key ? " selected>" : ">") . "$option</option>\n";
      }

      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function getName($id) {
      $desc = self::educationDescription();
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕНА";
   }
}

class MesecFormElement extends FormElement {
   protected static function mesecDescription() {
      return array(
          1 => 'Јануар', 2 => 'Фебруар', 3 => 'Март',
          4 => 'Април',  5 => 'Мај', 6 => 'Јун',
          7 => 'Јул', 8 => 'Август', 9 => 'Септембар',
          10 => 'Октобар', 11 => 'Новембар',
          12 => 'Децембар');
   }

   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function makeArr($nameAndID, $value, $indent) {
      $output  = self::openSelectElementArr($nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }

   public static function core($value, $indent) {
      $output = "";
      foreach (self::mesecDescription() as $key => $option) {
         $output .= str_repeat("\t", $indent+1) . "<option value=\"{$key}\"";
         $output .= ($value === $key ? " selected>" : ">") . "$option</option>\n";
      }
      return $output;
   }
   
   public static function getName($id) {
      $desc = self::mesecDescription();
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}

class TipObracunaFormElement extends FormElement {
   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function makeArr($nameAndID, $value, $indent) {
      $output  = self::openSelectElementArr($nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }

   public static function core($value, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"Akontacija\" ";
      $output .= ($value === "Akontacija" ? "selected" : "") . ">Аконтација</option>\n";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"Konacni\" ";
      $output .= ($value === "Konacni" ? "selected" : "") . ">Коначни обрачун</option>\n";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"LZPred\" ";
      $output .= ($value === "LZPred" ? "selected" : "") . ">Лична зарада предузетника</option>\n";
      return $output;
   }

   public static function getName($id) {
      $desc = array( 'Akontacija' => 'Аконтација',
          'Konacni' => 'Коначни обрачун',
          'LZPred' => 'Лична зарада предузетника');
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}

class TipCenaRadaFormElement extends FormElement {
   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function core($value, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"1\" ";
      $output .= ($value === 1 ? "selected" : "") . ">Бруто</option>\n";      
      $output .= str_repeat("\t", $indent+1) . "<option value=\"2\" ";
      $output .= ($value === 2 ? "selected" : "") . ">Нето</option>\n";      
      return $output;
   }

   public static function getName($id) {
      $desc = array( 1 => 'Бруто', 2 => 'Нето');
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}

class TipFirmeFormElement extends FormElement {
   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function core($value, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"1\" ";
      $output .= ($value === 1 ? "selected" : "") . ">Јавно предузеће које се не финансира из буџета</option>\n";      
      $output .= str_repeat("\t", $indent+1) . "<option value=\"2\" ";
      $output .= ($value === 2 ? "selected" : "") . ">Предузетник (приватни послодавац)</option>\n";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"3\" ";
      $output .= ($value === 3 ? "selected" : "") . ">Јавно предузеће које се финансира из буџета</option>\n";      
      $output .= str_repeat("\t", $indent+1) . "<option value=\"4\" ";
      $output .= ($value === 4 ? "selected" : "") . ">Физичко лице</option>\n";      
      $output .= str_repeat("\t", $indent+1) . "<option value=\"5\" ";
      $output .= ($value === 5 ? "selected" : "") . ">Страно представништво</option>\n";      

      return $output;
   }

   public static function getName($id) {
      $desc = array(
          1 => 'ЈП које се не финансира из буџета',
          2 => 'Предузетник (приватни послодавац)',
          3 => 'ЈП које се финансира из буџета',
          4 => 'Физичко лице',
          5 => 'Страно представништво');
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}

class StatusFormElement extends FormElement {
   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function core($value, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"Aktivan\" ";
      $output .= ($value === "Aktivan" ? "selected" : "") . ">Активан</option>\n";      
      $output .= str_repeat("\t", $indent+1) . "<option value=\"Pasivan\" ";
      $output .= ($value === "Pasivan" ? "selected" : "") . ">Пасиван</option>\n";      
      return $output;
   }

   public static function getName($id) {
      $desc = array( 'Aktivan' => 'Активан', 'Pasivan' => 'Пасиван');
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}

class PolFormElement extends FormElement {
   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function core($value, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"m\" ";
      $output .= ($value === "m" ? "selected" : "") . ">Мушки</option>\n";      
      $output .= str_repeat("\t", $indent+1) . "<option value=\"f\" ";
      $output .= ($value === "f" ? "selected" : "") . ">Женски</option>\n";      
      return $output;
   }

   public static function getName($id) {
      $desc = array( 'm' => 'Мушки', 'f' => 'Женски');
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}

class TipRadniUcinakFormElement extends FormElement {
   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function core($value, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"1\" ";
      $output .= ($value === 1 ? "selected" : "") . ">Проценат зараде за редован рад</option>\n";      
      $output .= str_repeat("\t", $indent+1) . "<option value=\"2\" ";
      $output .= ($value === 2 ? "selected" : "") . ">Фиксни износ</option>\n";      
      return $output;
   }

   public static function getName($id) {
      $desc = array(
          1 => 'Проценат зараде за редован рад',
          2 => 'Фиксни износ');
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}

class TipUmanjenjaFormElement extends FormElement {
   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function core($value, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"1\" ";
      $output .= ($value === 1 ? "selected" : "") . ">Стандардно</option>\n";      
      $output .= str_repeat("\t", $indent+1) . "<option value=\"2\" ";
      $output .= ($value === 2 ? "selected" : "") . ">Гранични случај 25000</option>\n";      
      return $output;
   }

   public static function getName($id) {
      $desc = array(
          1 => 'Стандардно',
          2 => 'Гранични случај 25000');
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}

class TipNagradaFormElement extends FormElement {
   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function core($value, $indent) {
      $output  = "";
      $output .= str_repeat("\t", $indent+1) . "<option value=\"1\" ";
      $output .= ($value === 1 ? "selected" : "") . ">Фиксни износ</option>\n";      
      $output .= str_repeat("\t", $indent+1) . "<option value=\"2\" ";
      $output .= ($value === 2 ? "selected" : "") . ">Неки други начин...</option>\n";      
      return $output;
   }

   public static function getName($id) {
      $desc = array(
          1 => 'Фиксни износ',
          2 => 'Неки други начин...');
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}

class EmployeFormElement extends FormElement {
   static public function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::core($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }

   static public function makeAktivni($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      $output .= self::coreAktivni($value, $indent);
      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   protected static function core($value, $indent) {
      $output  = "";
      $rows = EmployeDB::loadAll(Application::getCurrFirmID());
      
      foreach($rows as $employe) {
         $output .= self::makeRow($employe, $value, $indent+1);
      }
      return $output;
   }

   protected static function coreAktivni($value, $indent) {
      $output  = "";
      $rows = EmployeDB::loadAllAktivni(Application::getCurrFirmID());
      
      foreach($rows as $employe) {
         $output .= self::makeRow($employe, $value, $indent+1);
      }
      return $output;
   }
   
   protected static function makeRow(Employe $employe, $value, $indent) {
      $id = $employe->getValue('id');
      $ime = $employe->getValue('prezime') . " " . $employe->getValue('ime');
      
      $output = str_repeat("\t", $indent+1);
      $output .= "<option value=\"{$id}\" " . ($value === $id ? "selected" : "") . ">{$ime}</option>\n";
      return $output;
   }
}

class ShopFormElement extends FormElement {
   /*
    * Ucitaj sve radnje iz baze.
    * @return msqli $result object
    */
   static public function loadAll() {
      $select = new Select();
      $select->what(array('id', 'naziv'));
      $select->from('shop');
      $result = $select->execute();
      return $result;
   }
   
   static public function shopElement($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      
      $rows = self::loadAll();
      while($row = $rows->fetch_assoc()) {
         $output .= str_repeat("\t", $indent+1);
         $output .= "<option value=\"{$row['id']}\" " . ($value === (int)$row['id'] ? "selected" : "") . ">{$row['naziv']}</option>\n";
      }

      $output .= self::closeSelectElement($indent);
      return $output;
   }

   static public function getName($id) {
      $select = new Select(); $select->what(array('naziv'));
      $select->from('shop');
      $select->condition("id = ?", array($id));
      $result = $select->execute();
      $row = $result->fetch_assoc();
      if($row) {
         return $row['naziv'];
      } else {
         trigger_error("Shop::getName with id $id not found.");
      }
   }
}

class TipZaposlenjaFormElement extends FormElement {
   protected static function tipZaposlenjaDescription() {
      return array(
          1 => 'Запослени',
          2 => 'Оснивач и чланови привредног друштва запослени у свом привредном друштву',
          3 => 'Лице осигурано по основу самосталне делатности укључујући самосталне уметнике',
          4 => 'Лице осигурано по основу пољопривредне делатности',
          5 => 'Лице које није осугурано по другом основу',
          6 => 'Нерезидент',
          7 => 'Инвалидно лице',
          8 => 'Војни осигураник',
          9 => 'Лице пензионер по основу запослености',
          10 => 'Лице пензионер по основу самосталне делатности',
          11 => 'Лице коме се исплаћују приходи ван радног осноса на које се не обрачунавају и не плаћају доприноси за обавезно социјално осигурање',
          12 => 'Војни пензионер',
          13 => 'Пољопривредни пензионер',
      );
   }

   public static function make($label, $nameAndID, $value, $indent) {
      $output  = self::openSelectElement($label, $nameAndID, $indent);
      
      foreach (self::tipZaposlenjaDescription() as $key => $option) {
         $output .= str_repeat("\t", $indent+1) . "<option value=\"{$key}\"";
         $output .= ($value === $key ? " selected>" : ">") . "$option</option>\n";
      }

      $output .= self::closeSelectElement($indent);
      return $output;
   }
   
   public static function getName($id) {
      $desc = self::tipZaposlenjaDescription();
      return array_key_exists($id, $desc) ? $desc[$id] : "НИЈЕ ПРОНАЂЕН";
   }
}