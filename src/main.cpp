#define NDEBUG

#include <iostream>
#include <iomanip>
#include <cassert>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <ctime>

#include "system.h"
#include "config.h"
#include "timer.h"
#include "export2D.h"

void MonteCarlo(Config config, bool usePreviousStates,
        std::string simulationID, std::string previousID,
        int sampleBegin);
void PrintAcceptanceInfo(const System& system, int numIterations);
std::string GetCurrentTime(std::chrono::high_resolution_clock::time_point current,
        double offset);
void CheckFileExistence(std::string fileName);

// Argument options:
// 2: sample delay & path
// 3: sample delay & path & previous path
int main(int argc, char* argv[])
{
    Timer timer;

    try
    {
        bool usePreviousStates = false;

        std::string previousPath;
        std::string currentPath;
        long long int sampleBegin;
        if(argc==3)
        {
            currentPath = argv[2];
            std::cout<<"Current path: "<<currentPath<<std::endl;
            sampleBegin = std::stoll(argv[1]);
        }
        else if(argc==4)
        {
            currentPath = argv[2];
            std::cout<<"Current path: "<<currentPath<<std::endl;
            previousPath = argv[3];
            std::cout<<"Previous path: "<<previousPath<<std::endl;

            std::string previousConfigFile = previousPath + "/config.txt";
            CheckFileExistence(previousConfigFile);

            usePreviousStates = true;

            sampleBegin = std::stoll(argv[1]);
        }
        else
        {
            throw std::out_of_range("Wrong number of arguments.");
        }


        std::string configFile = currentPath + "/config.txt";
        CheckFileExistence(configFile);
        Config config(configFile);

        std::cout<<"\nPosition sampling starts at iteration: "<<sampleBegin<<std::endl;

        MonteCarlo(config, usePreviousStates, currentPath, previousPath, sampleBegin);
    }
    catch(std::out_of_range& e)
    {
        std::cout<<e.what()<<std::endl;
    }
    catch(std::invalid_argument& e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

void MonteCarlo(Config config, bool usePreviousStates,
        std::string currentPath, std::string previousPath,
        int sampleBegin)
{
    System system(config, usePreviousStates, previousPath);

    const long long int numIterations = config.GetNumIterations();
    const double swapProbability = config.GetSwapProbability();

    std::string outputIterationsFile = currentPath + "/iterations.txt";
    std::string outputEnergyFile = currentPath + "/energy.txt";
    std::string outputSwapFile = currentPath + "/swapAcceptance.txt";
    std::string outputTranslationFile = currentPath + "/translationAcceptance.txt";
    ClearContents(outputIterationsFile);
    ClearContents(outputEnergyFile);
    ClearContents(outputSwapFile);
    ClearContents(outputTranslationFile);

    std::string outputStatesFile10000 = currentPath + "/outputStates10000.txt";
    std::string outputStatesFile100000 = currentPath + "/outputStates100000.txt";
    std::string outputStatesFile1000000 = currentPath + "/outputStates1000000.txt";
    std::string outputStatesFile10000000 = currentPath + "/outputStates10000000.txt";
    std::string lastStateFile = currentPath + "/lastState.txt";
    ClearContents(outputStatesFile10000);
    ClearContents(outputStatesFile100000);
    ClearContents(outputStatesFile1000000);
    ClearContents(outputStatesFile10000000);
    ClearContents(lastStateFile);

    std::vector<std::vector<double>> exportedStates;

    // Record start time.
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::cout<<"Simulation started at "<<GetCurrentTime(start, 0)<<std::endl<<std::endl;

    long long int attemptedSwaps = 0;
    long long int attemptedTranslations = 0;
    int whichPrint = 0;
    long long int logScaler = 10;
    for(long long int it=0; it<numIterations; ++it)
    {
        if((it >= sampleBegin) && (numIterations>=10000) && (it%10000==0) && (it<=10000000))
        {
            exportedStates = system.GetStates();
            Export2D(exportedStates, outputStatesFile10000, it);
        }
        if((it >= sampleBegin) && (numIterations>=100000) && (it%100000==0) && (it<=100000000))
        {
            exportedStates = system.GetStates();
            Export2D(exportedStates, outputStatesFile100000, it);
        }
        if((it >= sampleBegin) && (numIterations>=1000000) && (it%1000000==0) && (it<=1000000000))
        {
            exportedStates = system.GetStates();
            Export2D(exportedStates, outputStatesFile1000000, it);
        }
        if((it >= sampleBegin) && (numIterations>=10000000) && (it%10000000==0) && (it<=10000000000))
        {
            exportedStates = system.GetStates();
            Export2D(exportedStates, outputStatesFile10000000, it);
        }
        if(it==(numIterations-1))
        {
            exportedStates = system.GetStates();
            Export2D(exportedStates, lastStateFile, it);
        }

        if(it%((long long int) logScaler/10)==0)
        {
            ExportItem(it, outputIterationsFile);
            ExportItem(system.GetTotalEnergy(), outputEnergyFile);

            // Export MC acceptance ratios.
            double swapRatio = (double) system.GetAcceptedSwaps()/attemptedSwaps;
            double translationRatio = (double) system.GetAcceptedTranslations()/attemptedTranslations;

            ExportItem(swapRatio, outputSwapFile);
            ExportItem(translationRatio, outputTranslationFile);
        }
        if(it==logScaler)
        {
            logScaler *= 10;
        }

        if(system.IsChosenWithProbability(swapProbability))
        {
            system.AttemptSwap();
            ++attemptedSwaps;
        }
        else
        {
            system.AttemptTranslation();
            ++attemptedTranslations;
        }

        // Printing progress and ETA.
        int numProgressUpdates = 10;
        if(it%((long long int) (numIterations-1)/numProgressUpdates)==0)
        {
            if(it!=0)
            {
                whichPrint++;
                auto current = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> timeSinceStart = current - start;
                int numProgressUpdatesToDo = numProgressUpdates - whichPrint;
                auto estimatedTimeOfCompletion = timeSinceStart/whichPrint*numProgressUpdatesToDo;

                long long int progress = (long long int) 100*it/(numIterations-1);

                auto dateTime = GetCurrentTime(current, 0);

                double secondsLeft = estimatedTimeOfCompletion.count();

                std::cout<<std::setw(3)<<progress<<"%"
                    << std::setw(15)<<secondsLeft/3600<<"h"
                    <<" at "<<dateTime<<std::endl;
                std::cout<<std::setw(24)<<"ETA: "<<GetCurrentTime(current, secondsLeft)
                    <<std::endl;
            }
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

void CheckFileExistence(std::string fileName)
{
    std::ifstream file(fileName);
    if(!file)
    {
        std::string message = fileName + " does not exist.";
        throw std::invalid_argument(message);
    }
}
