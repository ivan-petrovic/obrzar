# Napravi tabelu zaposlenih za obracun zarade

DROP TABLE IF EXISTS employe;

CREATE TABLE employe (
   id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
   firmaID TINYINT UNSIGNED NOT NULL,
   interniID MEDIUMINT UNSIGNED NOT NULL DEFAULT 0,
   tipZaposlenja TINYINT UNSIGNED NOT NULL DEFAULT 1,
   ime     VARCHAR(60) NOT NULL,
   prezime VARCHAR(60) NOT NULL,
   adresa VARCHAR(100) NOT NULL DEFAULT '',
   mesto VARCHAR(60) NOT NULL DEFAULT '',
   telefon VARCHAR(40) NOT NULL DEFAULT '',
   status ENUM ('Aktivan', 'Pasivan', 'Obrisan') NOT NULL DEFAULT 'Aktivan',
   koeficijent DECIMAL(4,2) NOT NULL DEFAULT 0,
   donetMinuliRad CHAR(10) NOT NULL DEFAULT '0-0-0',
   datumZaposlenja DATE NOT NULL,
   jmbg CHAR(13) NOT NULL DEFAULT '',
   datumRodjenja DATE,
   pol ENUM ('m', 'f') NOT NULL DEFAULT 'm',
   strucnaSpremaID TINYINT NOT NULL DEFAULT 0,
   opstinaID MEDIUMINT UNSIGNED NOT NULL DEFAULT 0,
   bankaID TINYINT NOT NULL DEFAULT 0,
   racun VARCHAR(60) NOT NULL DEFAULT '',
   radnoMesto TINYTEXT,
   opis TEXT,
   PRIMARY KEY (id)
);
