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
    const double ratioSizeSphere;

    const double volumeBox;
    const double lengthBox;
    const int latticeWidth;
    const double latticeParameter;

    const double temperatureFixed;

    const double swapProbability;
    const double maxTranslationDistanceInMaxParticleSize;
    double maxRadiusSphere;
    double minRadiusSphere;
    double maxTranslationDistance;

    const double numDensity;

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

    //// Toggles ////
    // Possible combination.
    /* Continuous polydisperse & WCA.
     * Continuous polydisperse & LJ & Additivity.
     * Radius ratio equal (5050) & WCA.
     * Radius ratio not eq(8020) & WCA & KobAnderson.
     */
    // Continuous polydisperse vs binary mixture.
    bool toggleContinuousPolydisperse = true;
    // Radius large:small = 50:50 vs 80:20.
    bool toggleRadiusRatioEqual = true;
    // WCA vs LJ.
    bool toggleWCA = false;
    // Kob Anderson interaction parameters vs general case.
    bool toggleKobAnderson = false;
    // Scale the degree of additivity of the radii.
    bool toggleAdditivity = true;

public:
    // Constructor
    System(const Config& config, const bool usePreviousStates, std::string previousID);

    int FindLatticeWidth() const;
    void PrintStates() const;
    std::vector<std::vector<double>> GetStates() const;

    void AttemptTranslation();
    void AttemptSwap();

    int ChooseRandomParticle();

    double CalculateEnergy(const int index, const Sphere& sphere);

    double PotentialWCA(const double sigmaSummedRadius, const double distanceBetweenSpheres) const;
    double PotentialLJ(const double sigmaSummedRadius, const double distanceBetweenSpheres) const;

    double RadiusSumOf(const Sphere& sphere1, const Sphere& sphere2) const;
    double DistanceBetween(const Sphere& sphere1, const Sphere& sphere2);

    void CorrectForPeriodicDistance(double& length) const;
    void CorrectForPeriodicSphere(Sphere& sphere);
    void CorrectForPeriodicCoordinate(double& coordinate) const;

    bool IsChosenWithProbability(const double probabilityReference);

    // Accessor functions
    int GetAcceptedTranslations() const;
    int GetAcceptedSwaps() const;

    double GetTotalEnergy();

    double GetPressure();

    double DistanceBetweenCoordinates(double coordinate1, double coordinate2);

    double ForceWCA(double difference, double distanceBetween, double sigmaSummedRadius);
    double ForceLJ(double difference, double distanceBetween, double sigmaSummedRadius);
};

#endif

