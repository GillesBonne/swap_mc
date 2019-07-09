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
void CopyFile(const std::string& sourcePath, const std::string& destinationPath);
std::string GetCurrentTime(std::chrono::high_resolution_clock::time_point current,
        double offset);
void CheckFileExistence(std::string fileName);

// Argument options:
// 0
// 1: SimulationID
// 2: SimulationID PreviousSimulationID
int main(int argc, char* argv[])
{
    Timer timer;

    try
    {
        bool usePreviousStates = false;
        std::string simulationID;
        std::string previousID;
        int sampleBegin;
        if(argc==1)
        {
            std::cout<<"Overwriting data files without ID."<<std::endl;
            sampleBegin = 0;
        }
        else if(argc==2)
        {
            std::cout<<"Overwriting data files without ID."<<std::endl;
            sampleBegin = std::stoi(argv[1]);
        }
        else if(argc==3)
        {
            simulationID = argv[2];
            std::cout<<"Simulation ID: "<<simulationID<<std::endl;
            sampleBegin = std::stoi(argv[1]);
        }
        else if(argc==4)
        {
            simulationID = argv[2];
            std::cout<<"Simulation ID: "<<simulationID<<std::endl;
            previousID = argv[3];
            std::cout<<"Previous ID: "<<previousID<<std::endl;
            if(simulationID == previousID)
            {
                throw std::invalid_argument("Current and previous ID are equal.");
            }

            std::string previousConfigFile = "data/data" + previousID + "/lastConfig.txt";
            CheckFileExistence(previousConfigFile);

            usePreviousStates = true;

            sampleBegin = std::stoi(argv[1]);
        }
        else
        {
            throw std::out_of_range("Wrong number of arguments.");
        }


        std::string configFile = "config" + simulationID + ".txt";
        CheckFileExistence(configFile);
        Config config(configFile);

        std::cout<<"\nPosition sampling starts at iteration: "<<sampleBegin<<std::endl;

        std::string data_command = "[ -d data ] || mkdir data";
        std::string command = "mkdir data/data" + simulationID;
        // To suppress the compiler warning of not using the return value.
        int _;
        _ = std::system(data_command.c_str());
        _ = std::system(command.c_str());

        std::string copyConfigFile = "data/data" + simulationID + "/lastConfig.txt";
        CopyFile(configFile, copyConfigFile);

        MonteCarlo(config, usePreviousStates, simulationID, previousID, sampleBegin);
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
        std::string simulationID, std::string previousID,
        int sampleBegin)
{
    System system(config, usePreviousStates, previousID);

    const long long int numIterations = config.GetNumIterations();
//    const int skipSamples = config.GetSkipSamples();
    const double swapProbability = config.GetSwapProbability();

    std::string outputIterationsFile = "data/data" + simulationID + "/iterations.txt";
    std::string outputEnergyFile ="data/data" + simulationID + "/energy.txt";
    std::string outputSwapFile = "data/data" + simulationID + "/swapAcceptance.txt";
    std::string outputTranslationFile = "data/data" + simulationID + "/translationAcceptance.txt";
    ClearContents(outputIterationsFile);
    ClearContents(outputEnergyFile);
    ClearContents(outputSwapFile);
    ClearContents(outputTranslationFile);

    std::string outputStatesFile10000 = "data/data" + simulationID + "/outputStates10000.txt";
    std::string outputStatesFile100000 = "data/data" + simulationID + "/outputStates100000.txt";
    std::string outputStatesFile1000000 = "data/data" + simulationID + "/outputStates1000000.txt";
    std::string outputStatesFile10000000 = "data/data" + simulationID + "/outputStates10000000.txt";
    ClearContents(outputStatesFile10000);
    ClearContents(outputStatesFile100000);
    ClearContents(outputStatesFile1000000);
    ClearContents(outputStatesFile10000000);

    std::vector<std::vector<double>> exportedStates;

    // Record start time.
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::cout<<"Simulation started at "<<GetCurrentTime(start, 0)<<std::endl<<std::endl;

    int attemptedSwaps = 0;
    int attemptedTranslations = 0;
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

void CheckFileExistence(std::string fileName)
{
    std::ifstream file(fileName);
    if(!file)
    {
        std::string message = fileName + " does not exist.";
        throw std::invalid_argument(message);
    }
}
