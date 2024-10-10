//
//  main.cpp
//  GroupA&B
//
//  Created by Aditya Shankar on 9/22/24.
//

#include "EuropeanOption.hpp"
#include "AmericanOption.hpp"
#include <vector>

using namespace std;

// global function generates mesh array to compute range of option prices
// given range of prices for underlying stock
vector<double> CreateMesh(double start, double end, double increment)
{
    vector<double> meshArray;
    
    for(double i = start; i <= end; i += increment)
    {
        meshArray.push_back(i);
    }
    
    return meshArray;
}


// function that uses an array of underlying stock prices
// to compute array of option prices
vector<double> calculateOptionPriceEuropean(vector<double> underlyingPrice, const EuropeanOption& source)
{
    vector<double> priceArray;
    
    
    
    for(double price:underlyingPrice)
    {
        EuropeanOption temp(source);
        temp.SetUnderlyingStockPrice(price);
        priceArray.push_back(temp.Price());
    }
    
    return priceArray;
}

// function that uses an array of underlying stock prices
// to compute array of call delta prices
vector<double> calculateCallOptionDelta(vector<double> underlyingPrice, const EuropeanOption& source)
{
    vector<double> priceArray;
    
    
    
    for(double price:underlyingPrice)
    {
        EuropeanOption temp(source);
        temp.SetUnderlyingStockPrice(price);
        priceArray.push_back(temp.callDelta());
    }
    
    return priceArray;
}

// function that uses an array of underlying stock prices
// to compute array of put delta prices
vector<double> calculatePutOptionDelta(vector<double> underlyingPrice, const EuropeanOption& source)
{
    vector<double> priceArray;
    
    
    
    for(double price:underlyingPrice)
    {
        EuropeanOption temp(source);
        temp.SetUnderlyingStockPrice(price);
        priceArray.push_back(temp.putDelta());
    }
    
    return priceArray;
}

vector<double> calculateOptionPriceAmerican(vector<double> underlyingPrice, const AmericanOption& source)
{
    vector<double> priceArray;
    
    
    
    for(double price:underlyingPrice)
    {
        AmericanOption temp(source);
        temp.SetUnderlyingStockPrice(price);
        priceArray.push_back(temp.Price());
    }
    
    return priceArray;
}


// using put price to calculate call price
double CallGivenPut(double putPrice, const OptionData& optData)
{
    return putPrice + optData.S - (optData.K * exp(-optData.r * optData.T));
}
    
    
// using call price to calculate put price
double PutGivenCall(double callPrice, const OptionData& optData)
{
    return callPrice - optData.S + (optData.K * exp(-optData.r * optData.T));
}
    
    
// testing if a pair of call and put prices satisfy parity
bool testParity(double callPrice, double putPrice, const OptionData& source, double tolerance)
{
    return abs( callPrice + source.K * exp(-source.r * source.T) - putPrice - source.S) < tolerance;
}






// If we visualize the struct OptionData as a vector of doubles,
// then a vector of <OptionData> is essentially a matrix
// hence I used each element of my vector as an OptionData struct
// and modified the required parameter of each OptionData
// according the user entered data
// Hence the vector<double> testParameters modifies the required element of
// each OptionData in our vector<OptionData>
vector<OptionData> optionParameters(OptionData baseCase, vector<double> testParameters, int paramType)
{
    vector<OptionData> meshParameters;
    
    for(int i = 0; i < testParameters.size(); i++)
    {
        OptionData temp = baseCase;
        
        switch(paramType)
        {
            case 1 : temp.T = testParameters[i]; break;
            case 2 : temp.K = testParameters[i]; break;
            case 3 : temp.sig = testParameters[i]; break;
            case 4 : temp.r = testParameters[i]; break;
            case 5 : temp.S = testParameters[i]; break;
            case 6 : temp.oType = testParameters[i]; break;
        }
        
        meshParameters.push_back(temp);
    }
    
    
    return meshParameters;
}



vector<double> matrixPriceCalculatorEuropean(vector<OptionData> meshParameters)
{
    vector<double> calculatedPrices;
    
    for(int i = 0; i < meshParameters.size(); i++)
    {
        EuropeanOption temp;
        temp.setOptData(meshParameters[i]);
        
        calculatedPrices.push_back(temp.Price());
    }
    
    return calculatedPrices;
    
}

// matrix pricer for delta
vector<double> matrixPriceCalculatorDelta(vector<OptionData> meshParameters)
{
    vector<double> calculatedPrices;
    
    for(int i = 0; i < meshParameters.size(); i++)
    {
        EuropeanOption temp;
        temp.setOptData(meshParameters[i]);
        
        // if-else statement so we can avoid re-writing code for call deltas and put deltas
        if(temp.getOptionData().oType == 0)
        {
            calculatedPrices.push_back(temp.callDelta());
        }
        else
        {
            calculatedPrices.push_back(temp.putDelta());
        }
    }
    
    return calculatedPrices;
    
}

