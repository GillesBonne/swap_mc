// Place #define NDEBUG if asserts should not be evaluated.

#include <iostream>
#include <cmath>
#include <random>
#include <assert.h>
#include "system.h"

System::System(int _numSpheres,
    int _numSmallSpheres,
    int _numLargeSpheres,
    double _minSizeSphere,
    double _maxSizeSphere,
    double _temperature,
    double _lengthBox)
    :   numSpheres(_numSpheres),
        numSmallSpheres(_numSmallSpheres),
        numLargeSpheres(_numLargeSpheres),
        minSizeSphere(_minSizeSphere),
        maxSizeSphere(_maxSizeSphere),
        temperature(_temperature),
        lengthBox(_lengthBox),
        mersenneTwister((std::random_device())())
{
    const int latticeWidth = FindLatticeWidth();
    const double latticeParameter = lengthBox / latticeWidth;
    assert(std::pow(latticeWidth,3) >= numSpheres);

    // Binary mixture specific
    std::uniform_int_distribution<int> chooseRadius(0,1);

    // Continuous: using random number generation or linear spaced.
    // std::uniform_real_distribution<double> chooseRadius(minSizeSphere, maxSizeSphere);

    int numPlacedSpheres = 0;

    // Possibly Binary mixture specific
    int numPlacedSmallSpheres = 0;
    int numPlacedLargeSpheres = 0;

    Sphere sphere;
    int randomRadius;
    for(int i=0; i<latticeWidth; ++i)
    {
        for(int j=0; j<latticeWidth; ++j)
        {
            for(int k=0; k<latticeWidth; ++k)
            {
                if(numPlacedSpheres<numSpheres)
                {
                    sphere.position.x = (i + 0.5)*latticeParameter;
                    sphere.position.y = (j + 0.5)*latticeParameter;
                    sphere.position.z = (k + 0.5)*latticeParameter;

                    // Binary mixture specific
                    randomRadius = chooseRadius(mersenneTwister);
                    if(randomRadius==0)
                    {
                        if(numPlacedSmallSpheres<numSmallSpheres)
                        {
                            sphere.radius = minSizeSphere;
                            numPlacedSmallSpheres++;
                        }
                        else
                        {
                            sphere.radius = maxSizeSphere;
                            numPlacedLargeSpheres++;
                        }
                    }
                    else if(randomRadius==1)
                    {
                        if(numPlacedLargeSpheres<numLargeSpheres)
                        {
                            sphere.radius = maxSizeSphere;
                            numPlacedLargeSpheres++;
                        }
                        else
                        {
                            sphere.radius = minSizeSphere;
                            numPlacedSmallSpheres++;
                        }
                    }
                    spheres.push_back(sphere);
                }
                else
                {
                    break;
                }
                ++numPlacedSpheres;
            }
        }
    }

}
int System::FindLatticeWidth() const
{
    for(int i=2; i<=numSpheres; ++i)
    {
        if(i*i*i >= numSpheres)
        {
            return i;
        }
    }
}

void System::PrintStates() const
{
    for(Sphere sphere : spheres)
    {
        std::cout<<"(x,y,z)=("<<sphere.position.x<<","
            <<sphere.position.y<<","<<sphere.position.z<<")   "
            <<"r="<<sphere.radius
            <<std::endl;
    }
}
