// Place #define NDEBUG if asserts should not be evaluated.

#include <iostream>
#include <cmath>
#include <random>
#include <assert.h>
#include "system.h"

System::System(int _numSpheres,
    int _numSmallSpheres,
    int _numLargeSpheres,
    double _ratioSizeSphere,
    double _temperature,
    double _lengthBox)
    :   numSpheres(_numSpheres),
        numSmallSpheres(_numSmallSpheres),
        numLargeSpheres(_numLargeSpheres),
        ratioSizeSphere(_ratioSizeSphere),
        temperature(_temperature),
        lengthBox(_lengthBox),
        mersenneTwister((std::random_device())())
{
    const int latticeWidth = FindLatticeWidth();
    const double latticeParameter = lengthBox / latticeWidth;
    assert(std::pow(latticeWidth,3) >= numSpheres);

    maxRadiusSphere = latticeParameter/2;
    minRadiusSphere = maxRadiusSphere/ratioSizeSphere;

    // Binary mixture specific
    std::uniform_int_distribution<int> chooseRadius(0,1);

    // Possibly Binary mixture specific
    int numPlacedSmallSpheres = 0;
    int numPlacedLargeSpheres = 0;

    int numPlacedSpheres = 0;
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
                            sphere.radius = minRadiusSphere;
                            numPlacedSmallSpheres++;
                        }
                        else
                        {
                            sphere.radius = maxRadiusSphere;
                            numPlacedLargeSpheres++;
                        }
                    }
                    else if(randomRadius==1)
                    {
                        if(numPlacedLargeSpheres<numLargeSpheres)
                        {
                            sphere.radius = maxRadiusSphere;
                            numPlacedLargeSpheres++;
                        }
                        else
                        {
                            sphere.radius = minRadiusSphere;
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
        std::cout<<"(x,y,z)=("
            <<sphere.position.x<<","
            <<sphere.position.y<<","
            <<sphere.position.z<<")   "
            <<"r="<<sphere.radius
            <<std::endl;
    }
}

std::vector<std::vector<double>> System::GetStates() const
{
    std::vector<std::vector<double>> sphereStates;
    sphereStates.reserve(numSpheres);
    std::vector<double> sphereState(4);

    for(Sphere sphere : spheres)
    {
        sphereState[0] = sphere.position.x;
        sphereState[1] = sphere.position.y;
        sphereState[2] = sphere.position.z;
        sphereState[3] = sphere.radius;
        sphereStates.push_back(sphereState);
    }
    return sphereStates;
}
