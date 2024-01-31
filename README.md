# Annoksen-hintalaskuri

## Tila
Versio 1.1 - Ohjelman toinen versio, johon on lisätty mahdollisuus muokata ja poistaa reseptejä.

Kehitän ohjelmaa yksityisessä repossa ja päivitän merkittävät muutokset tänne.


## Yleistä
Versio 1.0

Ohjelma hakee python skriptin ja seleniumin avulla tuotetiedot, tallentaa ne kuvan 3 havainnollistamalla tavalla (.csv) tietokantoihin, sekä ohjelman suorittamista varten map-rakenteisiin. 
Ohjelma näyttää kaikki tallennetut tuotteet käyttöliittymässä ja tuotteista voidaan luoda reseptejä. 
Reseptien annoshinta voidaan laskea valitsemalla resepti ja asettamalla syöntikertojen määrä arvovalitsimeen. 


Versio 1.1

Päivitetty käyttöliittymän asettelu kuvan 1 mukaiseksi.
Lisätty tietokannan muokkaustila, joka mahdollistaa reseptien muokkaamisen tai poistamisen. Tietokannan muokkaamisen jokainen erillinen vaihe vaatii "Tietokannan muokkaustila" -checkboxin valinna vahinkojen välttämiseksi.
Tietokannan muokkaustila, kuva 2, avautuu valitsemalla resepti, valitsemalla tietokannan muokkaustila päälle ja painamalla "Muokkaa reseptiä" -nappia.


## Tietojenkäsittely ohjelmaa suoritettaessa

Ohjelma hakee käynnistyessään tiedot tallennetuista tuotteista ja resepteistä csv-tiedosoista. Tiedot tallennetaan map-rakenteisiin, jotka vastaavat ohjelman kaikesta toiminnallisuudesta. Ohjelma siis käyttää map-rakenteita toiminnallisuuteen ja csv-tiedostot siirtävät ainoastaan tietoa käyttökertojen välillä. Reseptejen muokkauksessa ohjelma päivittää reseptien map-rakenten ja synkronoi tämän jälkeen csv tietokannan sen mukaan.


## Liitteet

Kuvaa [1] Ohjelman käyttöliittymä:
![annoshintaKäyttöliittymä1](https://github.com/pilvitallennus/annoksen-hintalaskuri/assets/87340153/4821fec9-470c-4e09-8c40-caa42466bab9)

Kuva [2] Ohjelman käyttöliittymä tiedokantaa muokatessa:
![annoshintaKäyttöliittymä2](https://github.com/pilvitallennus/annoksen-hintalaskuri/assets/87340153/ee1a7998-6dfb-4a11-817e-fcc72df9775c)

Kuva [3] Tiedonhaku ja tiedon tallentaminen ohjelmassa:
![image](https://github.com/pilvitallennus/annoksen-hintalaskuri/assets/87340153/198e4e28-9e81-4028-9afb-689be9ece61a)



## Seuraavaksi
- Ohjelman suorituskyvyn parantaminen
- Hintavahti ja olemassaolevien hintojen päivittäminen
- Annoshinnan vertaaminen opiskelijalounaaseen
- Satunnaisen reseptin arpominen
