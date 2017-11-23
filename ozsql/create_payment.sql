# Napravi tabelu obracuna
# Za svaki obracun, u tabeli karneti ce biti
# specifikacija karneta za dati obracun
# Dakle, za svakog zaposlenog (aktivnog u tom trenutku)
# jedan red u tabeli karnet sa ulaznim parametrima za obracun

DROP TABLE IF EXISTS payment;

CREATE TABLE payment (
   id SMALLINT UNSIGNED NOT NULL AUTO_INCREMENT,
   mesec TINYINT NOT NULL,
   godina SMALLINT NOT NULL, 
   tipObracuna ENUM ('Akontacija', 'Konacni', 'LZPred') NOT NULL,
   firmaID SMALLINT UNSIGNED NOT NULL DEFAULT 0,
   datumIsplate DATE,
   tipCenaRada TINYINT UNSIGNED NOT NULL DEFAULT 1,   
   cenaRada DECIMAL(8,2) NOT NULL DEFAULT 0,
   satiUMesecu SMALLINT UNSIGNED NOT NULL DEFAULT 0,
   koefRadPraznik DECIMAL(4,2) NOT NULL DEFAULT 0,
   koefRadNoc DECIMAL(4,2) NOT NULL DEFAULT 0,
   koefRadPrekovremeno DECIMAL(4,2) NOT NULL DEFAULT 0,
   koefRadSmena DECIMAL(4,2) NOT NULL DEFAULT 0,
   koefRadNedelja DECIMAL(4,2) NOT NULL DEFAULT 0,
   iznosTOPoDanu DECIMAL(8,2) NOT NULL DEFAULT 0,
   iznosRegres DECIMAL(8,2) NOT NULL DEFAULT 0,
   iznosPoklonDeci DECIMAL(8,2) NOT NULL DEFAULT 0,
   koefBolDo30D DECIMAL(4,2) NOT NULL DEFAULT 0,
   koefPrekid DECIMAL(4,2) NOT NULL DEFAULT 0,
   stopaPoreza DECIMAL(4,2) NOT NULL DEFAULT 0,
   iznosNeoporezivo DECIMAL(8,2) NOT NULL DEFAULT 0,
   stopaPIO DECIMAL(4,2) NOT NULL DEFAULT 0,
   stopaZdravstvo DECIMAL(4,2) NOT NULL DEFAULT 0,
   stopaNezaposlenost DECIMAL(4,2) NOT NULL DEFAULT 0,
   stopaPIOPoslodavac DECIMAL(4,2) NOT NULL DEFAULT 0,
   stopaZdravstvoPoslodavac DECIMAL(4,2) NOT NULL DEFAULT 0,
   stopaNezaposlenostPoslodavac DECIMAL(4,2) NOT NULL DEFAULT 0,
   minOsnovicaDoprinosa DECIMAL(8,2) NOT NULL DEFAULT 0,
   iznosBrutoZarada DECIMAL(8,2) NOT NULL DEFAULT 0,
   iznosMinZarada DECIMAL(8,2) NOT NULL DEFAULT 0,
   PRIMARY KEY (id)
);
