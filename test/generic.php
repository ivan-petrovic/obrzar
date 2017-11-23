<?php

require_once 'class.Employe.inc';

////////////////////////////////////////////////////////////////
//echo "<h1>Genericke klase</h1>\n";
//echo "<h3>Struktura za klasu Zaposleni</h3>\n";
//$es = new EmployeStructure;
//StructureWriter::write($es);
//echo ( $es->hasField("ime") ? "sadrzi " : "NE sadrzi " ) . "kljuc ime<br/>\n";
//echo ( $es->hasField("Ime") ? "sadrzi " : "NE sadrzi " ) . "kljuc Ime<br/>\n";
//echo ( $es->hasField("name") ? "sadrzi " : "NE sadrzi " ) . "kljuc name<br/>\n";
//
//$e = new Employe;
//echo "<h3>Default (\"prazan\") Zaposleni</h3>\n";
//DataWriter::write($e);
//echo "<h3>Tipovi podataka u default (\"praznom\") Zaposlenom</h3>\n";
//DataWriter::writeTypes($e);
//
//echo "<h3>get and sat value metodi</h3>\n";
//echo "Employe name: {$e->getValue('ime')}<br/>\n";
//// $e->setValue('ime', 'Ajvan');
//$e->setValue('id', -2);
//$e->setValue('ime', 'IVaN');
//$e->setValue('koeficijent', 5);
//echo "Employe name: {$e->getValue('ime')}<br/>\n";
//echo "<h3>Tipovi podataka u \"popunjenom\" Zaposlenom</h3>\n";
//DataWriter::writeTypes($e);
//DataWriter::write($e);
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//echo "<h3>Validacija podataka</h3>\n";
//$v = new Validator();
//echo "Podaci " . ($v->is_valid() ? "su" : "NISU") . " validni<br/>\n";
//$v->validate($es, $e);
//echo "Podaci " . ($v->is_valid() ? "su" : "NISU") . " validni<br/>\n";
//foreach ($v->getErrors() as $error) {
//   echo $error . "<br/>\n";
//}
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//if ($_SERVER['REQUEST_METHOD'] == 'POST') {
//   echo "<h1>Formular poslat</h1>\n";
//   echo "<p>Obradjujem...</p>\n";
//   
//   $e1 = new Employe;
//   $e1->setData($_POST);
//   if ($_POST) {
//     foreach ($_POST as $key => $value) {
//       echo "{$key}={$value}-" . gettype($value) . "<br/>\n";
//     }
//   }   
//   
//   $v = new Validator;
//   $v->validate($es, $e1);
//
//   if($v->is_valid()) {
//      echo "<p>Uspesan upis podataka</p>\n";
//      DataWriter::write($e1);
//   } else {
//      echo "<h3>Greske pri unosu su:</h3>\n";
//      foreach ($v->getErrors() as $error) {
//         echo $error . "<br/>\n";
//      }
//      echo "<h3>Formular za unos i izmenu</h3>\n";
//      echo FormMaker::form($es, $e1, "generic.php");
//   }
//} else {
//   echo "<h3>Formular za unos i izmenu</h3>\n";
//   echo FormMaker::form($es, $e, "generic.php");
//}
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//echo "<h2>Tipovi podataka - eksperimentisanje</h2>\n";
//$i = 20/5;
//echo "Naredba: \$i = 13;<br/>\n";
//echo "Tip varijable \$i: " . gettype($i). "<br/>\n";
//echo "Vrednost: {$i}<br/>\n";
//echo "Vrednost: ". number_format($i, 6) ."<br/>\n";
//
//$f = 20/3;
//echo "Naredba: \$f = 13.0;<br/>\n";
//echo "Tip varijable \$f: " . gettype($f). "<br/>\n";
//echo "Vrednost: {$f}<br/>\n";
//echo "Vrednost: ". number_format($f, 6) ."<br/>\n";
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//$a = "1bf2";
//echo "a = {$a}<br/>\n";
//echo "type(a) = " . gettype($a) . "<br/>\n";
//$sanitized_a = filter_var($a, FILTER_SANITIZE_NUMBER_INT);
//echo "sanitized_a = {$sanitized_a}<br/>\n";
//echo "type(sanitized_a) = " . gettype($sanitized_a) . "<br/>\n";
//settype($sanitized_a, "integer");
//echo "sanitized_a = {$sanitized_a}<br/>\n";
//echo "type(sanitized_a) = " . gettype($sanitized_a) . "<br/>\n";
//
//$b = 1;
//echo "b = $b<br/>\n";
//echo "type(b) = " . gettype($b) . "<br/>\n";
//settype($b, "string");
//echo "b = $b<br/>\n";
//echo "type(b) = " . gettype($b) . "<br/>\n";
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
echo "<h3>Struktura za klasu Zaposleni</h3>\n";
$es = new EmployeStructure;
StructureWriter::write($es);
echo ($es->hasField('id') ? "ima 'id'" : "NEMA 'id'") . "<br/>\n";
echo ($es->hasField('Id') ? "ima 'Id'" : "NEMA 'Id'") . "<br/>\n";
echo ($es->hasField('roki') ? "ima 'roki'" : "NEMA 'roki'") . "<br/>\n";
var_dump($es->getFieldDescriptor('id'));

$e = new Employe;
echo "<h3>Default (\"prazan\") Zaposleni</h3>\n";
DataWriter::writeWithTypes($e);

echo "<h3>setData metod</h3>\n";
$e->setData(array(
    'id'  => '13',
    'ime' => 'IvaN',
    'prezime' => '<strong>Petrovic</strong>',
    'koeficijent' => '5.70'
));
DataWriter::writeWithTypes($e);
////////////////////////////////////////////////////////////////
