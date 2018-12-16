-- MySQL dump 10.13  Distrib 5.5.61, for debian-linux-gnu (x86_64)
--
-- Host: cis-linux2    Database: shi
-- ------------------------------------------------------
-- Server version	5.5.61-0ubuntu0.14.04.1

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
-- Table structure for table `Visitors`
--

DROP TABLE IF EXISTS `Visitors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Visitors` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Email` varchar(45) NOT NULL,
  `Password` varchar(45) DEFAULT NULL,
  `FirstName` varchar(45) DEFAULT NULL,
  `LastName` varchar(45) DEFAULT NULL,
  `Status` int(11) DEFAULT '0',
  `Acode` varchar(45) DEFAULT NULL,
  `Rdatetime` datetime DEFAULT NULL,
  `Adatetime` datetime DEFAULT NULL,
  `Pdatetime` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID_UNIQUE` (`ID`),
  UNIQUE KEY `Email_UNIQUE` (`Email`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Visitors`
--

LOCK TABLES `Visitors` WRITE;
/*!40000 ALTER TABLE `Visitors` DISABLE KEYS */;
INSERT INTO `Visitors` VALUES (1,'shi@temple.edu','81dc9bdb52d04dc20036dbd8313ed055','Justin','Shi',0,'1903655373','2018-11-24 09:08:55','2018-11-24 09:11:05','2018-11-24 10:43:19');
/*!40000 ALTER TABLE `Visitors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `PLogs`
--

DROP TABLE IF EXISTS `PLogs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `PLogs` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Timestamp` varchar(45) DEFAULT NULL,
  `Host` varchar(45) DEFAULT NULL,
  `Size` int(11) DEFAULT NULL,
  `ElapsedTime` float DEFAULT NULL,
  `MFLOPS` float DEFAULT NULL,
  `LoopOrder` varchar(45) DEFAULT NULL,
  UNIQUE KEY `ID_UNIQUE` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PLogs`
--

LOCK TABLES `PLogs` WRITE;
/*!40000 ALTER TABLE `PLogs` DISABLE KEYS */;
INSERT INTO `PLogs` VALUES (1,'2018-11-24 20:00:24',NULL,100,4201.6,238.005,'ijk'),(2,'2018-11-24 20:00:24',NULL,100,5403,185.082,'ikj'),(3,'2018-11-24 20:00:24',NULL,100,4364.2,229.137,'kij'),(4,'2018-11-24 20:00:24',NULL,100,4485.8,222.926,'kji'),(5,'2018-11-24 20:00:24',NULL,100,4303,232.396,'jki'),(6,'2018-11-24 20:00:24',NULL,100,4483.2,223.055,'jik'),(7,'2018-11-24 20:00:24','Best Order',100,4201.6,238.005,'i-j-k'),(8,'2018-11-24 20:01:47',NULL,1000,0.21309,NULL,'BubbleSort'),(9,'2018-11-24 20:01:48',NULL,1000,0.103242,NULL,'BubbleSort2');
/*!40000 ALTER TABLE `PLogs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ScalabilityLog`
--

DROP TABLE IF EXISTS `ScalabilityLog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ScalabilityLog` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `P` int(11) DEFAULT NULL,
  `TimeStamp` datetime DEFAULT NULL,
  `LogID` int(11) DEFAULT NULL,
  `Size` float DEFAULT NULL,
  `LoopOrder` varchar(45) DEFAULT NULL,
  `ElapsedTime` float DEFAULT NULL,
  `MFLOPS` float DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=16 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ScalabilityLog`
--

LOCK TABLES `ScalabilityLog` WRITE;
/*!40000 ALTER TABLE `ScalabilityLog` DISABLE KEYS */;
INSERT INTO `ScalabilityLog` VALUES (1,2,'2018-11-24 20:01:05',NULL,100,'ikj',3938.5,100),(2,2,'2018-11-24 20:01:06',NULL,150,'ikj',4861,100),(3,2,'2018-11-24 20:01:06',NULL,200,'ikj',9854.5,100),(4,2,'2018-11-24 20:01:07',NULL,250,'ikj',12760.5,100),(5,2,'2018-11-24 20:01:07',NULL,300,'ikj',22988.5,100),(6,2,'2018-11-24 20:01:08',NULL,350,'ikj',41752.5,100),(7,2,'2018-11-24 20:01:09',NULL,400,'ikj',41811,100),(8,2,'2018-11-24 20:01:09',NULL,450,'ikj',66345.5,100),(9,2,'2018-11-24 20:01:10',NULL,500,'ikj',86872,100),(10,2,'2018-11-24 20:01:11',NULL,550,'ikj',117066,100);
/*!40000 ALTER TABLE `ScalabilityLog` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-11-24 22:46:56
