#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>
#include <iostream>

/**
 * Tuotetta kuvaava luokka johon tallennetaan:
 *      -tuotteen nimi
 *      -tuotteelle itseannettu, kuvaava nimi
 *      -hinta
 *      -kilohinta
 */
class Product
{
public:
    //  Constructor
    // Product name = Virallinen nimi verkkosivuilla
    // Descriptive name = Itse asetettu tuotetta kuvaava nimi
    Product(const std::string& productName,
            const std::string& descName,
            double productPrice,
            double productPricePerKg);

    // Destructor
    ~Product();

    // Disabling the default copy constructor and assignment
    Product(const Product& initialValue) = delete;
    Product& operator=(const Product& assignableValue) = delete;



    // Getter funktiot
    std::string getProductName() const;
    std::string getDescName() const;
    double getProductPrice() const;
    double getProductPricePerKg() const;



private:
    std::string productName_;
    std::string descName_;
    double productPrice_;
    double productPricePerKg_;


};

#endif // PRODUCT_H
