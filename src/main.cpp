// Place #define NDEBUG if asserts should not be evaluated.

#include <iostream>
#include <chrono>
#include <cassert>
#include <string>
#include <fstream>
#include <vector>
#include <array>

#include "system.h"
#include "config.h"

#include "export2D.h"

void MonteCarlo(Config config);
void PrintAcceptanceInfo(const System& system, int numIterations);
void CopyFile(const std::string& sourcePath, const std::string& destinationPath);

int main()
{
    auto startTime = std::chrono::high_resolution_clock::now();

    std::string configFile = "config.txt";
    Config config(configFile);

    CopyFile(configFile, "data/lastConfig.txt");

    MonteCarlo(config);

    auto finishTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsedTime = finishTime - startTime;
    std::cout << "Elapsed time: " << elapsedTime.count() << " s" << std::endl;
}

void MonteCarlo(Config config)
{
    System system(config);

    int numIterations = config.GetNumIterations();

    std::string outputStatesFile = "data/outputStates.txt";
    ClearContents(outputStatesFile);
    std::vector<std::vector<double>> exportedStates;

    std::vector<int> iterations;
    std::vector<double> totalEnergy;

    int prevAcceptedSwaps = 0;
    int prevAcceptedTranslations = 0;
    std::vector<double> swapAcceptance;
    std::vector<double> translationAcceptance;

    int progress = 0;
    for(int i=0; i<numIterations; ++i)
    {
        if(i%10000==0)
        {
            exportedStates = system.GetStates();
            Export2D(exportedStates, outputStatesFile, i);

            iterations.push_back(i);
            totalEnergy.push_back(system.GetTotalEnergy());

            swapAcceptance.push_back(system.GetAcceptedSwaps()-prevAcceptedSwaps);
            translationAcceptance.push_back(system.GetAcceptedTranslations()-prevAcceptedTranslations);
            prevAcceptedSwaps = system.GetAcceptedSwaps();
            prevAcceptedTranslations = system.GetAcceptedTranslations();
        }

        if(config.GetSwapProbability() > 0)
        {
            system.AttemptSwap();
        }

        system.AttemptTranslation();


        if(i%((int) (numIterations-1)/10 )==0)
        {
            if(i!=0)
            {
                progress = 100*i/(numIterations-1);
                std::cout<<progress<<"%"<<std::endl;
            }
        }
    }

    Export1D(iterations, "data/iterations.txt");
    Export1D(totalEnergy, "data/energy.txt");
    Export1D(swapAcceptance, "data/swapAcceptance.txt");
    Export1D(translationAcceptance, "data/translationAcceptance.txt");

    PrintAcceptanceInfo(system, numIterations);
}

void PrintAcceptanceInfo(const System& system, int numIterations)
{
    int acceptedTranslations = system.GetAcceptedTranslations();
    double acceptanceFracTranslations = (double) acceptedTranslations/numIterations;
    std::cout<<"Accepted/attempted translations: "<<acceptanceFracTranslations<<std::endl;

    int acceptedSwaps = system.GetAcceptedSwaps();
    int rejectedSwaps = system.GetRejectedSwaps();
    int attemptedSwaps = acceptedSwaps + rejectedSwaps;
    double acceptanceFracSwaps = (double) acceptedSwaps/attemptedSwaps;
    double totalAcceptanceFracSwaps = (double) acceptedSwaps/numIterations;
    std::cout<<"Accepted/attempted swaps: "<<acceptanceFracSwaps<<std::endl;
    std::cout<<"Accepted/numIterations: "<<totalAcceptanceFracSwaps<<std::endl;
}

void CopyFile(const std::string& sourceFile, const std::string& destinationFile)
{
    std::ifstream inFile(sourceFile);
    std::ofstream outFileClear(destinationFile, std::ios_base::trunc);
    std::ofstream outFile(destinationFile, std::ios_base::app);
    if(inFile.is_open())
    {
        std::string line;
        while(getline(inFile, line))
        {
            outFile << line;
            outFile << "\n";
        }
    }

}
