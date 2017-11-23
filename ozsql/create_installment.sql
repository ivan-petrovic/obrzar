# Napravi tabelu rata za obustave.
# Za svaku obustavu, u tabeli rate ce biti rate koje cine tu obustavu.

DROP TABLE IF EXISTS installment;

CREATE TABLE installment (
   id INT UNSIGNED NOT NULL AUTO_INCREMENT,
   obustavaID MEDIUMINT UNSIGNED NOT NULL,
   iznos DECIMAL(8,2) NOT NULL DEFAULT 0,
   mesec TINYINT NOT NULL,
   godina SMALLINT NOT NULL,
   tipObracuna ENUM ('Akontacija', 'Konacni') NOT NULL,
   PRIMARY KEY (id)
);
