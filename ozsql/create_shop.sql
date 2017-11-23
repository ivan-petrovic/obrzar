# Napravi tabelu radnji

DROP TABLE IF EXISTS shop;

CREATE TABLE shop (
   id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
   naziv VARCHAR(60) NOT NULL,
   adresa VARCHAR(100) NOT NULL DEFAULT '',
   mesto VARCHAR(60) NOT NULL DEFAULT '',
   telefon VARCHAR(40) NOT NULL DEFAULT '',
   status ENUM ('Aktivan', 'Pasivan', 'Obrisan') NOT NULL DEFAULT 'Aktivan',
   ziroracun VARCHAR(80) NOT NULL DEFAULT '',
   PRIMARY KEY (id)
);
