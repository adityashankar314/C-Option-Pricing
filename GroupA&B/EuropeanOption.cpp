//
//  EuropeanOption.cpp
//  GroupA&B
//
//  Created by Aditya Shankar on 9/25/24.
//

#include "EuropeanOption.hpp"

#include <iostream>
#include <boost/math/distributions/normal.hpp>
#include <cmath>
#include <vector>

using namespace std;


/*
// Calculates option price based on input of matrix containing option parameters
vector<double> EuropeanOption::MatrixPriceCalculator(vector<OptionData> meshParameters)
{
    // return object containing prices
    vector<double> calculatedPrices;
    
    for(int i = 0; i < meshParameters.size(); i++)
    {
        // setting each element of matrix to the correct updated optiondata
        EuropeanOption temp;
        temp.setOptData(meshParameters[i]);
        
        // adding updated option data onto our output vector
        calculatedPrices.push_back(temp.Price());
    }
    
    return calculatedPrices;
    
}


vector<double> EuropeanOption::CalculateOptionPrice(vector<double> underlyingPrice, const EuropeanOption& source)
{
    
    // return object
    vector<double> priceArray;
    
    
    for(double price:underlyingPrice)
    {
        // creating a temp object and changing its underlying stock price
        // then calculating price and adding it to our output vector 
        EuropeanOption temp(source);
        temp.SetUnderlyingStockPrice(price);
        priceArray.push_back(temp.Price());
    }
    
    return priceArray;
}
*/


// GREEKS

// pdf formula
double norm_pdf(const double& x) {
    return (1.0 / (sqrt(2.0 * M_PI))) * exp(-0.5 * x * x);
}


double EuropeanOption::callDelta() const
{
    
    boost::math::normal_distribution<> N(0, 1); // standard normal distrubution
    
    // Getting individual data memebers of struct OptionData for computation purposes
    double S = getOptionData().S;
    double K = getOptionData().K;
    double r = getOptionData().r;
    double b = getOptionData().b;
    double sig = getOptionData().sig;
    double T = getOptionData().T;

    double d1 = (log(S/K) + (b + ((sig * sig)/2)) * T) / (sig * sqrt(T));
    
    return exp((b - r) * T) * cdf(N, d1);
    
}

double EuropeanOption::putDelta() const
{
    
    boost::math::normal_distribution<> N(0, 1); // standard normal distrubution
    
    // Getting individual data memebers of struct OptionData for computation purposes
    double S = getOptionData().S;
    double K = getOptionData().K;
    double r = getOptionData().r;
    double b = getOptionData().b;
    double sig = getOptionData().sig;
    double T = getOptionData().T;

    double d1 = (log(S/K) + (b + ((sig * sig)/2)) * T) / (sig * sqrt(T));
    
    return exp((b - r) * T) * (cdf(N, d1) - 1);
    
}





double EuropeanOption::callGamma() const
{
    boost::math::normal_distribution<> N(0, 1); // standard normal distrubution
    
    
    
    // Getting individual data memebers of struct OptionData for computation purposes
    double S = getOptionData().S;
    double K = getOptionData().K;
    double r = getOptionData().r;
    double b = getOptionData().b;
    double sig = getOptionData().sig;
    double T = getOptionData().T;

    double d1 = (log(S/K) + (b + ((sig * sig)/2)) * T) / (sig * sqrt(T));
    
    // break down elements of gamma formula for easier readibility
    // double numerator = exp((b - r) * T) * cdf(
    return norm_pdf(d1) * exp((b - r) * T) / (S * sig * sqrt(T));
    
    
}

double EuropeanOption::putGamma() const
{
    return callGamma();
}

// Divided differece methods:

// double h is our step size for calculation
double EuropeanOption::ddmDelta(double h) const
{
    // Creating two option objects with slightly varying underlying price
    EuropeanOption tempPlus = (*this);
    tempPlus.SetUnderlyingStockPrice(tempPlus.getOptionData().S + h);
    
    EuropeanOption tempMinus = (*this);
    tempMinus.SetUnderlyingStockPrice(tempMinus.getOptionData().S - h);
    
    // using tempPlus and tempMinus in the DDM formula
    
    return (tempPlus.Price() - tempMinus.Price()) / (2 * h);
    

}


double EuropeanOption::ddmGamma(double h) const
{
    
    // Creating two option objects with slightly varying underlying price
    EuropeanOption tempPlus = (*this);
    tempPlus.SetUnderlyingStockPrice(tempPlus.getOptionData().S + h);
    
    EuropeanOption tempMinus = (*this);
    tempMinus.SetUnderlyingStockPrice(tempMinus.getOptionData().S - h);
    
    return (tempPlus.Price() + tempMinus.Price() - (2 * (*this).Price())) / (h * h);
    

}

