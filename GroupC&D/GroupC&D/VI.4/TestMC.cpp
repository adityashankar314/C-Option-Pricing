// HardCoded.cpp
//
// C++ code to price an option, essential algorithms.
//
// We take CEV model with a choice of the elaticity parameter
// and the Euler method. We give option price and number of times
// S hits the origin.
//
// (C) Datasim Education BC 2008-2011
//

#include "OptionData.hpp" 
#include "UtilitiesDJD/RNG/NormalGenerator.hpp"
#include "UtilitiesDJD/Geometry/Range.cpp"
#include <cmath>
#include <iostream>

template <class T> void print(const std::vector<T>& myList)
{  // A generic print function for vectors
	
	std::cout << std::endl << "Size of vector is " << myList.size() << "\n[";

	// We must use a const iterator here, otherwise we get a compiler error.
    typename std::vector<T>::const_iterator i;
	for (i = myList.begin(); i != myList.end(); ++i)
	{
			std::cout << *i << ",";

	}

	std::cout << "]\n";
}

namespace SDEDefinition
{ // Defines drift + diffusion + data

	OptionData* data;				// The data for the option MC

	double drift(double t, double X)
	{ // Drift term
	
		return (data->r)*X; // r - D
	}

	
	double diffusion(double t, double X)
	{ // Diffusion term
	
		double betaCEV = 1.0;
		return data->sig * pow(X, betaCEV);
		
	}

	double diffusionDerivative(double t, double X)
	{ // Diffusion term, needed for the Milstein method
	
		double betaCEV = 1.0;
		return 0.5 * (data->sig) * (betaCEV) * pow(X, 2.0 * betaCEV - 1.0);
	}
} // End of namespace



// function to calculate standard deviation
double CalculateSD(vector<double> prices, OptionData source, int NSim)
{
    double cSquare = 0;
    double cSumSquare = 0;
    double temp1 = 0;
    double temp2 = 0;
    
    for(int i = 0; i < prices.size(); i++)
    {
        // calculating first term in SD formula
        temp1 = prices[i];
        cSquare += (temp1 * temp1);
        
        // calculating second term in SD formula
        temp2 += prices[i];
    }
    
    cSumSquare = (temp2 * temp2);
    
    double SD = sqrt(((cSquare - (cSumSquare / NSim)) / (NSim - 1)) * exp(source.r * source.T * -1) );
    
    return SD;
}

double CalculateSE(double SD, double NSim)
{
    return SD / sqrt(NSim);
}


int main()
{
	std::cout <<  "1 factor MC with explicit Euler\n";
	OptionData myOption;
    
    
    // BATCH 1
	myOption.K = 65.0;
	myOption.T = 0.25;
	myOption.r = 0.08;
	myOption.sig = 0.3;
	myOption.type = -1;	// Put -1, Call +1
	double S_0 = 60;
    
    
    /*
    // BATCH 2
    myOption.K = 100.0;
    myOption.T = 1.0;
    myOption.r = 0.0;
    myOption.sig = 0.2;
    myOption.type = -1;    // Put -1, Call +1
    double S_0 = 100;
    */
    
    /*
    // BATCH 4
    myOption.K = 100.0;
    myOption.T = 30;
    myOption.r = 0.08;
    myOption.sig = 0.3;
    myOption.type = -1;    // Put -1, Call +1
    double S_0 = 100;
    */
    
    // Vectors to store values at the end for easy printing
    std::vector<double> finalPrice;
    std::vector<double> differences;
    std::vector<double> standardDeviations;
    std::vector<double> standardErrors;
    
    double numTrials;
    cout << "Enter the number of trials you want to run: " << endl;
    cin >> numTrials;
    
    for(int i = 0; i < numTrials; i++)
    {
        
        long N = 100;
        std::cout << "Number of subintervals in time: ";
        std::cin >> N;
        
        // Create the basic SDE (Context class)
        Range<double> range (0.0, myOption.T);
        double VOld = S_0;
        double VNew;
        
        std::vector<double> x = range.mesh(N);
        
        
        // V2 mediator stuff
        long NSim = 50000;
        std::cout << "Number of simulations: ";
        std::cin >> NSim;
        
        double k = myOption.T / double (N);
        double sqrk = sqrt(k);
        
        // Normal random number
        double dW;
        double price = 0.0;	// Option price
        
        // NormalGenerator is a base class
        NormalGenerator* myNormal = new BoostNormal();
        
        using namespace SDEDefinition;
        SDEDefinition::data = &myOption;
        
        std::vector<double> res;
        int coun = 0; // Number of times S hits origin
        
        // vector of prices that will be used to calculate SD
        std::vector<double> prices(NSim);
        
        // A.
        for (long i = 1; i <= NSim; ++i)
        { // Calculate a path at each iteration
            
            if ((i/10000) * 10000 == i)
            {// Give status after each 1000th iteration
                
                std::cout << i << std::endl;
            }
            
            VOld = S_0;
            for (unsigned long index = 1; index < x.size(); ++index)
            {
                
                // Create a random number
                dW = myNormal->getNormal();
                
                // The FDM (in this case explicit Euler)
                VNew = VOld  + (k * drift(x[index-1], VOld))
                + (sqrk * diffusion(x[index-1], VOld) * dW);
                
                VOld = VNew;
                
                // Spurious values
                if (VNew <= 0.0) coun++;
            }
            
            double tmp = myOption.myPayOffFunction(VNew);
            price += (tmp)/double(NSim);
            prices.push_back(price);    // Adding price at each step to our prices vector
        }
        
        // D. Finally, discounting the average price
        price *= exp(-myOption.r * myOption.T);
        
        // Cleanup; V2 use scoped pointer
        delete myNormal;
        
        // Calculating Standard Error
        double SD = CalculateSD(prices, myOption, NSim);
        double SE = CalculateSE(SD, NSim);
        
        
        
        finalPrice.push_back(price);
        differences.push_back(price - 5.84628);
        standardDeviations.push_back(SD);
        standardErrors.push_back(SE);
        
        
        /*
        std::cout << "Price, after discounting: " << price << ", " << std::endl;
        std::cout << "Number of times origin is hit: " << coun << endl;
        std::cout << "Difference from exact solution: " << price - 5.84628 << endl;
        std::cout << "Standard Error: " << SE << endl;
         */
    }
    
    for(int i = 0; i < finalPrice.size(); i++)
    {
        cout << "Price: " << finalPrice[i] << "\t\tDifference: " << differences[i] << "\t\tStandard Deviation: " << standardDeviations[i] << "\t\tStandard Error: " << standardErrors[i] << endl;
    }
    

	return 0;
}
