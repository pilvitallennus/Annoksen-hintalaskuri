#include "foodfunctionality.hh"
#include "product.hh"


#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <future>
#include <QDebug>
#include <QString>




FoodFunctionality::FoodFunctionality()
{

}

// Tietojenkäsittely - Tiedostojen muokkaus

bool FoodFunctionality::gatherFromWeb(const std::string& searchFor)
{
    // Luo komennon, -u unbuffered - !!korjaus ja testaus pycharmissa ei täällä!!
	// Kehitysvaiheessa tässä on absoluuttinen polku
    std::string command = "python -u gatherDataPython.py";

    // Käynnistää Python-skriptin komentoriviparametrilla
    command += " \"" + searchFor + "\"";

    FILE* pipe = _popen(command.c_str(), "r");

    if (!pipe)
    {
        qDebug() << "gatherFromWeb virhe";
        return false;
    }

    _pclose(pipe);
    return true;
}


bool FoodFunctionality::addProductToDatabase(const std::shared_ptr<Product>& product)
{
    // Avaa "tiedosto-tietokannan" lisäystilassa, vaihda lopussa build kansioon kirjoitettavaksi eli poista polku
    std::ofstream databaseFile("productDataBase.csv", std::ios::app);

    if (!databaseFile.is_open())
    {
        qDebug() << "Error addProductToDatabase() -funktiossa";
        return false;
    }

    databaseFile << product->getProductName() << ";"
               << product->getDescName() << ";"
               << product->getProductPrice() << ";"
               << product->getProductPricePerKg() << "\n";

    databaseFile.close();
    return true;
}


void FoodFunctionality::pullProductsFromDatabase()
{
    std::ifstream databaseFile("productDataBase.csv");

    if (!databaseFile.is_open())
    {
        qDebug() << "Error pullProductsFromDatabase() -funktiossa";
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

            std::shared_ptr<Product> newProduct = std::make_shared<Product>(name,
                                                                            descName,
                                                                            price,
                                                                            pricePerKg);
            allProducts_.insert({name, newProduct});
        }

        else
        {
            qDebug() << "Virheellinen rivi tiedostossa: " << QString::fromStdString(line);
        }
    }

    databaseFile.close();
}


bool FoodFunctionality::addRecipeToDatabase(const std::string& recipeName,
                                            const std::vector<std::pair<std::string,
                                            double>>& recipeProducts)
{
    // Avaa "tiedosto-tietokannan" lisäystilassa, vaihda lopussa build kansioon kirjoitettavaksi eli poista polku
    std::ofstream databaseFile("recipeDataBase.csv", std::ios::app);

    if (!databaseFile.is_open())
    {
        qDebug() << "Error addRecipeToDatabase() -funktiossa";
        return false;
    }

    // Ensimmäisenä reseptin nimi
    databaseFile << recipeName << ";";


    // Tuote1;Määrä1;Tuote2;Määrä2; jne...
    for (const auto& product : recipeProducts)
    {
        databaseFile << product.first << ";" << product.second << ";";
    }

    databaseFile << "\n";
    databaseFile.close();
    return true;
}


void FoodFunctionality::pullRecipesFromDatabase()
{
    std::ifstream databaseFile("recipeDataBase.csv");

    if (!databaseFile.is_open())
    {
        qDebug() << "Error pullRecipesFromDatabase() -funktiossa";
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

            // Luo reseptin, eli avaimen ja tyhjän vektorin tuotteille
            createRecipe(recipeName);

            // Käsittely tuotenimen ja määrän osalta
            for (size_t i = 1; i < params.size(); i += 2)
            {
                std::string productName = params[i];
                // Tarkistaa, että määrätieto on olemassa
                if (i + 1 < params.size() && !params[i + 1].empty())
                {
                    double amount = std::stod(params[i + 1]);

                    addProductToRecipe(recipeName, productName, amount);
                }
            }
        }
    }
    databaseFile.close();
}


