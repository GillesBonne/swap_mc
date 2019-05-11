// Place #define NDEBUG if asserts should not be evaluated.
#define NDEBUG

#include <iostream>
#include <cmath>
#include <random>
#include <cassert>
#include <algorithm>

#include "timer.h"

#include "system.h"
#include "config.h"

#include "../data_analysis/src/states.h"
#include "../data_analysis/src/states.cpp"

System::System(const Config& config, const bool usePreviousStates)
    :   numSpheres(config.GetNumSpheres()),
        spheres(numSpheres),
        ratioSizeSphere(config.GetRatioSizeSphere()),
        volumeBox(config.GetVolumeBox()),
        lengthBox(config.GetLengthBox()),
        latticeWidth(config.GetLatticeWidth()),
        latticeParameter(config.GetLatticeParameter()),
        temperatureFixed(config.GetTemperatureFixed()),
        swapProbability(config.GetSwapProbability()),
        maxTranslationDistanceInMaxParticleSize(config.GetMaxTranslationDistanceInMaxParticleSize()),
        numDensity(config.GetNumDensity()),
        mersenneTwister((std::random_device())()),
        randomDouble(0,1),
        randomPosNegDouble(-1,1),
        randomParticle(0,numSpheres-1)
{
    Timer timer;
    assert(numSpheres > 1);
    assert(std::pow(latticeWidth,3) >= numSpheres);

    maxRadiusSphere = latticeParameter/2;
    minRadiusSphere = maxRadiusSphere/ratioSizeSphere;

    // Max tranlationdistances should be such that translation acceptance is between 30-40%
    maxTranslationDistance = maxRadiusSphere*maxTranslationDistanceInMaxParticleSize;

    Sphere sphere;
    if(usePreviousStates)
    {
        std::cout<<"Use previous states"<<std::endl;
        States states("data/outputStates.txt", numSpheres);
        int maxSampleIndex = states.GetMaxSampleIndex();

        std::vector<std::vector<double>> spherePositions(numSpheres);
        spherePositions = states.GetSample(maxSampleIndex, true);

        for(int i=0; i<numSpheres; ++i)
        {
            sphere.position.x = spherePositions[i][0];
            sphere.position.y = spherePositions[i][1];
            sphere.position.z = spherePositions[i][2];
            sphere.radius = spherePositions[i][3];
            spheres[i] = sphere;
        }
    }
    else
    {
        std::cout<<"Do not use previous states"<<std::endl;
        int numPlacedSpheres = 0;
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

                        spheres[numPlacedSpheres] = sphere;
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

    // Toggle
    bool continuousPolydisperse = false;
    if(continuousPolydisperse)
    {
        std::uniform_real_distribution<double> randomDoubleMinMaxSize(minRadiusSphere,maxRadiusSphere);
        for(int i=0; i<numSpheres; ++i)
        {
            spheres[i].radius = randomDoubleMinMaxSize(mersenneTwister);
        }
    }
    else
    {
        // Toggle: if radius ratio is not equal then ratio large:small = 80:20
        bool radiusRatioEqual = true;
        if(radiusRatioEqual)
        {
            const int numSmallSpheres = 0.5 * numSpheres;
            const int numLargeSpheres = numSpheres - numSmallSpheres;

            int numPlacedSmallSpheres = 0;
            int numPlacedLargeSpheres = 0;
            int randomRadius;

            for(int i=0; i<numSpheres; ++i)
            {
                double randomRadius = randomDouble(mersenneTwister);
                if(randomRadius <= 0.5)
                {
                    if(numPlacedSmallSpheres<numSmallSpheres)
                    {
                        spheres[i].radius = minRadiusSphere;
                        ++numPlacedSmallSpheres;
                    }
                    else
                    {
                        spheres[i].radius = maxRadiusSphere;
                        ++numPlacedLargeSpheres;
                    }
                }
                else
                {
                    if(numPlacedLargeSpheres<numLargeSpheres)
                    {
                        spheres[i].radius = maxRadiusSphere;
                        ++numPlacedLargeSpheres;
                    }
                    else
                    {
                        spheres[i].radius = minRadiusSphere;
                        ++numPlacedSmallSpheres;
                    }
                }
            }
        }
        else
        {
            const int numSmallSpheres = 0.2 * numSpheres;
            const int numLargeSpheres = numSpheres - numSmallSpheres;

            int numPlacedSmallSpheres = 0;
            int numPlacedLargeSpheres = 0;
            int randomRadius;

            for(int i=0; i<numSpheres; ++i)
            {
                double randomRadius = randomDouble(mersenneTwister);
                if(randomRadius <= 0.2)
                {
                    if(numPlacedSmallSpheres<numSmallSpheres)
                    {
                        spheres[i].radius = minRadiusSphere;
                        ++numPlacedSmallSpheres;
                    }
                    else
                    {
                        spheres[i].radius = maxRadiusSphere;
                        ++numPlacedLargeSpheres;
                    }
                }
                else
                {
                    if(numPlacedLargeSpheres<numLargeSpheres)
                    {
                        spheres[i].radius = maxRadiusSphere;
                        ++numPlacedLargeSpheres;
                    }
                    else
                    {
                        spheres[i].radius = minRadiusSphere;
                        ++numPlacedSmallSpheres;
                    }
                }
            }
        }
    }
}

std::vector<std::vector<double>> System::GetStates() const
{
    std::vector<std::vector<double>> sphereStates(numSpheres);
    std::vector<double> sphereState(4);

    for(int i=0; i<numSpheres; ++i)
    {
        sphereState[0] = spheres[i].position.x;
        sphereState[1] = spheres[i].position.y;
        sphereState[2] = spheres[i].position.z;
        sphereState[3] = spheres[i].radius;
        sphereStates[i] = sphereState;
    }
    return sphereStates;
}

void System::AttemptTranslation()
{
    int randomParticleIndex = ChooseRandomParticle();
    double energy = CalculateEnergy(randomParticleIndex, spheres[randomParticleIndex]);

    Sphere newSphere = spheres[randomParticleIndex];
    newSphere.position.x += maxTranslationDistance*randomPosNegDouble(mersenneTwister);
    newSphere.position.y += maxTranslationDistance*randomPosNegDouble(mersenneTwister);
    newSphere.position.z += maxTranslationDistance*randomPosNegDouble(mersenneTwister);

    double energyNew = CalculateEnergy(randomParticleIndex, newSphere);
    double energyDifference = energyNew - energy;

    double acceptProbability = std::min(1.0,
                    std::exp(-energyDifference/(boltzmannConstant * temperatureFixed)));

    if(IsChosenWithProbability(acceptProbability))
    {
        CorrectForPeriodicSphere(newSphere);
        spheres[randomParticleIndex] = newSphere;
        ++acceptedTranslations;
    }
}

void System::AttemptSwap()
{
    if(IsChosenWithProbability(swapProbability))
    {
        // Choose two DIFFERENT particles randomly
        int randomParticleIndex1 = ChooseRandomParticle();
        int randomParticleIndex2;
        do
        {
            randomParticleIndex2 = ChooseRandomParticle();
        }
        while(randomParticleIndex1 == randomParticleIndex2);

        double energy = CalculateEnergy(randomParticleIndex1, spheres[randomParticleIndex1])
                            +CalculateEnergy(randomParticleIndex2, spheres[randomParticleIndex2]);

        Sphere newSphere1 = spheres[randomParticleIndex1];
        newSphere1.radius = spheres[randomParticleIndex2].radius;

        Sphere newSphere2 = spheres[randomParticleIndex2];
        newSphere2.radius = spheres[randomParticleIndex1].radius;

        double energyNew = CalculateEnergy(randomParticleIndex1, newSphere1)
                            +CalculateEnergy(randomParticleIndex2, newSphere2);

        double energyDifference = energyNew - energy;

        double acceptProbability = std::min(1.0,
                        std::exp(-energyDifference/(boltzmannConstant * temperatureFixed)));

        if(IsChosenWithProbability(acceptProbability))
        {
            spheres[randomParticleIndex1] = newSphere1;
            spheres[randomParticleIndex2] = newSphere2;

            ++acceptedSwaps;
        }
    }
}

int System::ChooseRandomParticle()
{
    int randomParticleIndex = randomParticle(mersenneTwister);
    return randomParticleIndex;
}

double System::CalculateEnergy(const int index, const Sphere& sphere)
{
    double energy = 0;
    for(int i=0; i<numSpheres; ++i)
    {
        if(i!=index)
        {
            // Toggle
            bool WCA = true;
            if(WCA)
            {
                energy += PotentialWCA(RadiusSumOf(sphere,spheres[i]),
                    DistanceBetween(sphere,spheres[i]));
            }
            else
            {
                energy += PotentialLennardJones(RadiusSumOf(sphere,spheres[i]),
                    DistanceBetween(sphere,spheres[i]));
            }
        }
    }
    return energy;
}

double System::PotentialWCA(const double sigmaSummedRadius, const double distanceBetweenSpheres) const
{
    double potential;
    const double cutOffDistance = 1.12246204831 * sigmaSummedRadius;
    if(distanceBetweenSpheres > cutOffDistance)
    {
        potential = 0;
    }
    else
    {
        potential = epsilonConstant + 4 * epsilonConstant * (- pow((sigmaSummedRadius/distanceBetweenSpheres),6)
                                        + pow((sigmaSummedRadius/distanceBetweenSpheres),12));
    }
    return potential;
}

double System::PotentialLennardJones(const double sigmaSummedRadius, const double distanceBetweenSpheres) const
{
    double potential;
    const double cutOffDistance = 2.5 * sigmaSummedRadius;
    const double shift = 0.00407922;
    if(distanceBetweenSpheres > cutOffDistance)
    {
        potential = 0;
    }
    else
    {
        // Toggle
        bool kobAndersonParameters = false;
        double epsilonValue;
        double sigma;
        if(kobAndersonParameters)
        {
            if(sigmaSummedRadius == 2 * maxRadiusSphere)
            {
                epsilonValue = 1.0;
                sigma = 1.0;
            }
            else if(sigmaSummedRadius == 2 * minRadiusSphere)
            {
                epsilonValue = 0.5;
                sigma = 0.88;
            }
            else if(sigmaSummedRadius == (maxRadiusSphere + minRadiusSphere))
            {
                epsilonValue = 1.5;
                sigma = 0.8;
            }
            else
            {
                throw std::out_of_range("Kob Anderson: radii do not match");
            }
        }
        else
        {
            epsilonValue = epsilonConstant;
            sigma = sigmaSummedRadius;
        }
        potential = 4 * epsilonValue * (shift - pow((sigma/distanceBetweenSpheres),6)
                                        + pow((sigma/distanceBetweenSpheres),12));
    }
    return potential;
}

double System::RadiusSumOf(const Sphere& sphere1, const Sphere& sphere2) const
{
    return sphere1.radius + sphere2.radius;
}

double System::DistanceBetween(const Sphere& sphere1, const Sphere& sphere2)
{
    double diffX = sphere1.position.x - sphere2.position.x;
    double diffY = sphere1.position.y - sphere2.position.y;
    double diffZ = sphere1.position.z - sphere2.position.z;

    CorrectForPeriodicDistance(diffX);
    CorrectForPeriodicDistance(diffY);
    CorrectForPeriodicDistance(diffZ);

    return sqrt(diffX*diffX + diffY*diffY + diffZ*diffZ);
}

void System::CorrectForPeriodicDistance(double& length) const
{
    bool negativeOutsideBoundary = (length < -0.5*lengthBox);
    bool positiveOutsideBoundary = (length > 0.5*lengthBox);
    if(positiveOutsideBoundary)
    {
        length -= lengthBox;
    }
    else if(negativeOutsideBoundary)
    {
        length += lengthBox;
    }
    assert(!(length < -0.5*lengthBox || length > 0.5*lengthBox));
}

void System::CorrectForPeriodicSphere(Sphere& sphere)
{
    CorrectForPeriodicCoordinate(sphere.position.x);
    CorrectForPeriodicCoordinate(sphere.position.y);
    CorrectForPeriodicCoordinate(sphere.position.z);
}

void System::CorrectForPeriodicCoordinate(double& coordinate) const
{
    while(coordinate < 0.0)
    {
        coordinate += lengthBox;
    }
    while(coordinate >= lengthBox)
    {
        coordinate -= lengthBox;
    }
}

bool System::IsChosenWithProbability(const double probabilityReference)
{
    double probabilityRandom = randomDouble(mersenneTwister);

    bool isChosen;
    if(probabilityRandom<=probabilityReference)
    {
        isChosen = true;
    }
    else
    {
        isChosen = false;
    }
    return isChosen;
}

int System::GetAcceptedTranslations() const
{
    return acceptedTranslations;
}
int System::GetAcceptedSwaps() const
{
    return acceptedSwaps;
}

double System::GetTotalEnergy()
{
    double energy = 0;
    for(int i=0; i < (numSpheres-1); ++i)
    {
        for(int j=i+1; j < numSpheres; ++j)
        {
            energy += PotentialWCA(RadiusSumOf(spheres[i],spheres[j]),
                    DistanceBetween(spheres[i],spheres[j]));
        }
    }
    return energy;
}

double System::GetPressure()
{
    const int dimensionality = 3;
    double pressure = 0;

    double xDiff;
    double yDiff;
    double zDiff;
    double sigmaSummedRadius;
    double xForce;
    double yForce;
    double zForce;
    double distanceSquaredBetween;
    for(int i=0; i < (numSpheres-1); ++i)
    {
        for(int j=i+1; j < numSpheres; ++j)
        {
            xDiff = DistanceBetweenCoordinates(spheres[i].position.x,
                                                 spheres[j].position.x);
            yDiff = DistanceBetweenCoordinates(spheres[i].position.y,
                                                 spheres[j].position.y);
            zDiff = DistanceBetweenCoordinates(spheres[i].position.z,
                                                 spheres[j].position.z);
            distanceSquaredBetween = xDiff*xDiff+yDiff*yDiff+zDiff*zDiff;
            sigmaSummedRadius = RadiusSumOf(spheres[i],spheres[j]);
            xForce = ForceWCA(xDiff, distanceSquaredBetween, sigmaSummedRadius);
            yForce = ForceWCA(yDiff, distanceSquaredBetween, sigmaSummedRadius);
            zForce = ForceWCA(zDiff, distanceSquaredBetween, sigmaSummedRadius);

            pressure += xDiff*xForce + yDiff*yForce + zDiff*zForce;
        }
    }
    pressure /= (dimensionality * volumeBox);
    pressure += numDensity * temperatureFixed * boltzmannConstant;

    return pressure;
}

double System::DistanceBetweenCoordinates(double coordinate1, double coordinate2)
{
    double difference = coordinate2 - coordinate1;
    CorrectForPeriodicDistance(difference);
    return difference;
}

double System::ForceWCA(double difference, double SqDistance, double sigmaSummedRadius)
{
    double force;
    const double cutOffDistance = 1.12246204831 * sigmaSummedRadius;
    if(SqDistance > (cutOffDistance*cutOffDistance))
    {
        force = 0;
    }
    else
    {
        force = 24 * epsilonConstant * difference * pow(sigmaSummedRadius, 6)
            * (2*pow(sigmaSummedRadius, 6)-SqDistance*SqDistance*SqDistance)
            / (pow(SqDistance,7));
    }
    return force;
}
