import csv
import time
import sys
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.common.exceptions import NoSuchElementException




def cleanText(text):
    # Poistaa "euro" ja "euroa/kg" tekstimuotoisista hinnoista
    cleanedText = text.replace("€/kg", "").replace("€", "").strip()
    return cleanedText


def main(searchFor):

    driver = webdriver.Firefox()
    url = f"**SIVUN OSOITE**?queryString={searchFor}"
    driver.get(url)

    time.sleep(2)

    #ehkä tarpeellinen
    #driver.execute_script("window.scrollBy(0, 500);")
    # time.sleep(2)

    # Etsii 8 tai alle tuotetta
    products = driver.find_elements(By.XPATH, '(//*[contains(@class, "sc-76f76d49-0")])[position() <= 8]')

    # time.sleep(2)

    # Avaa tiedosto kirjoittamista varten
    with open('searchResults.csv', 'w', newline='', encoding='utf-8') as csvfile:
        # Luo CSV-tiedoston kirjoittajan
        writer = csv.writer(csvfile, delimiter=';')


        for product in products:
            try:
                productNameElement = product.find_element(By.XPATH, './/*[contains(@class, "sc-4dcde147-0")]')
                productName = productNameElement.get_attribute("innerHTML")

                # jos haku ei onnistu skipataan tuote
                if not productName:
                    continue;


                # Hae tuotteen hinnan ja kilohinnan
                productPriceElement = product.find_element(By.XPATH, './/*[contains(@class, "sc-68088102-0 ")]')
                # Vaihdettu .text -> get_attribute
                productPrice = cleanText(productPriceElement.get_attribute("innerHTML"))
                # Punnittavat tuotteet aiheuttivat ongelmia
                productPrice = productPrice.replace("~", "")

                if not productPrice:
                    continue;

                productPricePerKiloElement = product.find_element(By.XPATH, './/*[contains(@class, "sc-67cf5218-0")]')
                productPricePerKilo = cleanText(productPricePerKiloElement.get_attribute("innerHTML"))

                # Tallennus riville
                writer.writerow([productName, productPrice, productPricePerKilo])

            except:
                return

    driver.quit()

if __name__ == "__main__":

    #Älä poista missään tilanteessa, ei toimi ilman :DDDD
    searchFor = sys.argv[1] if len(sys.argv) > 1 else ''
    main(searchFor)
