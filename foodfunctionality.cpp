#include "foodfunctionality.hh"
#include "product.hh"


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <future> // async

#include <QDebug>
#include <QString>




FoodFunctionality::FoodFunctionality()
{

}

// Käynnistää Python-hakuskriptin komentorivikomennolla
// Ei välttämättä paras mahdollinen ratkaisu mutta omaan käyttöön
// kehitetyssä ohjelmassa mielestäni riittävä
void FoodFunctionality::gatherFromWeb(const std::string& searchFor)
{
    // Luo komennon, -u unbuffered - toimii älä muuta
    // Julkisesta reposta poistettu gatherDataPython.py tarkka polku tietokoneella, ei välttämättä toimi
    // Kehitysvaiheessa tiedosto sijaitsi other files kansiossa, jotta sen muokkaaminen oli helppoa
    // Lopulliseen versioon tulee varmaan build-kansioon?
    std::string command = "python -u gatherDataPython.py";

    // Käynnistää Python-skriptin komentoriviparametrilla
    command += " \"" + searchFor + "\"";  // Lisätään hakusana komennon perään lainausmerkeissä

    FILE* pipe = _popen(command.c_str(), "r");

    if (!pipe)
    {
        return;
    }

    // Lähettää tiedot Python-skriptille
    std::string input_data = "";
    fprintf(pipe, "%s", input_data.c_str());
    fflush(pipe);


    _pclose(pipe);
    qDebug() << "Python script executed.";
}


// Hakee kaikki tallennetut tuotteet allProducts_ rakenteesta ja palauttaa niiden nimet vektorina
std::vector<std::string> FoodFunctionality::getAllProducts()
{
    std::vector<std::string> allProductNames;

    for (const auto& product : allProducts_)
    {
        std::string productName = product.first;

        allProductNames.push_back(productName);
    }

    return allProductNames;
}

// Hakee kaikki reseptin tuotteet ja palauttaa ne vektorina
std::vector<std::pair<std::string, int> > FoodFunctionality::getRecipeProducts(const std::string &recipeName)
{
    auto recipeIter = allRecipes_.find(recipeName);

    if (recipeIter != allRecipes_.end())
    {
        // Palautetaan reseptin tuotteet ja määrät
        return recipeIter->second;
    }
    else
    {
        // Tyhjä vektori, jos reseptiä ei löydy
        return {};
    }
}


// Tallentaa tuotteen productDataBase.csv tiedostoon
bool FoodFunctionality::addProductToDatabase(const std::shared_ptr<Product>& product)
{
    // Avaa "tiedosto-tietokannan" lisäystilassa
    // Poistettu .csv polku
    std::ofstream databaseFile("productDataBase.csv", std::ios::app);

    if (!databaseFile.is_open())
    {
        qDebug() << "Error opening databaseFile addProductToDatabase() -funktiossa";
        return false;
    }

    // riville kirjoitus
    databaseFile << product->getProductName() << ";"
               << product->getDescName() << ";"
               << product->getProductPrice() << ";"
               << product->getProductPricePerKg() << "\n";


    databaseFile.close();

    qDebug() << QString::fromStdString(product->getProductName()) << " lisätty tietokantaan";
    return true;
}


// Ohjelman käynnistyessä hakee productDataBase.csv tiedoston tuotteet ja tallentaa ne
// map rakenteeseen allProducts_
void FoodFunctionality::pullProductsFromDatabase()
{
    // Poistettu .csv polku
    std::ifstream databaseFile("productDataBase.csv");

    if (!databaseFile.is_open())
    {
        qDebug() << "Error opening databaseFile pullProductsFromDatabase() -funktiossa";
        return;
    }

    std::string line;


    while (std::getline(databaseFile, line))
    {
        std::vector<std::string> params = split(line, ';');


        if (params.size() == 4)
        {

            std::string name = params[0];
            std::string descName = params[1];
            double price = std::stod(params[2]);
            double pricePerKg = std::stod(params[3]);

            std::shared_ptr<Product> newProduct = std::make_shared<Product>(name, descName, price, pricePerKg);

            allProducts_.insert({name, newProduct});
        }
        else
        {
            qDebug() << "Virheellinen rivi tiedostossa: " << QString::fromStdString(line);
        }
    }

    // Sulje tiedosto
    databaseFile.close();
    // printAllProducts();
}




