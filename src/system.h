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
    const double temperature;
    const double lengthBox;

    std::vector<Sphere> spheres;

    double minRadiusSphere;
    double maxRadiusSphere;

    // Member variables: random number generators
    std::mt19937 mersenneTwister;

public:
    // Constructor
    System(int _numSpheres,
            int _numSmallSpheres,
            int _numLargeSpheres,
            double _ratioSizeSphere,
            double _temperature,
            double _lengthBox);

    int FindLatticeWidth() const;
    void PrintStates() const;
    std::vector<std::vector<double>> GetStates() const;
};

#endif

