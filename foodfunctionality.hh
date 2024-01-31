#ifndef FOODFUNCTIONALITY_HH
#define FOODFUNCTIONALITY_HH

#include "product.hh"

#include <memory>
#include <vector>
#include <map>
#include <string>




using allProducts = std::map<std::string, std::shared_ptr<Product>>;
using foundProducts = std::map<std::string, std::shared_ptr<Product>>;
using allRecipes = std::map<std::string, std::vector<std::pair<std::string, double>>>;



class FoodFunctionality
{
public:
    FoodFunctionality();


    // Tietojenkäsittely - Tiedostojen muokkaus

    /**
     * Hakee tuotteita python-skriptin avulla verkkosivustolta
     * @param searchFor - Hakusana joka välitetään verkkosivulle
     * @return 'true', jos haku onnistui, muuten 'false'
     */
    bool gatherFromWeb(const std::string& searchFor);

    /**
     * Lisää tallennetun tuotteen tietokantaan, eli productDataBase.csv -tiedostoon
     * @param product - Osoitin lisättävän tuotteen tuoteolioon
     * @return 'true', jos lisäys onnistui, muuten 'false'
     */
    bool addProductToDatabase(const std::shared_ptr<Product>& product);

    /**
     * Hakee productDataBase.csv tiedostosta kaikki tallennetut tuotteet ja lisää ne
     * allProducts_ map:iin
     */
    void pullProductsFromDatabase();

    /**
     * Lisää tallennetun reseptin tietokantaan, eli recipeDataBase.csv -tiedostoon
     * @param recipeName - Lisättävän reseptin nimi
     * @param recipeProducts - Vektori jossa on reseptin tuotteiden nimet ja määrät
     * @return 'true', jos lisäys onnistui, muuten 'false'
     */
    bool addRecipeToDatabase(const std::string& recipeName,
                             const std::vector<std::pair<std::string,
                             double>>& recipeProducts);

    /**
     * Hakee recipeDataBase.csv tiedostosta kaikki tallennetut reseptit ja lisää ne
     * allRecipes_ map:iin
     */
    void pullRecipesFromDatabase();

    /**
     * Poistaa reseptin allRecipes_ map:ist ja päivittää recipeDataBase.csv vastaamaan
     * allRecipes_ map:ia.
     * @param recipeName - Poistettavan reseptin nimi
     * @return 'true', jos reseptin poisto onnistui, muuten 'false'
     */
    bool deleteRecipeData(const std::string& recipeName);

    /**
     * Synkronoi resepti tietokannan ja allRecipes_ map:in.
     */
    void syncRecipeDataBase();


    // Tuotteiden käsittely

    /**
     * Hakee tuotteelle hinnan ja palauttaa sen
     * @param productName - Haettavan tuotteen nimi
     * @return tuotteen hinta haettuna tuoteoliosta
     */
    double getProductPrice(const std::string& productName) const;

    /**
     * Hakee tuoteen kuvaavan nimen ja palauttaa sen
     * @param productName - Haettavan tuotteen nimi
     * @return tuotteen kuvaava nimi haettuna tuoteoliosta
     */
    std::string getProductDescName(const std::string& productName) const;

    /**
     * Hakee kaikki tallennetut tuotteet allProducts_ map:ista ja palauttaa
     * niiden nimet vektorina
     * @return Kaikkien tallennettujen tuotteiden nimet vektorina
     */
    std::vector<std::string> getAllProducts() const;

    /**
     * Luo hakutuloksena löytyneelle tuotteelle tuoteolion ja lisää sen foundProducts_
     * map:iin
     * @param name - Löydetyn tuotteen nimi
     * @param price - Löydetyn tuotteen hinta
     * @param pricePerKg - Löydetyn tuotteen kilohinta
     */
    void addFoundProduct(const std::string& name, double& price, double& pricePerKg);


