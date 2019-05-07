// Place #define NDEBUG if asserts should not be evaluated.

#include <iostream>
#include <cassert>
#include <string>
#include <fstream>
#include <vector>
#include <array>

#include "system.h"
#include "config.h"
#include "timer.h"

#include "export2D.h"

void MonteCarlo(Config config, bool usePreviousStates);
void PrintAcceptanceInfo(const System& system, int numIterations);
void CopyFile(const std::string& sourcePath, const std::string& destinationPath);

int main(int argc, char* argv[])
{
    Timer timer;

    bool usePreviousStates;
    if(argc>1)
    {
        if(argv[1]==std::string("y"))
        {
            usePreviousStates = true;
        }
        else
        {
            usePreviousStates = false;
        }
    }

    std::string configFile = "config.txt";
    Config config(configFile);

    CopyFile(configFile, "data/lastConfig.txt");

    MonteCarlo(config, usePreviousStates);
}

void MonteCarlo(Config config, bool usePreviousStates)
{
    System system(config, usePreviousStates);

    int numIterations = config.GetNumIterations();

    std::string outputStatesFile = "data/outputStates.txt";
    ClearContents(outputStatesFile);
    std::vector<std::vector<double>> exportedStates;

    std::vector<int> iterations;
    std::vector<double> totalEnergy;
    std::vector<double> pressure;

    int prevAcceptedSwaps = 0;
    int prevAcceptedTranslations = 0;
    std::vector<double> swapAcceptance;
    std::vector<double> translationAcceptance;

    const int skipSample = 10000;

    int progress = 0;
    for(int i=0; i<numIterations; ++i)
    {
        if(i%skipSample==0)
        {
            exportedStates = system.GetStates();
            Export2D(exportedStates, outputStatesFile, i);

            iterations.push_back(i);
            totalEnergy.push_back(system.GetTotalEnergy());

            pressure.push_back(system.GetPressure());

            int numAcceptedSwaps = system.GetAcceptedSwaps();
            int numAcceptedTranslations = system.GetAcceptedTranslations();
            double swapRatio = (double) (numAcceptedSwaps-prevAcceptedSwaps)/skipSample;
            double translationRatio = (double) (numAcceptedTranslations-prevAcceptedTranslations)/skipSample;
            swapAcceptance.push_back(swapRatio);
            translationAcceptance.push_back(translationRatio);
            prevAcceptedSwaps = numAcceptedSwaps;
            prevAcceptedTranslations = numAcceptedTranslations;
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
    Export1D(pressure, "data/pressure.txt");
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
    double totalAcceptanceFracSwaps = (double) acceptedSwaps/numIterations;
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
