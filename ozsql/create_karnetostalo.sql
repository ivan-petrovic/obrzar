# Napravi tabelu karneta ostalo

DROP TABLE IF EXISTS karnetostalo;

CREATE TABLE karnetostalo (
   zaposleniID SMALLINT UNSIGNED NOT NULL,
   obracunID SMALLINT UNSIGNED NOT NULL,
   iznosBruto30 DECIMAL(8,2) NOT NULL DEFAULT 0,
   iznosNeto30 DECIMAL(8,2) NOT NULL DEFAULT 0,
   iznosPIO30 DECIMAL(8,2) NOT NULL DEFAULT 0,
   PRIMARY KEY (zaposleniId, obracunId)
);
