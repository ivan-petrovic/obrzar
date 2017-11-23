<?php

/**
 * MySQLi database; only one connection is allowed. 
 */
class DB {

   private $_connection;
   // Store the single instance.
   private static $_instance;

   /**
    * Get an instance of the Database.
    * @return Database 
    */
   public static function getInstance() {
      if (!self::$_instance) {
         self::$_instance = new self();
      }
      return self::$_instance;
   }

   /**
    * Constructor.
    */
   private function __construct() {
        $url = parse_url(getenv("CLEARDB_DATABASE_URL"));

        $server = $url["host"];
        $username = $url["user"];
        $password = $url["pass"];
        $db = substr($url["path"], 1);

        $this->_connection = new mysqli($server, $username, $password, $db);
       
      // $this->_connection = new mysqli('localhost', 'root', 'ivan', 'oz');
      // Error handling.
      if (mysqli_connect_error()) {
         trigger_error('Failed to connect to MySQL: ' . mysqli_connect_error(), E_USER_ERROR);
      }
   }

   /**
    * Empty clone magic method to prevent duplication. 
    */
   private function __clone() {
   }

   /**
    * Get the mysqli connection. 
    */
   public function getConnection() {
      return $this->_connection;
   }
}
