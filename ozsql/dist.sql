-- MySQL dump 10.13  Distrib 5.6.17, for Win32 (x86)
--
-- Host: localhost    Database: OZ
-- ------------------------------------------------------
-- Server version	5.5.19

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `district`
--

DROP TABLE IF EXISTS `district`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `district` (
  `id` smallint(5) unsigned NOT NULL,
  `naziv` varchar(70) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `district`
--

LOCK TABLES `district` WRITE;
/*!40000 ALTER TABLE `district` DISABLE KEYS */;
INSERT INTO `district` VALUES (0,'ðôÐÇð░ð┤ ðæðÁð¥ð│ÐÇð░ð┤'),(1,'ðíðÁð▓ðÁÐÇð¢ð¥ð▒ð░Ðçð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│ '),(2,'ðíÐÇðÁð┤ÐÜð¥ð▒ð░ð¢ð░ÐéÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(3,'ðíðÁð▓ðÁÐÇð¢ð¥ð▒ð░ð¢ð░ÐéÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│ '),(4,'ðêÐâðÂð¢ð¥ð▒ð░ð¢ð░ÐéÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(5,'ðùð░ð┐ð░ð┤ð¢ð¥ð▒ð░Ðçð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(6,'ðêÐâðÂð¢ð¥ð▒ð░Ðçð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(7,'ðíÐÇðÁð╝Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(8,'ð£ð░Ðçð▓ð░ð¢Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(9,'ðÜð¥ð╗Ðâð▒ð░ÐÇÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(10,'ðƒð¥ð┤Ðâð¢ð░ð▓Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(11,'ðæÐÇð░ð¢ð©ÐçðÁð▓Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(12,'ð¿Ðâð╝ð░ð┤ð©ÐÿÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(13,'ðƒð¥ð╝ð¥ÐÇð░ð▓Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(14,'ðæð¥ÐÇÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(15,'ðùð░ÐÿðÁÐçð░ÐÇÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(16,'ðùð╗ð░Ðéð©ð▒ð¥ÐÇÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(17,'ð£ð¥ÐÇð░ð▓ð©Ðçð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(18,'ðáð░Ðêð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(19,'ðáð░Ðüð©ð¢Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(20,'ðØð©Ðêð░ð▓Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(21,'ðóð¥ð┐ð╗ð©Ðçð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(22,'ðƒð©ÐÇð¥ÐéÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(23,'ðêð░ð▒ð╗ð░ð¢ð©Ðçð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(24,'ðƒÐçð©ÐÜÐüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(25,'ðÜð¥Ðüð¥ð▓Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(26,'ðƒðÁÐøð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(27,'ðƒÐÇð©ðÀÐÇðÁð¢Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(28,'ðÜð¥Ðüð¥ð▓Ðüð║ð¥-ð╝ð©ÐéÐÇð¥ð▓ð░Ðçð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│'),(29,'ðÜð¥Ðüð¥ð▓Ðüð║ð¥-ð┐ð¥ð╝ð¥ÐÇð░ð▓Ðüð║ð© Ðâð┐ÐÇð░ð▓ð¢ð© ð¥ð║ÐÇÐâð│');
/*!40000 ALTER TABLE `district` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-10-24 12:38:28