bool FoodFunctionality::addRecipeToDatabase(const std::string& recipeName, const std::vector<std::pair<std::string, int>>& recipeProducts)
{
    // Avaa "tiedosto-tietokannan" lisäystilassa, vaihda lopussa build kansioon kirjoitettavaksi eli poista polku
    std::ofstream databaseFile("recipeDataBase.csv", std::ios::app);

    if (!databaseFile.is_open())
    {
        qDebug() << "Error opening databaseFile addRecipeToDatabase() -funktiossa";
        return false;
    }

    // Rivin ensimmäinen reseptin nimi
    databaseFile << recipeName << ";";


    // Kirjoita jokainen tuote ja sen määrä peräkkäin
    for (const auto& product : recipeProducts)
    {
        databaseFile << product.first << ";" << product.second << ";";
    }
    // Rivinvaihto
    databaseFile << "\n";
    databaseFile.close();


    return true;
}


void FoodFunctionality::pullRecipesFromDatabase()
{
    std::ifstream databaseFile("recipeDataBase.csv");

    if (!databaseFile.is_open())
    {
        qDebug() << "Error opening databaseFile pullRecipesFromDatabase() -funktiossa";
        return;
    }

    std::string line;


    while (std::getline(databaseFile, line))
    {
        std::vector<std::string> params = split(line, ';');


        // Tarkistaa, että rivillä on parillinen määrä tietoja (nimi   +tuote1;määrä1,...... ;tyhjä)
        if (params.size() % 2 == 0)
        {
            std::string recipeName = params[0];
            createRecipe(recipeName);

            // Käsittely tuotenimen ja määräparien osalta
            for (size_t i = 1; i < params.size(); i += 2)
            {
                std::string productName = params[i];

                // Tarkistaa, että määrätieto on olemassa ennen sen käyttämistä
                if (i + 1 < params.size() && !params[i + 1].empty())
                {
                    int amount = std::stoi(params[i + 1]);

                    // Lisää tuote reseptiin
                    addProductToRecipe(recipeName, productName, amount);
                }
                else
                {
                    // ei mitään varmistaa ettei kaadu
                }
            }
        }
    }
    databaseFile.close();
}


// Käytetään tallentamaan hakutuloksen tuotteet
void FoodFunctionality::addFoundProduct(const std::string& name, double& price, double& pricePerKg)
{
    if (foundProducts_.find(name) != foundProducts_.end() )
    {
        qDebug() << "tuote on jo etsityissä tuotteissa";
        return;
    }
    else
    {
        // Luodaan std::shared_ptr<Product> ja lisätään se allProducts_-map:iin
        std::shared_ptr<Product> newProduct = std::make_shared<Product>(name, "", price, pricePerKg);
        foundProducts_.insert({name, newProduct});
        return;
    }
}


