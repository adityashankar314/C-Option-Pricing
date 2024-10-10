//
//  AmericanOption.hpp
//  GroupA&B
//
//  Created by Aditya Shankar on 9/24/24.
//

#ifndef AmericanOption_hpp
#define AmericanOption_hpp

#include <iostream>
#include <boost/math/distributions/normal.hpp>
#include <cmath>
#include <vector>

#include "Option.hpp"

using namespace std;

class AmericanOption : public Option
{
    
public:
    
    // Default constructor
    AmericanOption() : Option() {}
    
    // Contructor with initialized values
    AmericanOption(OptionData optData) : Option(optData) {}
    
    
    // copy constructor
    AmericanOption(const AmericanOption& source) : Option(source) {}
    
    
    // overriding base class pure virtual price function
    double Price() const override
    {
        if(getOptionData().oType == 0)
        {
            return CallPrice();
        }
        else
        {
            return PutPrice();
        }
        
    }
    
    double CallPrice() const
    {
        boost::math::normal_distribution<> N(0, 1); // standard normal distrubution to be used in Black-Scholes equation
        
        // Getting individual data memebers of struct OptionData for computation purposes
        double S = getOptionData().S;
        double K = getOptionData().K;
        double r = getOptionData().r;
        double b = getOptionData().b;
        double sig = getOptionData().sig;
        // double T = getOptionData().T;
        
        double rootFirstTerm = b / (sig * sig) - 0.5;    // splitting up formula for easier readability
        double rootSecondTerm = 2 * r / (sig * sig);
        
        double y1 = 0.5 - (b / (sig * sig)) + sqrt(rootFirstTerm * rootFirstTerm + rootSecondTerm);
        
        // Power term
        double p = ((y1 - 1) / y1) * (S / K);
        
        // price
        double c = (K / (y1 - 1)) * pow(p, y1);
        
        return c;
        
    }
    
    
    double PutPrice() const
    {
        // Getting individual data memebers of struct OptionData for computation purposes
        double S = getOptionData().S;
        double K = getOptionData().K;
        double r = getOptionData().r;
        double b = getOptionData().b;
        double sig = getOptionData().sig;
        // double T = getOptionData().T;
        
        double rootFirstTerm = b / (sig * sig) - 0.5;    // splitting up formula for easier readability
        double rootSecondTerm = 2 * r / (sig * sig);
        
        double y2 = 0.5 - (b / (sig * sig)) - sqrt(rootFirstTerm * rootFirstTerm + rootSecondTerm);
        
        // power term
        double p = ((y2 - 1) / y2) * (S / K);
        
        // price
        double c = (K / (1 - y2)) * pow(p, y2);
        
        return c;
    }
    
    // Calculates option prices for monotonically increasing underlying stock price
    vector<double> CalculateOptionPrice(vector<double> underlyingPrice, const AmericanOption& source);
    
    // Outputs a vector of doubles containing option prices
    vector<double> MatrixPriceCalculator(vector<OptionData> meshParameters);

    
    
    
};

#endif /* AmericanOption_hpp */
