#ifndef PRODUCT_H
#define PRODUCT_H
#include <string>
#include <iostream>

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



    // Return funktiot
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
