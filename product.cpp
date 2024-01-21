#include "product.hh"
#include <string>
#include <iostream>




Product::Product(const std::string& productName,
                 const std::string& descName,
                 double productPrice,
                 double productPricePerKg):
    productName_(productName),
    descName_(descName),
    productPrice_(productPrice),
    productPricePerKg_(productPricePerKg)
{

}

Product::~Product()
{

}


std::string Product::getProductName() const
{
    return productName_;
}

std::string Product::getDescName() const
{
    return descName_;
}

double Product::getProductPrice() const
{
    return productPrice_;
}

double Product::getProductPricePerKg() const
{
    return productPricePerKg_;
}














