// Place #define NDEBUG if asserts should not be evaluated.

#include <iostream>
#include <cmath>
#include <random>
#include <cassert>
#include <algorithm>

#include "system.h"
#include "config.h"

System::System(const Config& config)
    :   numSpheres(config.GetNumSpheres()),
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
    assert(numSpheres > 1);
    assert(std::pow(latticeWidth,3) >= numSpheres);

    maxRadiusSphere = latticeParameter/2;
    double minRadiusSphere = maxRadiusSphere/ratioSizeSphere;

    // Max tranlationdistances should be such that translation acceptance is between 30-40%
    maxTranslationDistance = maxRadiusSphere*maxTranslationDistanceInMaxParticleSize;

    // Binary mixture specific 50:50 ratio
    const int numSmallSpheres = numSpheres/2;
    const int numLargeSpheres = numSpheres - numSmallSpheres;
    assert(numSpheres == numSmallSpheres+numLargeSpheres);
    std::uniform_int_distribution<int> chooseRadius(0,1);
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
                            ++numPlacedSmallSpheres;
                        }
                        else
                        {
                            sphere.radius = maxRadiusSphere;
                            ++numPlacedLargeSpheres;
                        }
                    }
                    else if(randomRadius==1)
                    {
                        if(numPlacedLargeSpheres<numLargeSpheres)
                        {
                            sphere.radius = maxRadiusSphere;
                            ++numPlacedLargeSpheres;
                        }
                        else
                        {
                            sphere.radius = minRadiusSphere;
                            ++numPlacedSmallSpheres;
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
        else
        {
            ++rejectedSwaps;
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
            energy += PotentialWCA(RadiusSumOf(sphere,spheres[i]),
                    DistanceBetween(sphere,spheres[i]));
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
    do
    {
        if(positiveOutsideBoundary)
        {
            length -= lengthBox;
        }
        else if(negativeOutsideBoundary)
        {
            length += lengthBox;
        }
    negativeOutsideBoundary = (length < -0.5*lengthBox);
    positiveOutsideBoundary = (length > 0.5*lengthBox);
    }
    while(negativeOutsideBoundary || positiveOutsideBoundary);
}

void System::CorrectForPeriodicSphere(Sphere& sphere)
{
    CorrectForPeriodicCoordinate(sphere.position.x);
    CorrectForPeriodicCoordinate(sphere.position.y);
    CorrectForPeriodicCoordinate(sphere.position.z);
}

void System::CorrectForPeriodicCoordinate(double& coordinate) const
{
    bool negativeOutsideBoundary = (coordinate < 0);
    bool positiveOutsideBoundary = (coordinate >= lengthBox);
    do
    {
        if(positiveOutsideBoundary)
        {
            coordinate -= lengthBox;
        }
        else if(negativeOutsideBoundary)
        {
            coordinate += lengthBox;
        }
        negativeOutsideBoundary = (coordinate < 0);
        positiveOutsideBoundary = (coordinate >= lengthBox);
    }
    while(negativeOutsideBoundary || positiveOutsideBoundary);
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
int System::GetRejectedSwaps() const
{
    return rejectedSwaps;
}

double System::GetTotalEnergy()
{
    double energy = 0;
    for(int i=0; i<numSpheres; ++i)
    {
        energy += 0.5 * CalculateEnergy(i, spheres[i]);
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
