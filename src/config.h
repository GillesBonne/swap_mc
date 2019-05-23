#ifndef  CONFIG_H
#define  CONFIG_H

#include <string>

class Config
{
private:
    // Member variables
    const std::string configFile;

    int numSpheres;
//    double ratioSizeSphere;

    double numDensity;
//    double volumeBox;
//    double lengthBox;
//    int latticeWidth;
//    double latticeParameter;

    double temperatureFixed;

    double maxTranslationDistanceInLengthUnits;

    double swapProbability;

    int numIterations;
    int skipSamples;
public:
    // Constructor
    Config(const std::string _configFile);

//    int FindLatticeWidth() const;

    // Accessor functions
    int GetNumSpheres() const;
    double GetRatioSizeSphere() const;

//    double GetVolumeBox() const;
//    double GetLengthBox() const;
//    int GetLatticeWidth() const;
//    double GetLatticeParameter() const;

    double GetTemperatureFixed() const;

    double GetMaxTranslationDistanceInLengthUnits() const;

    double GetSwapProbability() const;

    int GetNumIterations() const;

    double GetNumDensity() const;

    int GetSkipSamples() const;
};

#endif
