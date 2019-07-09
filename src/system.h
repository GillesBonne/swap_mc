#ifndef  SYSTEM_H
#define  SYSTEM_H

#include <vector>
#include <random>
#include <string>

#include "sphere.h"
#include "config.h"

class System
{
private:
    // Member variables
    const int numSpheres;
    const double temperatureFixed;
    const double numDensity;
    const double volumeBox;
    const double lengthBox;

    double maxTranslationDistance;

    const double epsilonConstant = 1;
    const double boltzmannConstant = 1;

    std::vector<Sphere> spheres;

    int acceptedTranslations = 0;
    int acceptedSwaps = 0;

    // Member variables: random number generators
    std::mt19937 mersenneTwister;
    std::uniform_real_distribution<double> randomDouble;
    std::uniform_real_distribution<double> randomPosNegDouble;
    std::uniform_int_distribution<int> randomParticle;

public:
    // Constructor
    System(const Config& config, const bool usePreviousStates, std::string previousID);

    std::vector<std::vector<double>> GetStates() const;

    void AttemptTranslation();
    void AttemptSwap();

    int ChooseRandomParticle();

    double CalculateEnergy(const int index, const Sphere& sphere);

    double PotentialSRPP(const double sigmaSummedRadius, const double distanceBetweenSpheres) const;

    double RadiusSumOf(const Sphere& sphere1, const Sphere& sphere2) const;
    double DistanceBetween(const Sphere& sphere1, const Sphere& sphere2);

    void CorrectForPeriodicDistance(double& length) const;
    void CorrectForPeriodicSphere(Sphere& sphere);
    void CorrectForPeriodicCoordinate(double& coordinate) const;

    bool IsChosenWithProbability(const double probabilityReference);

    // Accessor functions
    int GetAcceptedTranslations() const;
    int GetAcceptedSwaps() const;

    // Analysis function
    double GetTotalEnergy();

    double GetPressure();

    double DistanceBetweenCoordinates(double coordinate1, double coordinate2);

    double ForceSRPP(double difference, double sqDistance, double sigmaSummedRadius);
};

#endif
