<?php
////////////////////////////////////////////////
// Poredjenje stringova
////////////////////////////////////////////////
echo "<!DOCTYPE html>\n";
echo "<html>\n";
echo "   <head>\n";
echo "      <title>Обрачун зарада</title>\n";
echo "      <meta charset=\"UTF-8\">\n";
echo "      <link rel=\"stylesheet\" href=\"../common/style.css\">\n";
echo "   </head>\n";
echo "   <body>\n";

$str1 = "Петровић";
$str2 = "Ševo";
echo "$str1 је " . (strcmp($str1, $str2) > 0 ? "већи" : "мањи") . " од $str2.";
echo "   </body>\n";

////////////////////////////////////////////////
// DB test
////////////////////////////////////////////////
//require_once 'class.Employe.inc';
//require_once 'class.EmployeDB.inc';
//require_once 'class.EmployeForm.inc';
//require_once 'class.Validator.inc';
//
//if ($_SERVER['REQUEST_METHOD'] == 'POST') {
//   echo "<h3>Formular poslat</h3>\n";
//   echo "<p>Obradjujem...</p>\n";
//   
//   $e1 = new Employe;
//   $e1->setData($_POST);
//   if ($_POST) {
//     foreach ($_POST as $key => $value) {
//       echo "{$key}={$value}-" . gettype($value) . "<br/>\n";
//     }
//   }
//   DataWriter::writeWithTypes($e1);
//   $v = new Validator;
//   $v->validate($e1);
//
//   if($v->is_valid()) {
//      EmployeDB::update($e1);
//      // EmployeDB::insert($e1);
//      echo "<p>Uspesan upis podataka</p>\n";
//   } else {
//      echo "<h3>Greske pri unosu su:</h3>\n";
//      foreach ($v->getErrors() as $error) {
//         echo $error . "<br/>\n";
//      }
//      echo "<h3>Formular za unos i izmenu</h3>\n";
//      echo EmployeForm::form($e1, "test.php");
//   }
//} else {
//
//   echo "<h3>Formular za unos i izmenu</h3>\n";
//   $e = EmployeDB::load(2);
//   DataWriter::writeWithTypes($e);
//   echo EmployeForm::form($e, "test.php");
//   // var_dump($e->getFieldNames());
//   // var_dump($e->getFieldValues());
//}

//$e = EmployeDB::load(4);
//DataWriter::writeWithTypes($e);
//
//$nize = EmployeDB::loadAll();
//foreach ($nize as $emp) {
//   echo "{$emp->getValue('ime')} {$emp->getValue('prezime')} <br/>\n";
//}

//require_once 'class.Delete.inc';
//$delete = new Delete();
//$delete->where('temp');
//$delete->condition('id = ?');
//$delete->values(array(8));
//echo $delete;
//$delresult = $delete->execute();
//var_dump($delresult);

//require_once 'class.Update.inc';
//$update = new Update();
//$update->what(array('ime', 'koeficijent'));
//$update->where('temp');
//$update->condition('id = ?');
//$update->values(array('Ivan', 5.7, 2));
//echo $update;
//$updresult = $update->execute();
//var_dump($updresult);

//require_once 'class.Insert.inc';
//$insert = new Insert();
//$insert->what(array('ime', 'prezime', 'status', 'koeficijent'));
//$insert->where('temp');
//$insert->values(array('Zorica', 'Gacic', 'Pasivan', 3.50));
//echo $insert;
//$insresult = $insert->execute();
//var_dump($insresult);

//require_once 'class.Select.inc';
//$select = new Select();
//$select->what(array('id', 'ime', 'prezime', 'koeficijent', 'status'));
//$select->from('temp');
//// $select->condition("id > ? AND id <= ?", array(0, 2));
//
//echo "<!DOCTYPE html>\n";
//echo "<html>\n";
//echo "   <head>\n";
//echo "      <title>Обрачун зарада</title>\n";
//echo "      <meta charset=\"UTF-8\">\n";
//echo "      <link rel=\"stylesheet\" href=\"../common/style.css\">\n";
//echo "   </head>\n";
//echo "   <body>\n";
//echo $select . "<br/>\n";
//$result = $select->execute();
//while($row = $result->fetch_assoc()) {
//   echo $row['id'] . " " . gettype($row['id']) . "---\n";
//   echo $row['ime'] . " " . gettype($row['ime']) . "---";
//   echo $row['prezime'] . " " . gettype($row['prezime']) . "---\n";
//   echo $row['status'] . " " . gettype($row['status']) . "---\n";
//   echo $row['koeficijent'] . " " . gettype($row['koeficijent']) . "<br />\n";
//}
//echo 'Total results: ' . $result->num_rows;
//// var_dump($select);
//$result->free();
//echo "   </body>\n";
//echo "   </html>\n";
////////////////////////////////////////////////

