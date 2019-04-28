// Place #define NDEBUG if asserts should not be evaluated.

#include <iostream>
#include <chrono>
#include <cmath>
#include <fstream>
#include <cassert>

#include "system.h"
#include "config.h"

void MonteCarlo(Config config, int numIterations);
void Export2DVector(std::vector<std::vector<double>> &vector2D);
void Export2DVectorRadial(std::vector<std::vector<double>> &vector2D);

int main()
{
    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    Config config("data/config.txt");

    int numIterations = 1000000;

    MonteCarlo(config, numIterations);

    // Record end time
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;
}

void MonteCarlo(Config config, int numIterations)
{
    System system(config);

    std::vector<std::vector<double>> exportedStates = system.GetStates();

    Export2DVector(exportedStates);

    for(int i=0; i<numIterations; ++i)
    {
        if(config.GetSwapProbability() > 0)
        {
            system.AttemptSwap();
        }
        system.AttemptTranslation();
    }

    int acceptedTranslations = system.GetAcceptedTranslations();
    double acceptanceFracTranslations = (double) acceptedTranslations/numIterations;
    std::cout<<"Attempted translations: "<<numIterations<<std::endl;
    std::cout<<"Accepted translations: "<<acceptedTranslations<<std::endl;
    std::cout<<"Accepted/attempted translations: "<<acceptanceFracTranslations<<std::endl;

    int acceptedSwaps = system.GetAcceptedSwaps();
    int rejectedSwaps = system.GetRejectedSwaps();
    int attemptedSwaps = acceptedSwaps + rejectedSwaps;
    double acceptanceFracSwaps = (double) acceptedSwaps/attemptedSwaps;
    double totalAcceptanceFracSwaps = (double) acceptedSwaps/numIterations;
    double actualSwapProbability = (double) attemptedSwaps/numIterations;
    std::cout<<"Attempted swaps: "<<attemptedSwaps<<std::endl;
    std::cout<<"Accepted swaps: "<<acceptedSwaps<<std::endl;
    std::cout<<"Accepted/attempted swaps: "<<acceptanceFracSwaps<<std::endl;
    std::cout<<"Accepted/numIterations: "<<totalAcceptanceFracSwaps<<std::endl;
    std::cout<<"Actual swap probability: "<<actualSwapProbability<<std::endl;

    exportedStates = system.GetStates();
    Export2DVector(exportedStates);

    std::vector<std::vector<double>> exportedRadial = system.GetRadialDistributionFunction();
    Export2DVectorRadial(exportedRadial);
}

void Export2DVector(std::vector<std::vector<double>> &vector2D)
{
    std::ofstream outFile("data/outputStates.txt", std::ios_base::trunc);
    for(std::vector<double> vector1D : vector2D)
    {
        for(int i=0; i<4; ++i)
        {
            outFile << vector1D[i];
            if(i==3)
            {
                break;
            }
            outFile << ",";
        }
        outFile << "\n";
    }
}

void Export2DVectorRadial(std::vector<std::vector<double>> &vector2D)
{
    std::ofstream outFile("data/outputRadial.txt", std::ios_base::trunc);
    for(std::vector<double> vector1D : vector2D)
    {
        for(int i=0; i<2; ++i)
        {
            outFile << vector1D[i];
            if(i==1)
            {
                break;
            }
            outFile << ",";
        }
        outFile << "\n";
    }
}
