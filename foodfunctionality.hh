#ifndef FOODFUNCTIONALITY_HH
#define FOODFUNCTIONALITY_HH

#include "product.hh"

#include <memory>
#include <vector>
#include <map>
#include <string>




using allProducts = std::map<std::string, std::shared_ptr<Product>>;
using foundProducts = std::map<std::string, std::shared_ptr<Product>>;
using allRecipes = std::map<std::string, std::vector<std::pair<std::string, int>>>;




class FoodFunctionality
{

public:
    FoodFunctionality();


    // tietojenkäsittely
    void gatherFromWeb(const std::string& searchFor);
    std::vector<std::string> getAllProducts();
    std::vector<std::pair<std::string, int>> getRecipeProducts(const std::string& recipeName);

    bool addProductToDatabase(const std::shared_ptr<Product>& product);
    void pullProductsFromDatabase();

    bool addRecipeToDatabase(const std::string& recipeName, const std::vector<std::pair<std::string, int>>& recipeProducts);
    void pullRecipesFromDatabase();




    // tuotteiden käsittely
    void addFoundProduct(const std::string& name, double& price, double& pricePerKg);
    bool addProduct(const std::string& name, const std::string& descName);


    // reseptien käsittely
    bool recipeExists(const std::string recipeName);
    void createRecipe(const std::string recipeName);
    void addProductToRecipe(const std::string recipeName, std::string productName, int amount);



    // muut funktiot
    std::vector<std::string> split(const std::string& str, char delim = ';');


    //testifunktiot
    void printAllProducts() const;
    void tulostaFoundProductsKoko();
    void tulostaRecipes();

private:

    // pitää  kirjaa tallennetuista tuotteista
    allProducts allProducts_;       // map<string, osoitin tuoteolioon>

    // pitää kirjaa haetuista (ja löydetyistä tuotteesta)
    // tyhjennetään, kun yksi haetuista tuotteista lisätään allProducts rakenteeseen
    foundProducts foundProducts_;   // map<strig, osoitin tuoteolioon>,

    // pitää kirjaa kaikista luoduista resepteistä
    allRecipes allRecipes_;    // map<string, vector<pair<tuote-olio, määrä>>
};


#endif // FOODFUNCTIONALITY_HH