// Käytetään tallentamaan lisättävä tuote (hakutuloksista)
bool FoodFunctionality::addProduct(const std::string& name, const std::string& descName)
{
    // ei olemassaoleva tuote ja tulee olla löydettyjen tuotteiden mapissa
    if (allProducts_.find(name) != allProducts_.end())
    {
        qDebug() << "Tuotete on jo tallennetuissa tuotteissa";
        return false;
    }
    else if (foundProducts_.find(name) == foundProducts_.end())
    {
        qDebug() << "Tuotetta ei ole haetuissa tuotteissa";
        return false;
    }

    else
    {
        auto foundProductIter = foundProducts_.find(name);

        // Hae hinta ja kilohinta
        double price = foundProductIter->second->getProductPrice();
        double pricePerKg = foundProductIter->second->getProductPricePerKg();

        // Luodaan std::shared_ptr<Product> ja lisätään se allProducts_-map:iin
        std::shared_ptr<Product> newProduct = std::make_shared<Product>(name, descName, price, pricePerKg);
        allProducts_.insert({name, newProduct});

        if(addProductToDatabase(newProduct))
        {
            qDebug() <<"Tuotteen " << QString::fromStdString(name) <<
                "lisääminen onnistui. Tuotteita tallennettu " << allProducts_.size();
            // tyhjennetään väliaikainen löytyneiden tuotteiden map
            foundProducts_.empty();
            return true;
        }
        else
        {
            // ei pitäisi tapahtua ikinä
            qDebug() << "HÄLYTYS HÄLYTYS KAMALA ERRORI addProduct() -funktiossa";
            return false;
        }



    }

}


// Tarkistaa onko resepti olemassa (allRecipes_ mapissa)
bool FoodFunctionality::recipeExists(const std::string recipeName)
{
    if (allRecipes_.find(recipeName) != allRecipes_.end())
    {
        qDebug() << "Resepti " << QString::fromStdString(recipeName) << " löytyy";
        return true;
    }
    return false;
}


// Luo uuden reseptin allRecipes_ mappiin
void FoodFunctionality::createRecipe(const std::string recipeName)
{
    if(!recipeExists(recipeName))
    {
        // Luodaan (resepti) -map rakenteeseen uusi avain reseptille ja tyhjä vektori
        allRecipes_[recipeName] = {};
        qDebug() << "Uusi resepti luotu: " << QString::fromStdString(recipeName);
    }
}


// Lisää tuotteen olemassaolevaan reseptiin
void FoodFunctionality::addProductToRecipe(const std::string recipeName, std::string productName, int amount)
{
    auto foundRecipe = allRecipes_.find(recipeName);

    if (foundRecipe != allRecipes_.end())
    {
        foundRecipe->second.push_back(std::make_pair(productName, amount));
        qDebug() << "Lisätty tuote " << QString::fromStdString(productName) << " reseptiin " << QString::fromStdString(recipeName);

    }
    else
    {
        qDebug() << "KAMALA VIRHE addProcutToRecipe() else trigger, ei pitäisi ikinä tapahtua";
    }
}


// Jakaa rivin delim mukaan osiin ja palauttaa vektorina
std::vector<std::string> FoodFunctionality::split(const std::string& str, char delim)
{
    std::vector<std::string> result = {""};
    bool insideQuotation = false;
    for ( char currentChar : str )
    {
        if ( currentChar == '"' )
        {
            insideQuotation = not insideQuotation;
        }
        else if ( currentChar == delim and not insideQuotation)
        {
            result.push_back("");
        }
        else
        {
            result.back().push_back(currentChar);
        }
    }
    return result;
}



// TESTIFUNKTIOITA
void FoodFunctionality::tulostaFoundProductsKoko()
{
    qDebug() << "comboboxin koko tulisi olla: " <<  foundProducts_.size();
}


void FoodFunctionality::tulostaRecipes()
{
    qDebug();
    for (const auto& recipe : allRecipes_)
    {
        qDebug() << "Resepti: " << QString::fromStdString(recipe.first);
        qDebug() << "Tuotteet: ";
        for (const auto& product : recipe.second)
        {
            qDebug() << "  - " << QString::fromStdString(product.first) << ": " << product.second;
        }
        qDebug() << "-------------------------";
    }
}


void FoodFunctionality::printAllProducts() const
{
    qDebug() << "Kaikki tuotteet:\n";

    for (const auto& productPair : allProducts_)
    {
        const std::string& name = productPair.first;
        const std::shared_ptr<Product>& product = productPair.second;

        qDebug() << "Nimi: " << QString::fromStdString(name) <<
                    " Kuvaava nimi: " << QString::fromStdString(product->getDescName()) <<
                    " Hinta: " << QString::number(product->getProductPrice());
    }
}