// matrix pricer for gamme
vector<double> matrixPriceCalculatorGamma(vector<OptionData> meshParameters)
{
    vector<double> calculatedPrices;
    
    for(int i = 0; i < meshParameters.size(); i++)
    {
        EuropeanOption temp;
        temp.setOptData(meshParameters[i]);
        
        calculatedPrices.push_back(temp.callGamma());
        
    }
    
    return calculatedPrices;
    
}

vector<double> matrixPriceCalculatorAmerican(vector<OptionData> meshParameters)
{
    vector<double> calculatedPrices;
    
    for(int i = 0; i < meshParameters.size(); i++)
    {
        AmericanOption temp;
        temp.setOptData(meshParameters[i]);
        
        calculatedPrices.push_back(temp.Price());
    }
    
    return calculatedPrices;
    
}






int main()
{
    //////////////////////////// GROUP A QUESTION SET 1 /////////////////////////
    cout << "----------GROUP A QUESTION SET 1----------" << endl << endl;
    
    // Testing EuropeanOption class with following data batches
    // First test using call options
    // cod stands for call option data
    OptionData cod1(0.25, 65, 0.3, 0.08, 60, 0.08, 0);
    OptionData cod2(1, 100, 0.2, 0, 100, 0, 0);
    OptionData cod3(1, 10, 0.5, 0.12, 5, 0.12, 0);
    OptionData cod4(30.0, 100.0, 0.30, 0.08, 100.0, 0.08, 0);
    
    // ceu stands for call european option
    EuropeanOption ceu1(cod1);
    EuropeanOption ceu2(cod2);
    EuropeanOption ceu3(cod3);
    EuropeanOption ceu4(cod4);
    
    cout << "---------- PART A ----------" << endl;
    cout << "Price of Call Option 1: " << ceu1.Price() << endl;
    cout << "Price of Call Option 2: " << ceu2.Price() << endl;
    cout << "Price of Call Option 3: " << ceu3.Price() << endl;
    cout << "Price of Call Option 4: " << ceu4.Price() << endl << endl << endl;
    
    
    // Now test using put options
    // pod stands for put option data
    OptionData pod1(0.25, 65, 0.3, 0.08, 60, 0.08, -1);
    OptionData pod2(1, 100, 0.2, 0, 100, 0, -1);
    OptionData pod3(1, 10, 0.5, 0.12, 5, 0.12, -1);
    OptionData pod4(30.0, 100.0, 0.30, 0.08, 100.0, 0.08, -1);
    
    // peu stands for put european option
    EuropeanOption peu1(pod1);
    EuropeanOption peu2(pod2);
    EuropeanOption peu3(pod3);
    EuropeanOption peu4(pod4);
    
    cout << "Price of Put Option 1: " << peu1.Price() << endl;
    cout << "Price of Put Option 2: " << peu2.Price() << endl;
    cout << "Price of Put Option 3: " << peu3.Price() << endl;
    cout << "Price of Put Option 4: " << peu4.Price() << endl;
     
     
    cout << endl << endl << endl;
    
     
     
    cout << "---------- PART B ----------" << endl;
    
    // testing parity functions
    double ptest = CallGivenPut(peu1.Price(), peu1.getOptionData());
    cout << "Price of call 1 given put price: " << ptest << endl;
    
    double ptest2 = PutGivenCall(ceu1.Price(), ceu1.getOptionData());
    cout << "Price of put 1 given call price: " << ptest2 << endl;
    
    // testing if call price 2 and put price 2 satisfy put-call parity
    bool paritycheck = testParity(ceu2.Price(), peu2.Price(), ceu2.getOptionData(), 0.001);
    
    if(paritycheck == true)
    {
        cout << "Relationship holds parity" << endl;
    }
    else
    {
        cout << "Relationship does not hold parity" << endl;
    }
    
        
    cout << endl << endl << endl;
     
    cout << "---------- Part C ----------" << endl;
    // creating an array for calculating prices of ceu1 (call european option 1)
    vector<double> underlyingPrice = CreateMesh(45, 80, 5);
    
    // computing the range of option prices and storing them in a vector according to
    // above global funtion
    vector<double> computedOptionPrices = calculateOptionPriceEuropean(underlyingPrice, ceu1);
    
    
    // displaying option prices
    for(int i = 0; i < computedOptionPrices.size(); i++)
    {
        cout << "Underlying stock price: " << underlyingPrice[i] << " ------ Option price: " << computedOptionPrices[i] << endl;
    }
    
    cout << endl << endl << endl;
      
    cout << "---------- Part D ----------" << endl;
    
    

  
    // testing meshers
    vector<double> strikePrices{80, 85, 90, 95, 100, 105, 110, 115, 120};
    vector<OptionData> params = optionParameters(pod2, strikePrices, 2);
    
    vector<double> calculatedPrices = matrixPriceCalculatorEuropean(params);
    
    cout << endl << endl << endl << "Option prices based on matrix of input factors: " << endl;
    
    for(int i = 0; i < calculatedPrices.size(); i++)
    {
        // cout << calculatedPrices[i] << endl;
        cout << "Given Prices: " << strikePrices[i] << " => Calculated option price: " << calculatedPrices[i] << endl;
    }
    
    

    
    
    
    
    
    
    
    //////////////////////////// GROUP A QUESTION SET 2 - THE GREEKS /////////////////////////
    
    
    cout << endl << endl;
    cout << "----------GROUP A QUESTION SET 2: THE GREEKS----------" << endl << endl;
    
    cout << "---------- Part A----------" << endl;
    
    // Testing gamma function using given option data
    OptionData greekTest(0.5, 100, 0.36, 0.1, 105, 0, 0);
    
    EuropeanOption callOptionTest(greekTest);
    
    cout << "Greeks" << endl;
    
    cout << "Call Delta: " << callOptionTest.callDelta() << endl;
    cout << "Put Delta: " << callOptionTest.putDelta() << endl << endl;
    
    cout << "Call Gamma: " << callOptionTest.callGamma() << endl;
    cout << "Put Gamma: " << callOptionTest.putGamma() << endl;
    

    cout << endl << "---------- Part B----------" << endl;
    
    // Testing greek computing for monotonically increasing underlying stock price
    vector<double> underlying = CreateMesh(90, 120, 5);
    
    vector<double> computedCallPrices = calculateCallOptionDelta(underlying, callOptionTest);
    
    
    cout << endl << endl << endl << "Option deltas based on monotonically increasing underlying: " << endl;
    
    for(int i = 0; i < computedCallPrices.size(); i++)
    {
        // cout << calculatedPrices[i] << endl;
        cout << "Given Prices: " << underlying[i] << " => Calculated call delta price: " << computedCallPrices[i] << endl;
    }
    
    
    
    cout << endl << "---------- Part C----------" << endl;
    
    // testing matrix pricer:
    // in this example we input a matrix of option data with varying underlying prices
    // and compute greeks
    vector<OptionData> greekTester = optionParameters(greekTest, underlying, 5);
    
    vector<double> computedCallDeltas = matrixPriceCalculatorDelta(greekTester);
    vector<double> computedCallGammas = matrixPriceCalculatorGamma(greekTester);
    
    cout << "Computing greeks given matrix of optiondata: " << endl;
    
    for(int i = 0; i < greekTester.size(); i++)
    {
        cout << "Price: " << underlying[i] << " =====> Delta: " << computedCallDeltas[i] << ", Gamma: " << computedCallGammas[i] << endl;
    }
    
    cout << endl << endl;
    
    cout << "----------PART D----------" << endl;
    

    cout << "Call Delta using formula: " << callOptionTest.callDelta() << endl;
    cout << "Call Delta using DDM:     " << callOptionTest.ddmDelta(1) << endl << endl;
    
    cout << "Call Gamma using formula: " << callOptionTest.callGamma() << endl;
    cout << "Call Gamma using DDM:     " << callOptionTest.ddmGamma(1) << endl << endl;
    
    
    
    
    
    //////////////////////////// GROUP B /////////////////////////
    
    
    cout << endl << endl << endl;
    cout << "-------------------- GROUB B --------------------" << endl << endl;
    
    cout << "---------- Part A & B ----------" << endl;
    
    // Testing American Options with given data
    OptionData sCall(0, 100, 0.1, 0.1, 110, 0.02, 0);
    OptionData sPut(0, 100, 0.1, 0.1, 110, 0.02, -1);
    
    AmericanOption optCall(sCall);
    AmericanOption optPut(sPut);
    
    cout << "Call Price: " << optCall.Price() << endl;
    cout << "Put Price: " << optPut.Price() << endl;
    
    
    cout << endl << "---------- Part C ----------" << endl;
    
    
    // creating an array for calculating prices of optCall
    vector<double> underlyingPrice2 = CreateMesh(100, 125, 5);
    
    // computing the range of option prices and storing them in a vector according to
    // above global funtion
    vector<double> computedOptionPrices2 = calculateOptionPriceAmerican(underlyingPrice2, optCall);
    
    
    // displaying option prices
    for(int i = 0; i < computedOptionPrices2.size(); i++)
    {
        cout << "Underlying stock price: " << underlyingPrice2[i] << " ------ Option price: " << computedOptionPrices2[i] << endl;
    }
    
    cout << endl << endl;
    
    
    cout << "---------- Part D ---------" << endl;
    
    
    

    
    // Testing our mesh for pricing call options with varing volatility sigma

    // Creating a mesh with varying volatility
    vector<double> volatility = CreateMesh(0.05, 0.20, 0.05);
    
    
    // creating a "matrix" of OptionData with varying volatilities,
    // and all other parameters same
    vector<OptionData> params2 = optionParameters(sCall, volatility, 3);
    
    // calculating prices and storing them in a vector
    vector<double> calculatedPrices2 = matrixPriceCalculatorAmerican(params2);
     
    cout << endl << "Option prices based on matrix of input factors: " << endl;
    
    for(int i = 0; i < calculatedPrices2.size(); i++)
    {
        cout << "Given volatility: " << volatility[i] << " => Calculated option price: " << calculatedPrices2[i] << endl;
    }
    
    
    
    return 0;
    
}