////////////////////////////////////////////////
// Private array in class
////////////////////////////////////////////////
//class A {
//   private $a;
//   
//   public function __construct() {
//      $this->a = array();
//      $this->a[] = "Poruka u konstruktoru";
//      $this->a[] = "Jos jedna poruka u konstruktoru";
//   }
//   
//   public function printArray() {
//      foreach ($this->a as $value) {
//         echo $value . "<br/>\n";
//      }
//   }
//   
//   public function getArray() {
//      return $this->a;
//   }
//
//   public function addMessage($message) {
//      $this->a[] = $message;
//   }
//}
//
//$a = new A;
//echo "Stampam a<br/>\n";
//$a->printArray();
//
//$b = $a->getArray();
//$b[] = "Dodata jos jedna poruka van konstruktora";
//echo "Stampam b<br/>\n";
//foreach ($b as $value) {
//   echo $value . "<br/>\n";
//}
//echo "Stampam a<br/>\n";
//$a->printArray();
//$a->addMessage("Dodajem jos jednu poruku u a");
//echo "Ponovo stampam b<br/>\n";
//foreach ($b as $value) {
//   echo $value . "<br/>\n";
//}
//echo "Stampam a<br/>\n";
//$a->printArray();
//
//$b = $a->getArray();
//echo "Stampam b posle jos jednog getArray<br/>\n";
//foreach ($b as $value) {
//   echo $value . "<br/>\n";
//}
////////////////////////////////////////////////

////////////////////////////////////////////////
// Validation testing
////////////////////////////////////////////////
//if ($_SERVER['REQUEST_METHOD'] == 'POST') {
////   $a = filter_input(INPUT_POST, 'test', FILTER_VALIDATE_INT, array('options' => array(
////       'default' => 0, 'min_range' => 10, 'max_range' => 100)));
//   $a = filter_input(INPUT_POST, 'test', FILTER_VALIDATE_FLOAT, array('options' => array(
//       'default' => 0.0, 'decimal' => '.')));
//
//
////   $a = filter_input(INPUT_POST, 'test', FILTER_SANITIZE_NUMBER_FLOAT, FILTER_FLAG_ALLOW_FRACTION);
////   echo "Posle sanitize: {$a} sa tipom " . gettype($a) . "<br/>\n";
////   if(!$a) { $a = 0.0; }
////   if(gettype($a) !== 'double') {
////      echo "postavljam tip na double<br/>\n";
////      settype($a, 'double');
////   }
//   echo "Uneto je: {$a} sa tipom " . gettype($a) . "<br/>\n";
//   $b = $a + 1.5;
//   echo "B = A + 1.5: {$b} sa tipom " . gettype($b) . "<br/>\n";
//}
//
//echo "<form action=\"test.php\" method=\"POST\">\n";
//echo "<input type=\"text\" name=\"test\" id=\"test\" value=\"\" />\n";
//echo "<input type=\"submit\" value=\"Snimi\">\n";
//echo "</form>\n";
////////////////////////////////////////////////

////////////////////////////////////////////////
// Singleton pattern
////////////////////////////////////////////////
//class A {
//   private $proprety;
//   private static $_instance;
//   
//   public static function getInstance() {
//      if (!self::$_instance) {
//         self::$_instance = new self();
//      }
//      return self::$_instance;
//   }
//
//   private function __construct() {
//      echo "inside constructor<br/>\n";
//      $this->proprety = 18;
//   }
//   
//   public function setProperty($value) {
//      $this->proprety = $value;
//   }
//   
//   public function getProperty() {
//      return $this->proprety;
//   }
//}
//
//A::getInstance()->setProperty(13);
//echo A::getInstance()->getProperty() . "<br/>\n";
//A::getInstance()->setProperty("ivkea pea");
//echo A::getInstance()->getProperty() . "<br/>\n";
////////////////////////////////////////////////