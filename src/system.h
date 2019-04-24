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
    const double minSizeSphere;
    const double maxSizeSphere;
    const double temperature;
    const double lengthBox;
    std::vector<Sphere> spheres;

    // Member variables: random number generators
    std::mt19937 mersenneTwister;

public:
    // Constructor
    System(int _numSpheres,
            int _numSmallSpheres,
            int _numLargeSpheres,
            double _minSizeSphere,
            double _maxSizeSphere,
            double _temperature,
            double _lengthBox);

    int FindLatticeWidth() const;
    void PrintStates() const;
};

#endif

