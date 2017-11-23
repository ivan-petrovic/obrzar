# Napravi tabelu firmi za koje se vrsi obracun zarade

DROP TABLE IF EXISTS firm;

CREATE TABLE firm (
   id TINYINT UNSIGNED NOT NULL AUTO_INCREMENT,
   naziv VARCHAR(60) NOT NULL,
   adresa VARCHAR(100) NOT NULL DEFAULT '',
   mesto VARCHAR(60) NOT NULL DEFAULT '',
   telefon VARCHAR(40) NOT NULL DEFAULT '',
   tip TINYINT UNSIGNED NOT NULL DEFAULT 1,
   status ENUM ('Aktivan', 'Pasivan', 'Obrisan') NOT NULL DEFAULT 'Aktivan',
   matbr CHAR(8) NOT NULL DEFAULT '',
   pib CHAR(9) NOT NULL DEFAULT '',
   sifdel CHAR(4) NOT NULL DEFAULT '',
   ziroracun VARCHAR(80) NOT NULL DEFAULT '',
   nazivBanke VARCHAR(80) NOT NULL DEFAULT '',
   PRIMARY KEY (id)
);
