#include <vector>

#include "states.h"

#include "../../src/config.h"
#include "../../src/config.cpp"

void RadialDistributionFunction(const std::vector<std::vector<double>>& spherePositions,
                                Config lastConfig);
double CalculateDistance(std::vector<double> sphere1, std::vector<double> sphere2, double lengthBox);
void CorrectForPeriodic(double& length, const double lengthBox);

int main()
{
    Config lastConfig("../data/lastConfig.txt");

    int numSpheres = lastConfig.GetNumSpheres();

    States states("../data/outputStates.txt", numSpheres);
    int maxSampleIndex = states.GetMaxSampleIndex();

    std::vector<std::vector<double>> spherePositions;
    spherePositions = states.GetSample(1);

    for(std::vector<double> spherePosition : spherePositions)
    {
        for(double pos : spherePosition)
        {
            std::cout<<pos<<", ";
        }
        std::cout<<std::endl;
    }


    spherePositions = states.GetSample(maxSampleIndex);

    for(std::vector<double> spherePosition : spherePositions)
    {
        for(double pos : spherePosition)
        {
            std::cout<<pos<<", ";
        }
        std::cout<<std::endl;
    }

    std::cout<<std::endl;
    std::cout<<spherePositions[2][0]<<std::endl;
    std::cout<<std::endl;

    RadialDistributionFunction(spherePositions, lastConfig);
}

void RadialDistributionFunction(const std::vector<std::vector<double>>& spherePositions,
                                Config lastConfig)
{
    const int numSpheres = lastConfig.GetNumSpheres();

    const double maxRadiusSphere = 0.5 * lastConfig.GetLatticeParameter();
    const double binSize = 0.1 * maxRadiusSphere;

    const double lengthBox = lastConfig.GetLengthBox();

    int numBins = ceil(0.5*lengthBox/binSize);

    std::vector<int> bins(numBins);

    std::cout<<"numBins"<< numBins<<std::endl;

    for(int i=0; i<(numSpheres-1); ++i)
    {
        for(int j=i+1; j<numSpheres; ++j)
        {
            double distanceBetween = CalculateDistance(spherePositions[i], spherePositions[j], lengthBox);
            int binIndex = floor(distanceBetween);
            ++bins[binIndex];
        }
    }

    std::cout<<std::endl;
    for(int it : bins)
    {
        std::cout<<it<<std::endl;
    }
    std::cout<<std::endl;

    std::vector<double> radial(numBins);
    for(int i=0; i<numBins; ++i)
    {
        radial[i] = (0.5 + i) * binSize;
    }
}

double CalculateDistance(std::vector<double> sphere1, std::vector<double> sphere2, double lengthBox)
{
    double diffX = sphere1[0] - sphere2[0];
    double diffY = sphere1[1] - sphere2[1];
    double diffZ = sphere1[2] - sphere2[2];

    CorrectForPeriodic(diffX, lengthBox);
    CorrectForPeriodic(diffY, lengthBox);
    CorrectForPeriodic(diffZ, lengthBox);

    return sqrt(diffX*diffX + diffY*diffY + diffZ*diffZ);
}

void CorrectForPeriodic(double& length, const double lengthBox)
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
