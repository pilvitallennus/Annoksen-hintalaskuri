# Annoksen-hintalaskuri

## Saatavilla suomeksi ja englanniksi (Available in Finnish and English)
Scroll down for English version


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








<br><br><br>
<br><br><br>
<br><br><br>








# Annoksen-hintalaskuri in English

## Status
Version 1.1 - The second version of the program, with the addition of the possibility to edit and delete recipes.

I develop the project in a private repo and update any significant changes here.


## General information
Version 1.0

The program uses a python script and Selenium to retrieve the product data, stores them in the manner illustrated in image 3 into csv-databases, as well as in map structures for program execution. 
The program displays all stored products in the user interface and recipes can be created from the products. 
The portion price of a recipe can be calculated by selecting the recipe and setting the number of servings in the value selector next to the recipe name.


Version 1.1

Updated the layout of the user interface to match image 1.
Added a database editing mode that allows you to edit or delete recipes. Each separate step of database editing requires the "Database editing mode" checkbox to be checked to avoid mistakes.
The database edit mode, iamge 2, is opened by selecting a recipe, selecting the database edit mode and pressing the "Edit recipe" button.

## Data processing during the execution of the programme

When the program is started, it retrieves the information about the stored products and recipes from the csv-files. The data is stored in map structures, which are responsible for all the functionality of the program. The program therefore uses the map structures for the functionality and the csv-files only to transfer data between runs. When editing recipes, the program updates the recipe map structures and then synchronizes the csv-database accordingly.

## Attachments

Image [1] Program UI:
![annoshintaKäyttöliittymä1](https://github.com/pilvitallennus/annoksen-hintalaskuri/assets/87340153/4821fec9-470c-4e09-8c40-caa42466bab9)

Image [2] Program UI during the "Database editing mode":
![annoshintaKäyttöliittymä2](https://github.com/pilvitallennus/annoksen-hintalaskuri/assets/87340153/ee1a7998-6dfb-4a11-817e-fcc72df9775c)

Image [3] Data flow for data retrieval and storage
![image](https://github.com/pilvitallennus/annoksen-hintalaskuri/assets/87340153/198e4e28-9e81-4028-9afb-689be9ece61a)


## Next up
- Improving performance
- Price watch and updating of existing prices
- Comparison of portion price with student lunch
- Random recipe draw
