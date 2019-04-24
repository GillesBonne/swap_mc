#include "system.h"

System::System(int _numSpheres,
    int _numSmallSpheres,
    int _numLargeSpheres,
    double _temperature,
    double _lengthBox)
    :   numSpheres(_numSpheres),
        numSmallSpheres(_numSmallSpheres),
        numLargeSpheres(_numLargeSpheres),
        temperature(_temperature),
        lengthBox(_lengthBox)
{
}