bool FoodFunctionality::deleteRecipeData(const std::string& recipeName)
{
    if (allRecipes_.find(recipeName) != allRecipes_.end() )
    {
        qDebug() << "resepti " << QString::fromStdString(recipeName) << " on olemassa";
        // Reseptin poisto map:ista
        allRecipes_.erase(recipeName);

        // resepti tietokannan synkronointi map kanssa.
        // Kyseenalainen ratkaisu mutta omassa käytössä toimiva
        // Tyhjennetään reseptien tietokanta

        std::ofstream clearFile(
            "recipeDataBase.csv",
            std::ofstream::out | std::ofstream::trunc);
        clearFile.close();

        // Lisää map reseptit takaisin tiedostoon
        for (const auto& recipe : allRecipes_)
        {
            addRecipeToDatabase(recipe.first, recipe.second);
        }
        return true;

    }
    return false;
}

void FoodFunctionality::syncRecipeDataBase()
{
    if (!allRecipes_.empty())
    {
        std::ofstream clearFile(
            recipeDataBase.csv",
            std::ofstream::out | std::ofstream::trunc);
        clearFile.close();

        // Lisää map reseptit takaisin tiedostoon
        for (const auto& recipe : allRecipes_)
        {
            addRecipeToDatabase(recipe.first, recipe.second);
        }
        return;
    }
    else
    {
        qDebug() << "allRecipes tyhjä, jotain kamalaa tapahtunut";
        return;
    }
}



// Tuotteiden käsittely

double FoodFunctionality::getProductPrice(const std::string &productName) const
{
    auto productIter = allProducts_.find(productName);
    if (productIter != allProducts_.end() )
    {
        double productPrice =  productIter->second->getProductPrice();
        return productPrice;
    }
    else
    {
        return 0.0;
    }
}


std::string FoodFunctionality::getProductDescName(const std::string &productName) const
{
    auto productIter = allProducts_.find(productName);
    if (productIter != allProducts_.end() )
    {
        std::string descName = productIter->second->getDescName();
        return descName;
    }
    else
    {
        return "";
    }
}


std::vector<std::string> FoodFunctionality::getAllProducts() const
{
    std::vector<std::string> allProductNames;

    for (const auto& product : allProducts_)
    {
        std::string productName = product.first;
        allProductNames.push_back(productName);
    }
    return allProductNames;
}


void FoodFunctionality::addFoundProduct(const std::string& name,
                                        double& price,
                                        double& pricePerKg)
{
    if (foundProducts_.find(name) != foundProducts_.end() )
    {
        qDebug() << "Tuote on jo etsityissä tuotteissa";
        return;
    }
    else
    {
        std::shared_ptr<Product> newProduct = std::make_shared<Product>(name, "",
                                                                        price,
                                                                        pricePerKg);
        foundProducts_.insert({name, newProduct});
        return;
    }
}


bool FoodFunctionality::addProduct(const std::string& name, const std::string& descName)
{
    // Tuote on jo tallennettu
    if (allProducts_.find(name) != allProducts_.end())
    {
        qDebug() << "Tuotete on jo tallennetuissa tuotteissa";
        return false;
    }
    // Tuotetta ei ole haettujen tuotteiden joukossa
    else if (foundProducts_.find(name) == foundProducts_.end())
    {
        qDebug() << "Tuotetta ei ole haetuissa tuotteissa";
        return false;
    }

    else
    {
        auto foundProductIter = foundProducts_.find(name);

        // Hakee hinnan ja kilohinnan
        double price = foundProductIter->second->getProductPrice();
        double pricePerKg = foundProductIter->second->getProductPricePerKg();

        // Luodaan std::shared_ptr<Product> ja lisätään se allProducts_ map:iin
        std::shared_ptr<Product> newProduct = std::make_shared<Product>(name,
                                                                        descName,
                                                                        price,
                                                                        pricePerKg);
        allProducts_.insert({name, newProduct});

        if(addProductToDatabase(newProduct))
        {
            qDebug() << "Tuotteen " <<
                        QString::fromStdString(name) <<
                        "lisääminen onnistui. Tuotteita tallennettu " <<
                        allProducts_.size();

            // tyhjennetään väliaikainen löytyneiden tuotteiden map
            foundProducts_.empty();
            return true;
        }
        else
        {
            // ei pitäisi tapahtua ikinä
            qDebug() << " addProduct() - error";
            return false;
        }
    }
}



