#define NDEBUG

#include <iostream>
#include <cmath>
#include <random>
#include <cassert>
#include <algorithm>
#include <string>

#include "timer.h"
#include "system.h"
#include "config.h"
#include "states.h"

System::System(const Config& config, const bool usePreviousStates, std::string previousID)
    :   numSpheres(config.GetNumSpheres()),
        spheres(numSpheres),
        temperatureFixed(config.GetTemperatureFixed()),
        maxTranslationDistanceInLengthUnits(config.GetMaxTranslationDistanceInLengthUnits()),
        numDensity(config.GetNumDensity()),
        volumeBox(numSpheres/numDensity),
        lengthBox(cbrt(volumeBox)),
        //mersenneTwister((std::random_device())()),
        mersenneTwister(42),
        randomDouble(0,1),
        randomPosNegDouble(-1,1),
        randomParticle(0,numSpheres-1)
{
    Timer timer;

    // Get the number of spheres along one dimension.
    int numSpheres1D;
    for(int i=2; i<=numSpheres; ++i)
    {
        if(i*i*i == numSpheres)
        {
            numSpheres1D = i;
            break;
        }
    }

    double lengthUnit = lengthBox/numSpheres1D;

    double ratioSizeSphere;
    double sigmaMax;
    double sigmaMin;

    if(toggleContinuousPolydisperse)
    {
        // Average sigma should be 1.
        sigmaMax = 1.6095;
        sigmaMin = 0.725327;

        maxRadiusSphere = 0.5*sigmaMax;
        minRadiusSphere = 0.5*sigmaMin;
    }
    if(toggleBinaryMixture)
    {
        ratioSizeSphere = 2.219;
        sigmaMax = lengthUnit;
        sigmaMin = sigmaMax/ratioSizeSphere;

        maxRadiusSphere = 0.5*sigmaMax;
        minRadiusSphere = 0.5*sigmaMin;
    }


    maxTranslationDistance = maxTranslationDistanceInLengthUnits * lengthUnit;

    if(usePreviousStates)
    {
        std::cout<<"Use previous states"<<std::endl;
        std::string previousConfigFile = "data/data" + previousID + "/outputStates.txt";
        States states(previousConfigFile, numSpheres);

        int maxSampleIndex = states.GetMaxSampleIndex();

        std::vector<std::vector<double>> spherePositions(numSpheres);
        spherePositions = states.GetSample(maxSampleIndex, true);

        for(int i=0; i<numSpheres; ++i)
        {
            spheres[i].position.x = spherePositions[i][0];
            spheres[i].position.y = spherePositions[i][1];
            spheres[i].position.z = spherePositions[i][2];
            spheres[i].radius = spherePositions[i][3];
        }
    }
    else
    {
        std::cout<<"Do not use previous states"<<std::endl;
        // Initialize random radii.
        if(toggleContinuousPolydisperse)
        {
            int numBins = 10000;
            double interval = (double) (sigmaMax - sigmaMin)/numBins;

            std::vector<double> sigmas(numBins);
            std::vector<double> weights(numBins);

            for(int i=0; i<numBins; ++i)
            {
                double sig = sigmaMin+(i+0.5)*interval;
                sigmas[i] = sig;

                double weight = 1/pow(sig,3);
                weights[i] = weight;
            }

            std::discrete_distribution<> randomSigma(weights.begin(), weights.end());

            for(int i=0; i<numSpheres; ++i)
            {
                int index = randomSigma(mersenneTwister);
                double radius = 0.5*sigmas[index];
                spheres[i].radius = radius;
            }
        }
        if(togglePolydisperse)
        {
            int numSpecies = 2;
            int n = numSpecies - 1;

            double sigmaMax;
            double sigmaMin;

            // Function constants.
            double a;
            double b;
            double c;

            // Values specific for the number of species and ratio=2.219.
            // Can be calculated using discrete_polydisperse.nb.
            if(numSpecies==2)
            {
                sigmaMax =  2.01323;
                sigmaMin =  0.907267;
                a =         0.684184;
                b =         sigmaMin;
                c =         1.10596;
            }
            else if(numSpecies==3)
            {
                sigmaMax =  1.85908;
                sigmaMin =  0.837803;
                a =         0.441701;
                b =         sigmaMin;
                c =         0.510641;
            }
            else if(numSpecies==5)
            {
                sigmaMax =  1.78458;
                sigmaMin =  0.804227;
                a =         0.321294;
                b =         sigmaMin;
                c =         0.326784;
            }
            else if(numSpecies==7)
            {
                sigmaMax =  1.69954;
                sigmaMin =  0.765902;
                a =         0.175843;
                b =         sigmaMin;
                c =         0.155606;
            }
            else if(numSpecies==10)
            {
                sigmaMax =  1.66969;
                sigmaMin =  0.752453;
                a =         0.121035;
                b =         sigmaMin;
                c =         0.101916;
            }
            else if(numSpecies==20)
            {
                sigmaMax =  1.63799;
                sigmaMin =  0.738164;
                a =         0.0593967;
                b =         sigmaMin;
                c =         0.047359;
            }
            else if(numSpecies==40)
            {
                sigmaMax =  1.62335;
                sigmaMin =  0.731569;
                a =         0.0294358;
                b =         sigmaMin;
                c =         0.0228662;
            }
            else if(numSpecies==80)
            {
                sigmaMax =  1.61633;
                sigmaMin =  0.728405;
                a =         0.0146545;
                b =         sigmaMin;
                c =         0.0112396;
            }
            else
            {
                throw std::out_of_range("Parameters of the set number of species are not defined.");
            }

            maxRadiusSphere = 0.5*sigmaMax;
            minRadiusSphere = 0.5*sigmaMin;

            double interval = (sigmaMax - sigmaMin)/n;

            std::vector<double> sigmas(numSpecies);
            std::vector<double> weights(numSpecies);

            for(int i=0; i<numSpecies; ++i)
            {
                sigmas[i] = i*interval + sigmaMin;
                weights[i] = a/(pow((b+c*i),3));
            }

            std::discrete_distribution<> randomSigma(weights.begin(), weights.end());

            for(int i=0; i<numSpheres; ++i)
            {
                int index = randomSigma(mersenneTwister);
                double radius = 0.5*sigmas[index];
                spheres[i].radius = radius;
            }
        }
        if(toggleBinaryMixture)
        {
            if(toggleRadius5050)
            {
                const int numSmallSpheres = 0.5 * numSpheres;
                const int numLargeSpheres = numSpheres - numSmallSpheres;

                int numPlacedSmallSpheres = 0;
                int numPlacedLargeSpheres = 0;

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
            if(toggleRadius8020)
            {
                const int numSmallSpheres = 0.2 * numSpheres;
                const int numLargeSpheres = numSpheres - numSmallSpheres;

                int numPlacedSmallSpheres = 0;
                int numPlacedLargeSpheres = 0;

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

        // Initialize positions.
        int numPlacedSpheres = 0;
        for(int i=0; i<numSpheres1D; ++i)
        {
            for(int j=0; j<numSpheres1D; ++j)
            {
                for(int k=0; k<numSpheres1D; ++k)
                {
                    if(numPlacedSpheres<numSpheres)
                    {
                        spheres[numPlacedSpheres].position.x = (i + 0.5)*lengthUnit;
                        spheres[numPlacedSpheres].position.y = (j + 0.5)*lengthUnit;
                        spheres[numPlacedSpheres].position.z = (k + 0.5)*lengthUnit;
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
    // Choose two DIFFERENT particles randomly
    int randomParticleIndex1 = ChooseRandomParticle();
    int randomParticleIndex2;
    do
    {
        randomParticleIndex2 = ChooseRandomParticle();
    }
    while((randomParticleIndex1 == randomParticleIndex2)
            ||
            (spheres[randomParticleIndex1].radius==spheres[randomParticleIndex2].radius));

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
            if(toggleWCA)
            {
                energy += PotentialWCA(RadiusSumOf(sphere,spheres[i]),
                    DistanceBetween(sphere,spheres[i]));
            }
            if(toggleSRPP)
            {
                energy += PotentialSRPP(RadiusSumOf(sphere,spheres[i]),
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
        double epsilonValue;
        double sigma;
        if(toggleKobAnderson)
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
                throw std::out_of_range("Kob Anderson: radii do not match in potential calculation.");
            }
        }
        else
        {
            epsilonValue = epsilonConstant;
            sigma = sigmaSummedRadius;
        }
        potential = epsilonValue + 4 * epsilonValue * (- pow((sigma/distanceBetweenSpheres),6)
                                    + pow((sigma/distanceBetweenSpheres),12));
    }
    return potential;
}

double System::PotentialSRPP(const double sigmaSummedRadius, const double distanceBetweenSpheres) const
{
    double potential;
    const double cutOffDistance = 1.25 * sigmaSummedRadius;
    if(distanceBetweenSpheres > cutOffDistance)
    {
        potential = 0;
    }
    else
    {
        const int n = 12;
        const double c0 = -1.92415;
        const double c2 = 2.11106;
        const double c4 = -0.591097;

        potential = pow((sigmaSummedRadius/distanceBetweenSpheres),n)
                        + c4 * pow((distanceBetweenSpheres/sigmaSummedRadius),4)
                        + c2 * pow((distanceBetweenSpheres/sigmaSummedRadius),2)
                        + c0;
    }
    return potential;
}

double System::RadiusSumOf(const Sphere& sphere1, const Sphere& sphere2) const
{
    if(toggleChangeAdditivity)
    {
        double nonAdditivityConstant = 0.2;

        return (sphere1.radius + sphere2.radius)*
            (1 - 2*nonAdditivityConstant*abs(sphere1.radius - sphere2.radius));
    }
    else
    {
        return sphere1.radius + sphere2.radius;
    }
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
            if(toggleWCA)
            {
                energy += PotentialWCA(RadiusSumOf(spheres[i],spheres[j]),
                        DistanceBetween(spheres[i],spheres[j]));
            }
            if(toggleSRPP)
            {
                energy += PotentialSRPP(RadiusSumOf(spheres[i],spheres[j]),
                        DistanceBetween(spheres[i],spheres[j]));
            }
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

            if(toggleWCA)
            {
                xForce = ForceWCA(xDiff, distanceSquaredBetween, sigmaSummedRadius);
                yForce = ForceWCA(yDiff, distanceSquaredBetween, sigmaSummedRadius);
                zForce = ForceWCA(zDiff, distanceSquaredBetween, sigmaSummedRadius);
            }
            if(toggleSRPP)
            {
                xForce = ForceSRPP(xDiff, distanceSquaredBetween, sigmaSummedRadius);
                yForce = ForceSRPP(yDiff, distanceSquaredBetween, sigmaSummedRadius);
                zForce = ForceSRPP(zDiff, distanceSquaredBetween, sigmaSummedRadius);
            }

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

double System::ForceWCA(double difference, double sqDistance, double sigmaSummedRadius)
{
    double force;
    const double cutOffDistance = 1.12246204831 * sigmaSummedRadius;
    if(sqDistance > (cutOffDistance*cutOffDistance))
    {
        force = 0;
    }
    else
    {
        double epsilonValue;
        double sigma;
        if(toggleKobAnderson)
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
                throw std::out_of_range("Kob Anderson: radii do not match in force calculation.");
            }
        }
        else
        {
            epsilonValue = epsilonConstant;
            sigma = sigmaSummedRadius;
        }
        force = 24 * epsilonValue * difference * pow(sigma, 6)
            * (2*pow(sigma, 6)-sqDistance*sqDistance*sqDistance)
            / (pow(sqDistance,7));
    }
    return force;
}

double System::ForceSRPP(double difference, double sqDistance, double sigmaSummedRadius)
{
    double force;
    const double cutOffDistance = 1.25 * sigmaSummedRadius;
    if(sqDistance > (cutOffDistance*cutOffDistance))
    {
        force = 0;
    }
    else
    {
        const int n = 12;
        const double c2 = 2.11106;
        const double c4 = -0.591097;

        force = - difference * (4*c4*sqDistance/(pow(sigmaSummedRadius,4))
                                + 2*c2/(sigmaSummedRadius*sigmaSummedRadius)
                                - n/(sqDistance) * pow((sigmaSummedRadius/sqrt(sqDistance)),n));
    }
    return force;
}
