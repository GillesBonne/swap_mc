#ifndef  SYSTEM_H
#define  SYSTEM_H

#include <vector>
#include <random>

#include "sphere.h"
#include "config.h"

class System
{
private:
    // Member variables
    const int numSpheres;
    const double ratioSizeSphere;

    const double volumeBox;
    const double lengthBox;
    const int latticeWidth;
    const double latticeParameter;

    const double temperatureFixed;

    const double swapProbability;
    const double maxTranslationDistanceInMaxParticleSize;
    double maxRadiusSphere;
    double maxTranslationDistance;

    const double numDensity;

    const double epsilonConstant = 1;
    const double boltzmannConstant = 1;

    std::vector<Sphere> spheres;

    int acceptedTranslations = 0;
    int acceptedSwaps = 0;
    int rejectedSwaps = 0;

    // Member variables: random number generators
    std::mt19937 mersenneTwister;
    std::uniform_real_distribution<double> randomDouble;
    std::uniform_real_distribution<double> randomPosNegDouble;
    std::uniform_int_distribution<int> randomParticle;

public:
    // Constructor
    System(const Config& config);

    int FindLatticeWidth() const;
    void PrintStates() const;
    std::vector<std::vector<double>> GetStates() const;

    void AttemptTranslation();
    void AttemptSwap();

    int ChooseRandomParticle();

    double CalculateEnergy(const int index, const Sphere& sphere);

    double PotentialWCA(const double sigmaSummedRadius, const double distanceBetweenSpheres) const;

    double RadiusSumOf(const Sphere& sphere1, const Sphere& sphere2) const;
    double DistanceBetween(const Sphere& sphere1, const Sphere& sphere2);

    void CorrectForPeriodicDistance(double& length) const;
    void CorrectForPeriodicSphere(Sphere& sphere);
    void CorrectForPeriodicCoordinate(double& coordinate) const;

    bool IsChosenWithProbability(const double probabilityReference);

    // Accessor functions
    int GetAcceptedTranslations() const;
    int GetAcceptedSwaps() const;
    int GetRejectedSwaps() const;

    double GetTotalEnergy();

    double GetPressure();

    double DistanceBetweenCoordinates(double coordinate1, double coordinate2);

    double ForceWCA(double difference, double sigmaSummedRadius);
};

#endif

