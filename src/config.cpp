#define NDEBUG

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include "config.h"

Config::Config(const std::string _configFile) : configFile(_configFile)
{
    std::ifstream inFile(configFile);

    std::string numSpheresString;
    std::string ratioSizeSphereString;

    std::string numDensityString;

    std::string temperatureFixedString;

    std::string maxTranslationDistanceInLengthUnitsString;

    std::string swapProbabilityString;

    std::string numIterationsString;
    std::string skipSamplesString;

    // Read contents of config file and assigns it to the corresponding member variables.
    if (inFile.is_open())
    {
        std::string line;
        while(getline(inFile, line))
        {
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            if(name=="numSpheres")
            {
                numSpheresString = value;
            }
            else if(name=="numDensity")
            {
                numDensityString = value;
            }
            else if(name=="temperatureFixed")
            {
                temperatureFixedString = value;
            }
            else if(name=="maxTranslationDistanceInLengthUnits")
            {
                maxTranslationDistanceInLengthUnitsString = value;
            }
            else if(name=="swapProbability")
            {
                swapProbabilityString = value;
            }
            else if(name=="numIterations")
            {
                numIterationsString = value;
            }
            else if(name=="skipSamples")
            {
                skipSamplesString = value;
            }
        }
    }
    numSpheres = std::stoi(numSpheresString);
    if(numSpheres < 8)
    {
        throw std::out_of_range("Not enough spheres.");
    }

    bool isPowerOfThree = false;
    for(int i=2; i<=numSpheres; ++i)
    {
        if(i*i*i == numSpheres)
        {
            isPowerOfThree = true;
        }
    }
    if(!isPowerOfThree)
    {
        throw std::out_of_range("Number of spheres is not a power of 3.");
    }

    numDensity = std::stod(numDensityString);
    temperatureFixed = std::stod(temperatureFixedString);
    maxTranslationDistanceInLengthUnits = std::stod(maxTranslationDistanceInLengthUnitsString);
    swapProbability = std::stod(swapProbabilityString);
    numIterations = std::stoll(numIterationsString);
    skipSamples = std::stoi(skipSamplesString);
}

int Config::GetNumSpheres() const
{
    return numSpheres;
}
double Config::GetTemperatureFixed() const
{
    return temperatureFixed;
}
double Config::GetMaxTranslationDistanceInLengthUnits() const
{
    return maxTranslationDistanceInLengthUnits;
}
double Config::GetSwapProbability() const
{
    return swapProbability;
}
long long int Config::GetNumIterations() const
{
    return numIterations;
}
double Config::GetNumDensity() const
{
    return numDensity;
}
int Config::GetSkipSamples() const
{
    return skipSamples;
}
