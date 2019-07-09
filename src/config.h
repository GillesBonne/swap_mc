#ifndef  CONFIG_H
#define  CONFIG_H

#include <string>

class Config
{
private:
    // Member variables
    const std::string configFile;

    int numSpheres;

    double numDensity;

    double temperatureFixed;

    double maxTranslationDistanceInLengthUnits;

    double swapProbability;

    long long int numIterations;

    int numSpecies;
    int randomSeed;

public:
    // Constructor
    Config(const std::string _configFile);

    // Accessor functions
    int GetNumSpheres() const;

    double GetTemperatureFixed() const;

    double GetMaxTranslationDistanceInLengthUnits() const;

    double GetSwapProbability() const;

    long long int GetNumIterations() const;

    double GetNumDensity() const;

    int GetNumSpecies() const;
    int GetRandomSeed() const;
};

#endif
