// Place #define NDEBUG if asserts should not be evaluated.
#define NDEBUG

#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <stdexcept>
#include <chrono>
#include <ctime>

#include "system.h"
#include "config.h"
#include "timer.h"

#include "export2D.h"

void MonteCarlo(Config config, bool usePreviousStates);
void PrintAcceptanceInfo(const System& system, int numIterations);
void CopyFile(const std::string& sourcePath, const std::string& destinationPath);
std::string GetCurrentTime(std::chrono::high_resolution_clock::time_point current, double offset);

int main(int argc, char* argv[])
{
    Timer timer;

    bool usePreviousStates = false;
    if(argc>1)
    {
        if(argv[1]==std::string("y"))
        {
            usePreviousStates = true;
        }
    }

    try
    {
        std::string configFile = "config.txt";
        Config config(configFile);

        CopyFile(configFile, "data/lastConfig.txt");

        MonteCarlo(config, usePreviousStates);
    }
    catch(std::out_of_range& e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

void MonteCarlo(Config config, bool usePreviousStates)
{
    System system(config, usePreviousStates);

    int numIterations = config.GetNumIterations();

    std::string outputStatesFile = "data/outputStates.txt";
    ClearContents(outputStatesFile);
    std::vector<std::vector<double>> exportedStates;

    std::string outputIterationsFile = "data/iterations.txt";
    std::string outputEnergyFile = "data/energy.txt";
    std::string outputPressureFile = "data/pressure.txt";
    std::string outputSwapFile = "data/swapAcceptance.txt";
    std::string outputTranslationFile = "data/translationAcceptance.txt";
    ClearContents(outputIterationsFile);
    ClearContents(outputEnergyFile);
    ClearContents(outputPressureFile);
    ClearContents(outputSwapFile);
    ClearContents(outputTranslationFile);

    int prevAcceptedSwaps = 0;
    int prevAcceptedTranslations = 0;

    const int skipSamples = config.GetSkipSamples();

    // Record start time.
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::cout<<"Simulation started at "<<GetCurrentTime(start, 0)<<std::endl<<std::endl;

    int whichPrint = 0;
    for(int i=0; i<numIterations; ++i)
    {
        if(i%skipSamples==0)
        {
            exportedStates = system.GetStates();
            Export2D(exportedStates, outputStatesFile, i);

            ExportItem(i, outputIterationsFile);
            ExportItem(system.GetTotalEnergy(), outputEnergyFile);
            ExportItem(system.GetPressure(), outputPressureFile);

            // Export MC acceptance ratios.
            int numAcceptedSwaps = system.GetAcceptedSwaps();
            int numAcceptedTranslations = system.GetAcceptedTranslations();
            double swapRatio = (double) (numAcceptedSwaps-prevAcceptedSwaps)/skipSamples;
            double translationRatio = (double) (numAcceptedTranslations-prevAcceptedTranslations)/skipSamples;

            ExportItem(swapRatio, outputSwapFile);
            ExportItem(translationRatio, outputTranslationFile);

            prevAcceptedSwaps = numAcceptedSwaps;
            prevAcceptedTranslations = numAcceptedTranslations;
        }

        system.AttemptSwap();

        system.AttemptTranslation();

        // Printing progress and ETA.
        int numProgressUpdates = 10;
        if(i%((int) (numIterations-1)/numProgressUpdates)==0)
        {
            if(i!=0)
            {
                whichPrint++;
                auto current = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> timeSinceStart = current - start;
                int numProgressUpdatesToDo = numProgressUpdates - whichPrint;
                auto estimatedTimeOfCompletion = timeSinceStart/whichPrint*numProgressUpdatesToDo;

                int progress = 100*i/(numIterations-1);

                auto dateTime = GetCurrentTime(current, 0);

                double secondsLeft = estimatedTimeOfCompletion.count();

                if(secondsLeft > 3600 * numProgressUpdatesToDo)
                {
                    std::cout<<std::setw(3)<<progress<<"%"
                        <<"\t ETA: "<< std::setw(12)<<secondsLeft/3600<<" hours"
                        <<"\t at UTC "<<dateTime<<std::endl;
                }
                else if(secondsLeft > 60 * numProgressUpdatesToDo)
                {
                    std::cout<<std::setw(3)<<progress<<"%"
                        <<"\t ETA: "<< std::setw(12)<<secondsLeft/60<<" min"
                        <<"\t at UTC "<<dateTime<<std::endl;
                }
                else
                {
                    std::cout<<std::setw(3)<<progress<<"%"
                        <<" ETA: "<< std::setw(10)<<secondsLeft<<"s"
                        <<" at UTC "<<dateTime<<std::endl;
                }

                std::cout<<"Estimated time of completion "<<GetCurrentTime(current, secondsLeft)
                    <<std::endl<<std::endl;
            }
        }
    }
}

void CopyFile(const std::string& sourceFile, const std::string& destinationFile)
{
    std::ifstream inFile(sourceFile);
    std::ofstream outFile(destinationFile, std::ios_base::trunc);
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

std::string GetCurrentTime(std::chrono::high_resolution_clock::time_point current, double offset)
{
    std::time_t currentTime = std::chrono::system_clock::to_time_t(current) + offset;
    auto dateTimeInfo = std::ctime(&currentTime);
    std::string dateTimeInfoString(dateTimeInfo);
    auto dateTime = dateTimeInfoString.substr(4,15);
    return dateTime;
}