// Respeitn käsittely

std::vector<std::string> FoodFunctionality::getAllRecipes() const
{
    std::vector<std::string> allRecipeNames;
    for (const auto& recipe : allRecipes_)
    {
        std::string recipeName = recipe.first;
        allRecipeNames.push_back(recipeName);
    }
    return allRecipeNames;
}


std::vector<std::pair<std::string, double> > FoodFunctionality::getRecipeProductPairs(
    const std::string &recipeName) const
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

std::vector<std::string> FoodFunctionality::getRecipeProductNames(const std::string& recipeName) const
{
    auto recipeIter = allRecipes_.find(recipeName);
    std::vector<std::string> productNames;

    if (recipeIter != allRecipes_.end())
    {
        for (const auto& productPair : recipeIter->second)
        {
            productNames.push_back(productPair.first);
        }
        return productNames;
    }

    else
    {
        // Tyhjä vektori, jos reseptiä ei löydy
        return {};
    }
}


bool FoodFunctionality::recipeExists(const std::string& recipeName) const
{
    if (allRecipes_.find(recipeName) != allRecipes_.end())
    {
        qDebug() << "Resepti " << QString::fromStdString(recipeName) << " löytyy";
        return true;
    }
    else
    {
       return false;
    }
}


void FoodFunctionality::createRecipe(const std::string& recipeName)
{
    if(!recipeExists(recipeName))
    {
        // Luodaan allRecipes_ map:iin uusi avain reseptille ja tyhjä vektori
        allRecipes_[recipeName] = {};
        qDebug() << "Uusi resepti luotu: " << QString::fromStdString(recipeName);
    }
}


void FoodFunctionality::addProductToRecipe(const std::string& recipeName,
                                           const std::string& productName,
                                           const double& amount)
{
    auto foundRecipe = allRecipes_.find(recipeName);

    if (foundRecipe != allRecipes_.end())
    {
        foundRecipe->second.push_back(std::make_pair(productName, amount));
    }
    else
    {
        qDebug() << "addProcutToRecipe() else, ei pitäisi ikinä tapahtua";
    }
}

bool FoodFunctionality::updateRecipeProducts(const std::string &recipeName, std::vector<std::pair<std::string, double> > updatedRecipeProducts)
{
    auto recipeIter = allRecipes_.find(recipeName);

    if (recipeIter != allRecipes_.end())
    {
        recipeIter->second = updatedRecipeProducts;
        return true;
    }

    qDebug() << "updateRecipeProducts() else, ei pitäisi ikinä tapahtua";
    return false;
}


double FoodFunctionality::calculatePricePerServing(const std::string &recipeName,
                                                   const double &servings) const
{
    auto foundRecipe = allRecipes_.find(recipeName);

    double pricePerServing = 0.0;

    if (foundRecipe != allRecipes_.end())
    {
        // Muuttuja, johon lasketaan kokonaishinta
        double totalPrice = 0.0;

        // Reseptin tuotteet
        std::vector<std::pair<std::string, double>> recipeProducts =
                                            getRecipeProductPairs(recipeName);

        for (const auto& product : recipeProducts)
        {
            // Vektorista tuotteen määtäkerroin
            double amount = product.second;

            // Etsitään tuoteolio ja sen hinta allProducts map:ista
            auto recipeProduct = allProducts_.find(product.first);
            double productPrice =  recipeProduct->second->getProductPrice();

            // Lisätään yhteishintaan tuotteen hinta kertaa määräkerroin
            totalPrice += productPrice * amount;

        }
        // Tuotteet käyty käpi, lasketaan annoshinta
        pricePerServing = totalPrice/servings;
    }
    return pricePerServing;
}


// Muut funktiot
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






