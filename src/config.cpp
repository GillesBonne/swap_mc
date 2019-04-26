#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>
#include "config.h"

Config::Config(std::string _configFile) : configFile(_configFile)
{
    std::ifstream inFile(configFile);

    std::string numSpheresString;
    std::string ratioSizeSphereString;

    std::string numDensityString;

    std::string temperatureFixedString;

    std::string maxTranslationDistanceInMaxParticleSizeString;

    std::string swapProbabilityString;

    // Read contents of config file and assigns it to the corresponding member variables.
    if (inFile.is_open())
    {
        std::string line;
        while(getline(inFile, line)){
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
            else if(name=="ratioSizeSphere")
            {
                ratioSizeSphereString = value;
            }
            else if(name=="numDensity")
            {
                numDensityString = value;
            }
            else if(name=="temperatureFixed")
            {
                temperatureFixedString = value;
            }
            else if(name=="maxTranslationDistanceInMaxParticleSize")
            {
                maxTranslationDistanceInMaxParticleSizeString = value;
            }
            else if(name=="swapProbability")
            {
                swapProbabilityString = value;
            }
        }
    }
    numSpheres = std::stoi(numSpheresString);
    ratioSizeSphere = std::stod(ratioSizeSphereString);
    numDensity = std::stod(numDensityString);
    temperatureFixed = std::stod(temperatureFixedString);
    maxTranslationDistanceInMaxParticleSize = std::stod(maxTranslationDistanceInMaxParticleSizeString);
    swapProbability = std::stod(swapProbabilityString);

    volumeBox = numSpheres/numDensity;
    lengthBox = cbrt(volumeBox);

    latticeWidth = FindLatticeWidth();
    latticeParameter = lengthBox / latticeWidth;
}

int Config::FindLatticeWidth() const
{
    for(int i=2; i<=numSpheres; ++i)
    {
        if(i*i*i >= numSpheres)
        {
            return i;
        }
    }
}

int Config::GetNumSpheres() const
{
    return numSpheres;
}
double Config::GetRatioSizeSphere() const
{
    return ratioSizeSphere;
}
double Config::GetNumDensity() const
{
    return numDensity;
}
double Config::GetLengthBox() const
{
    return lengthBox;
}
int Config::GetLatticeWidth() const
{
    return latticeWidth;
}
double Config::GetLatticeParameter() const
{
    return latticeParameter;
}
double Config::GetTemperatureFixed() const
{
    return temperatureFixed;
}
double Config::GetMaxTranslationDistanceInMaxParticleSize() const
{
    return maxTranslationDistanceInMaxParticleSize;
}
double Config::GetSwapProbability() const
{
    return swapProbability;
}
