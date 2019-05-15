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
    /* WCA & continuous polydisperse.
     * SRPP & continuous polydisperse & change additivity.
     * WCA & binary & 5050.
     * WCA & binary & 8020 & KobAnderson.
     */
    const bool toggleWCA = false;
    const bool toggleSRPP = true;

    const bool toggleContinuousPolydisperse = true;
    const bool toggleBinaryMixture = false;

    // If binary then size ratio large:small = 50:50 or 80:20
    const bool toggleRadius5050 = false;
    const bool toggleRadius8020 = false;

    // If WCA, binary, 8020, Kob Anderson interaction parameters vs general case.
    const bool toggleKobAnderson = false;

    // Scale the degree of additivity of the radii.
    const bool toggleChangeAdditivity = true;

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

    double ForceWCA(double difference, double sqDistance, double sigmaSummedRadius);
    double ForceSRPP(double difference, double sqDistance, double sigmaSummedRadius);
};

#endif

