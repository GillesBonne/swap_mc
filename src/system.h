#ifndef  SYSTEM_H
#define  SYSTEM_H

#include "sphere.h"

class System
{
private:
    // Member variables
    const int numSpheres;
    const int numSmallSpheres;
    const int numLargeSpheres;
    const double temperature;
    const double lengthBox;

    // Constructor
    System(int _numSpheres,
            int _numSmallSpheres,
            int _numLargeSpheres,
            double _temperature,
            double _lengthBox);
};

#endif

