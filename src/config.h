#ifndef  CONFIG_H
#define  CONFIG_H

#include <string>

class Config
{
private:
    // Member variables
    const std::string configFile;

    int numSpheres;
    double ratioSizeSphere;

    double numDensity;
    double volumeBox;
    double lengthBox;
    int latticeWidth;
    double latticeParameter;

    double temperatureFixed;

    double maxTranslationDistanceInMaxParticleSize;

    double swapProbability;
public:
    // Constructor
    Config(std::string _configFile);

    int FindLatticeWidth() const;

    // Accessor functions
    int GetNumSpheres() const;
    double GetRatioSizeSphere() const;

    double GetNumDensity() const;
    double GetLengthBox() const;
    int GetLatticeWidth() const;
    double GetLatticeParameter() const;

    double GetTemperatureFixed() const;

    double GetMaxTranslationDistanceInMaxParticleSize() const;

    double GetSwapProbability() const;
};

#endif
