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

int main()
{
    // Record start time
    auto start = std::chrono::high_resolution_clock::now();
    Config config("data/config.txt");

    int numIterations = 100000;

    MonteCarlo(config, numIterations);

    // Record end time
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    return 0;
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
    std::cout<<"Acceptance fraction: "<<acceptanceFracTranslations<<std::endl;

    int acceptedSwaps = system.GetAcceptedSwaps();
    int rejectedSwaps = system.GetRejectedSwaps();
    int attemptedSwaps = acceptedSwaps + rejectedSwaps;
    double acceptanceFracSwaps = (double) acceptedSwaps/attemptedSwaps;
    std::cout<<"Attempted translations: "<<attemptedSwaps<<std::endl;
    std::cout<<"Accepted translations: "<<acceptedSwaps<<std::endl;
    std::cout<<"Acceptance fraction: "<<acceptanceFracSwaps<<std::endl;

    exportedStates = system.GetStates();
    Export2DVector(exportedStates);
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
