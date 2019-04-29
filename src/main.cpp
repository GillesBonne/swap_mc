// Place #define NDEBUG if asserts should not be evaluated.

#include <iostream>
#include <chrono>
#include <cassert>

#include "system.h"
#include "config.h"

#include "export2D.h"

void MonteCarlo(Config config);
void PrintAcceptanceInfo(const System& system, int numIterations);

int main()
{
    auto startTime = std::chrono::high_resolution_clock::now();

    Config config("config.txt");

    MonteCarlo(config);

    auto finishTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = finishTime - startTime;
    std::cout << "Elapsed time: " << elapsedTime.count() << " s" << std::endl;
}

void MonteCarlo(Config config)
{
    System system(config);

    int numIterations = config.GetNumIterations();
    for(int i=0; i<numIterations; ++i)
    {
        if(config.GetSwapProbability() > 0)
        {
            system.AttemptSwap();
        }
        system.AttemptTranslation();
    }
    PrintAcceptanceInfo(system, numIterations);

    ClearContents("data/outputStates.txt");
    std::vector<std::vector<double>> exportedStates = system.GetStates();
    Export2D(exportedStates, "data/outputStates.txt", numIterations);

    ClearContents("data/outputRadial.txt");
    std::vector<std::vector<double>> exportedRadial = system.GetRadialDistributionFunction();
    Export2D(exportedRadial, "data/outputRadial.txt", numIterations);
}

void PrintAcceptanceInfo(const System& system, int numIterations)
{
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
}
