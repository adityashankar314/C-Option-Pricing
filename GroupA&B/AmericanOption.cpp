//
//  AmericanOption.cpp
//  GroupA&B
//
//  Created by Aditya Shankar on 9/25/24.
//

#include <iostream>
#include <boost/math/distributions/normal.hpp>
#include <cmath>
#include <vector>

#include "AmericanOption.hpp"

using namespace std;

/*
// Calculates option price based on input of matrix containing option parameters
vector<double> AmericanOption::MatrixPriceCalculator(vector<OptionData> meshParameters)
{
    // return object containing prices
    vector<double> calculatedPrices;
    
    for(int i = 0; i < meshParameters.size(); i++)
    {
        // setting each element of matrix to the correct updated optiondata
        AmericanOption temp;
        temp.setOptData(meshParameters[i]);
        
        // adding updated option data onto our output vector
        calculatedPrices.push_back(temp.Price());
    }
    
    return calculatedPrices;
    
}


vector<double> AmericanOption::CalculateOptionPrice(vector<double> underlyingPrice, const AmericanOption& source)
{
    
    // return object
    vector<double> priceArray;
    
    
    for(double price:underlyingPrice)
    {
        // creating a temp object and changing its underlying stock price
        // then calculating price and adding it to our output vector
        AmericanOption temp(source);
        temp.SetUnderlyingStockPrice(price);
        priceArray.push_back(temp.Price());
    }
    
    return priceArray;
}
*/
