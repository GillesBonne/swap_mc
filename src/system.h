#ifndef  SYSTEM_H
#define  SYSTEM_H

#include <vector>
#include <random>

#include "sphere.h"

class System
{
private:
    // Member variables
    const int numSpheres;
    const int numSmallSpheres;
    const int numLargeSpheres;
    const double ratioSizeSphere;
    const double temperatureFixed;
    const double lengthBox;
    const double swapProbability;
    const double epsilonConstant;

    std::vector<Sphere> spheres;

    double minRadiusSphere;
    double maxRadiusSphere;

    int attemptedSwap = 0;
    int rejectedSwap = 0;
    int attemptedTranslation = 0;
    int rejectedTranslation = 0;

    // Member variables: random number generators
    std::mt19937 mersenneTwister;

public:
    // Constructor
    System(int _numSpheres,
            int _numSmallSpheres,
            int _numLargeSpheres,
            double _ratioSizeSphere,
            double _temperatureFixed,
            double _lengthBox,
            double _swapProbability,
            double _epsilonConstant);

    int FindLatticeWidth() const;
    void PrintStates() const;
    std::vector<std::vector<double>> GetStates() const;

    void AttemptTranslation();
    void AttemptSwap();

    double CalculateEnergy(int index, Sphere sphere);

    double PotentialWCA(double sigmaSummedRadius, double distanceBetweenSpheres) const;

    double RadiusSumOf(Sphere sphere1, Sphere sphere2) const;
    double DistanceBetween(Sphere sphere1, Sphere sphere2);

    void CorrectForPeriodicDistance(double &length);
};

#endif

