import csv
import time
import sys
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.common.exceptions import NoSuchElementException




def clean_text(text):
    # Poista "euro" ja "euroa/kg" tekstimuotoisista hinnoista
    cleaned_text = text.replace("€/kg", "").replace("€", "").strip()
    return cleaned_text


def main(searchFor):



    driver = webdriver.Chrome()
    url = f"https://www.s-kaupat.fi/hakutulokset?queryString={searchFor}"
    driver.get(url)


    time.sleep(2)
    #ehkä tarpeellinen
    #driver.execute_script("window.scrollBy(0, 500);")
    time.sleep(2)

    # Etsii 8 tai alle tuotetta
    products = driver.find_elements(By.XPATH, '(//*[contains(@class, "sc-76f76d49-0")])[position() <= 8]')

    time.sleep(2)


    # Avaa tiedosto kirjoittamista varten, poistettu julkista repoa varten, joten
    # polku joten ei välttämättä toimi, korjataan kun on sen aika
    with open('searchResults.csv', 'w', newline='', encoding='utf-8') as csvfile:
        # Luo CSV-tiedoston kirjoittaja
        writer = csv.writer(csvfile, delimiter=';')


        for product in products:
            try:
                productNameElement = product.find_element(By.XPATH, './/*[contains(@class, "sc-4dcde147-0")]')
                #productNameElement = product.find_element(By.XPATH, '//span[@class="sc-4dcde147-0"]')
                productName = productNameElement.text

                # jos haku ei onnistu skipataan tuote
                if not productName:
                            productName = "virhe"
                            continue;


                # Hae tuotteen hinnan ja kilohinnan
                productPriceElement = product.find_element(By.XPATH, './/*[contains(@class, "sc-68088102-0 ")]')
                productPrice = clean_text(productPriceElement.text)



                productPricePerKiloElement = product.find_element(By.XPATH, './/*[contains(@class, "sc-67cf5218-0")]')
                productPricePerKilo = clean_text(productPricePerKiloElement.text)



                # Kirjoita tiedot CSV-tiedostoon
                writer.writerow([productName, productPrice, productPricePerKilo])

            except NoSuchElementException as e1:
                print(f"Virhe: {e1}")

    # Sulje selain
    driver.quit()

if __name__ == "__main__":

    searchFor = sys.argv[1] if len(sys.argv) > 1 else ''

    main(searchFor)
