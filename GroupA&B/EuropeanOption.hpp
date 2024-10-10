//
//  EuropeanOption.hpp
//  GroupA&B
//
//  Created by Aditya Shankar on 9/23/24.
//


// European option class derived from base class Option.
// It has all the same data members but additionally pricing functionality

#ifndef EuropeanOption_hpp
#define EuropeanOption_hpp

#include <iostream>
#include <boost/math/distributions/normal.hpp>
#include <cmath>
#include <vector>

#include "Option.hpp"

using namespace std;


class EuropeanOption : public Option
{
    
public:
    // constructors
    
    // EuropeanOption(double expiry, double strike, double volatility, double intRate, double stockPrice, int type) : Option(expiry, strike, volatility, intRate, stockPrice, type){}
    
    // Default constructor
    EuropeanOption() : Option() {}
    
    // Contructor with initialized values
    EuropeanOption(OptionData optData) : Option(optData) {}
    
    
    // copy constructor
    EuropeanOption(const EuropeanOption& source) : Option(source) {}
    
    
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
    

    
    // calculate and return call price
    double CallPrice() const
    {
        
        boost::math::normal_distribution<> N(0, 1); // standard normal distrubution to be used in Black-Scholes equation
        
        // Getting individual data memebers of struct OptionData for computation purposes
        double S = getOptionData().S;
        double K = getOptionData().K;
        double r = getOptionData().r;
        double b = getOptionData().b;
        double sig = getOptionData().sig;
        double T = getOptionData().T;
        
        double d1 = (log(S/K) + (b + ((sig * sig)/2)) * T) / (sig * sqrt(T));
        
        double d2 = d1 - (sig * sqrt(T));
        
        // computing call price using black scholes where b = r
        // therefore e^*(b-r)*T) becomes 1
        double C = S * cdf(N, d1) - K * exp(-r * T) * cdf(N, d2);
        
        return C;
        
    }
    
    
    // calculate and return put price
    double PutPrice() const
    {
        boost::math::normal_distribution<> N(0, 1); // standard normal distrubution to be used in Black-Scholes equation
        
        // Getting individual data memebers of struct OptionData for computation purposes
        double S = getOptionData().S;
        double K = getOptionData().K;
        double r = getOptionData().r;
        double b = getOptionData().b;
        double sig = getOptionData().sig;
        double T = getOptionData().T;
        
        double d1 = (log(S/K) + (b + ((sig * sig)/2)) * T) / (sig * sqrt(T));
        
        double d2 = d1 - (sig * sqrt(T));
        
        // computing put price
        double P = K * exp(-r * T) * cdf(N, -d2) - S * cdf(N, -d1);
        
        return P;
        
    }
    
    /*
    // Calculates option prices for monotonically increasing underlying stock price
    vector<double> CalculateOptionPrice(vector<double> underlyingPrice, const EuropeanOption& source);
    
    // Outputs a vector of doubles containing option prices
    vector<double> MatrixPriceCalculator(vector<OptionData> meshParameters);
    */
    
    
    // GREEKS
    
    double callDelta() const;
    double putDelta() const;
    
    
    double callGamma() const;
    double putGamma() const;
    
    
    // greeks using divided difference methods
    double ddmDelta(double h) const;
    double ddmGamma(double h) const;
    
    
    
    
    
    
};







#endif /* EuropeanOption_hpp */
