//
//  Option.hpp
//  GroupA&B
//
//  Created by Aditya Shankar on 9/23/24.
//

// option base class containing basic data members
// we will derive american and european options from this class

#ifndef Option_hpp
#define Option_hpp

#include <iostream>
#include <boost/math/distributions/normal.hpp>
#include <cmath>
#include <vector>

// Creating an OptionData struct for convenience in creating Option objects.
// Still keeping functionality for manual creation of options by individual input parameters
struct OptionData
{
    double T;   // expiry time
    double K;   // strike price
    double sig; // volatility
    double r;   // risk free interest rate
    double S;   // current stock price at time of pricing option
    double b;   // cost of carry
    int oType;  // option type, 0 for call, -1 for put
    
    OptionData(double expiry = 0, double strike = 0, double volatility = 0, double intRate = 0, double stockPrice = 0, double costOfCarry = 0,
               int type = 0) : T(expiry), K(strike), sig(volatility), r(intRate), S(stockPrice), b(costOfCarry), oType(type) {}
};


class Option
{
private:
    OptionData optData;
    
public:
    
    
    
    
    // getter function for optData since it is private
    OptionData getOptionData() const
    {
        return optData;
    }
    
    // setter function for optData
    void setOptData(const OptionData& source)
    {
        optData = source;
    }
    
    void SetUnderlyingStockPrice(double price)
    {
        optData.S = price;
    }
    
    
    // default constructor uses default optionData struct constructor and initializes all values to 0
    Option() : optData(OptionData()) {}
    
    // constructor with initialized data
    Option(OptionData source) : optData(source) {}

    // copy constructor sets option data to source option data
    Option(const Option& source) : optData(source.getOptionData()) {}
    
    
    // destructor
    ~Option() {}
    
    // pure virtual pricing function to be implemented in derived class
    virtual double Price() const = 0;
    
    
};




#endif /* Option_hpp */