    /**
     * Tallentaa löydetyn tuotteen tallennetuksi tuotteeksi, eli siirtää tuotteen
     * foundProducts_ -> allProducts_
     * @param name - Lisättävän tuotteen nimi
     * @param descName - Lisättävän tuotteen kuvaava nimi
     * @return 'true', jos tuotteen lisääminen onnistuu, muuten 'false'
     */
    bool addProduct(const std::string& name, const std::string& descName);



    // Reseptien käsittely

    /**
     * Hakee kaikki tallennetut reseptit allRecipes_ map:ista ja palauttaa
     * niiden nimet vektorina
     * @return Kaikkien tallennettujen reseptejen nimet vektorina
     */
    std::vector<std::string> getAllRecipes() const;

    /**
     * Etsii reseptin tuotteet ja määrät allRecipes_ map:ista ja palauttaa ne pair vektorina
     * @param recipeName - Etsittävän reseptin nimi
     * @return Vektori joka sisältää etsittävän reseptin tuotteet ja määrät
     */
    std::vector<std::pair<std::string, double>> getRecipeProductPairs(
        const std::string& recipeName) const;

    /**
     * Etsii reseptin tuotteet allRecipes_ map:ista ja palauttaa tuotteiden nimet vektorina
     * @param recipeName - Etsittävän reseptin nimi
     * @return Vektori joka sisältää etsittävän reseptin tuotteet
     */
    std::vector<std::string> getRecipeProductNames(const std::string& recipeName) const;

    /**
     * Tarkistaa onko resepti olemassa, eli onko reseptiä allRecipes_ map:issa
     * @param recipeName - Tarkistettavan reseptin nimi
     * @return 'true', jos resepti on olemassa, muuten 'false'
     */
    bool recipeExists(const std::string& recipeName) const;

    /**
     * Luo reseptin, jos sellaista ei vielä ole olemassa. Resepti luodaan
     * lisäämällä reseptin nimellä avain ja sille tyhjä vektori arvopariksi.
     * @param recipeName - Reseptin nimi
     */
    void createRecipe(const std::string& recipeName);

    /**
     * Lisää olemassaolevaan reseptiin tuotteen.
     * @param recipeName - Resepti, johon tuote lisätään
     * @param productName - Lisättävän tuotteen nimi
     * @param amount - Lisättävän tuotteen määrä
     *                  1 = koko tuote
     *                  0,5 = puolikas tuote jne...
     */
    void addProductToRecipe(const std::string& recipeName,
                            const std::string& productName,
                            const double& amount);

    /**
     * Päivittää reseptin vaihtamalla tuote,määrä vektorin uuteen
     * @param recipeName - Resepti, jota muokataan
     * @param updatedRecipeProducts - Päivitetty tuotevektori
     * @return palauttee 'true', jos onnistui, muuten 'false'
     */
    bool updateRecipeProducts(const std::string& recipeName,
                              std::vector<std::pair<std::string, double>> updatedRecipeProducts);

    /**
     * Laskee reseptin annoshinnan, eli aineet/kertoja syöty
     * @param recipeName - Resepti, jonka hintaa lasketaan
     * @param servings  - Syöntikertojen määrä
     * @return palauttaa annoshinnan
     */

    double calculatePricePerServing(const std::string& recipeName, const double& servings) const;


    // Muut funkiot

    /**
     * Jakaa merkkijonon osiin ja palauttaa sen vektorina.
     * @param str - Jaettava merkkijono
     * @param delim - Erotin jonka mukaan jaetaan
     * @return vektori jossa on alkiona eroteltu jaettava merkkijono
     */
    std::vector<std::string> split(const std::string& str, char delim = ';');




private:

    // Pitää  kirjaa tallennetuista tuotteista
    allProducts allProducts_;       // map<tuotteen nimi, osoitin tuoteolioon>

    // Pitää kirjaa hakutuloksista, tyhjennetään lisättäessä tuote
    foundProducts foundProducts_;   // map<tuotteen nimi, osoitin tuoteolioon>,

    // Pitää kirjaa kaikista luoduista resepteistä
    allRecipes allRecipes_;    // map<reseptin nimi, vector<pair<tuote-olio, määrä>>
};


#endif // FOODFUNCTIONALITY_HH
