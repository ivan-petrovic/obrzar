<?php

require_once 'util/class.FormMaker.inc';

/*
 * Klasa koja formira formular za unos obustave.
 */
class DeductionForm extends FormMaker {
   public static function radnjaElement($label, $nameAndID, $value, $indent) {
      return ShopFormElement::shopElement($label, $nameAndID, $value, $indent);
   }
   public static function tip_placanjaElement($label, $nameAndID, $value, $indent) {
      return TipPlacanjaFormElement::tipPlacanjaElement($label, $nameAndID, $value, $indent);
   }
   public static function tip_obustaveElement($label, $nameAndID, $value, $indent) {
      return TipObustaveFormElement::tipObustaveElement($label, $nameAndID, $value, $indent);
   }

   public static function update(Deduction $d, $indent) {
      $output  = str_repeat("\t", $indent) . "<table class=\"izmena-rate\">\n";
      $output .= str_repeat("\t", $indent+1) . "<tr><th>Редни број</th><th>Износ</th><th>Месец</th><th>Година</th><th>Тип обрачуна</th></tr>\n";
      
      $i = 1;
      foreach ($d->rate as $rata) {
         $output .= str_repeat("\t", $indent+1) . "<tr>\n";
         $output .= str_repeat("\t", $indent+1) . "<td>$i<input type=\"hidden\" name=\"id[]\" value=\"{$rata->getValue('id')}\" /></td>\n";
         $output .= str_repeat("\t", $indent+1) . "<td><input type=\"text\" name=\"iznos[]\" value=\"{$rata->getValue('iznos')}\" /></td>\n";
         $output .= str_repeat("\t", $indent+1) . "<td>\n" . MesecFormElement::makeArr("mesec", $rata->getValue('mesec'), $indent+1);
         $output .= str_repeat("\t", $indent+1) . "</td>\n";
         $output .= str_repeat("\t", $indent+1) . "<td><input type=\"text\" name=\"godina[]\" value=\"{$rata->getValue('godina')}\" /></td>\n";
         $output .= str_repeat("\t", $indent+1) . "<td>\n" . TipObracunaFormElement::makeArr("tipObracuna", $rata->getValue('tipObracuna'), $indent+1);
         $output .= str_repeat("\t", $indent+1) . "</td>\n";
         $output .= str_repeat("\t", $indent+1) . "</tr>\n";
         ++$i;
      }

      $output .= str_repeat("\t", $indent) . "</table>\n";
      return $output;
   }
   
   public static function openForm($scriptName, $indent) {
      $output = str_repeat("\t", $indent) .  "<form action=\"$scriptName\" method=\"POST\">\n";
      return $output;
   }
   
   public static function closeForm1($submitLabel, $indent) {
      $output = self::submitElement($submitLabel, $indent+1);
      $output .= str_repeat("\t", $indent) . "</form>\n";
      return $output;
   }
}

class TipPlacanjaFormElement {
   protected static function tipPlacanjaDescription() {
      return array(
          1 => 'При свакој исплати',
          2 => 'Само на аконтацији',
          3 => 'Само на коначном обрачуну'
      );
   }

   public static function tipPlacanjaElement($label, $nameAndID, $value, $indent) {
      $output  = str_repeat("\t", $indent) . "<p>\n";
      $output .= str_repeat("\t", $indent) . "<label for=\"{$nameAndID}\">{$label}:</label>\n";
      $output .= str_repeat("\t", $indent) . "<select id=\"{$nameAndID}\" name=\"{$nameAndID}\">\n";
      
      foreach (self::tipPlacanjaDescription() as $key => $option) {
         $output .= str_repeat("\t", $indent+1) . "<option value=\"{$key}\"";
         $output .= ($value === $key ? " selected>" : ">") . "$option</option>\n";
      }

      $output .= str_repeat("\t", $indent) . "</select>\n";
      $output .= str_repeat("\t", $indent) . "</p>\n";
      return $output;
   }
   
   static public function getName($id) {
      $desc = self::tipPlacanjaDescription();
      if( array_key_exists($id, $desc) ) {
         $output = $desc[$id];
      } else {
         $output = "НИЈЕ ПРОНАЂЕН";
      }
      return $output;
   }
}

class TipObustaveFormElement {
   protected static function tipObustaveDescription() {
      // standardna u vise rata(1), procenat od neto zarade (2),...
      return array(
          1 => 'Стандардна',
          2 => 'Проценат од нето зараде',
      );
   }

   public static function tipObustaveElement($label, $nameAndID, $value, $indent) {
      $output  = str_repeat("\t", $indent) . "<p>\n";
      $output .= str_repeat("\t", $indent) . "<label for=\"{$nameAndID}\">{$label}:</label>\n";
      $output .= str_repeat("\t", $indent) . "<select id=\"{$nameAndID}\" name=\"{$nameAndID}\">\n";
      
      foreach (self::tipObustaveDescription() as $key => $option) {
         $output .= str_repeat("\t", $indent+1) . "<option value=\"{$key}\"";
         $output .= ($value === $key ? " selected>" : ">") . "$option</option>\n";
      }

      $output .= str_repeat("\t", $indent) . "</select>\n";
      $output .= str_repeat("\t", $indent) . "</p>\n";
      return $output;
   }
   
   static public function getName($id) {
      $desc = self::tipObustaveDescription();
      if( array_key_exists($id, $desc) ) {
         $output = $desc[$id];
      } else {
         $output = "НИЈЕ ПРОНАЂЕН";
      }
      return $output;
   }
}
