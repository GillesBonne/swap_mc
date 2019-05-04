#include <vector>
#include <cassert>

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
    spherePositions = states.GetSample(maxSampleIndex);

    RadialDistributionFunction(spherePositions, lastConfig);
}

void RadialDistributionFunction(const std::vector<std::vector<double>>& spherePositions,
                                Config lastConfig)
{
    const double PI = 3.14159265;

    const int numSpheres = lastConfig.GetNumSpheres();

    const double maxDiameterSphere = lastConfig.GetLatticeParameter();
    const double binSize = 0.1 * maxDiameterSphere;

    const double lengthBox = lastConfig.GetLengthBox();

    double maxDistance = 0.5 * lengthBox;
    int numBins = ceil(maxDistance/binSize);

    std::vector<double> bins(numBins);

    for(int i=0; i<(numSpheres-1); ++i)
    {
        for(int j=i+1; j<numSpheres; ++j)
        {
            double distanceBetween = CalculateDistance(spherePositions[i], spherePositions[j], lengthBox);
            int binIndex = round(distanceBetween/binSize);
            if(binIndex < floor(lengthBox/(2*binSize)))
            {
                bins[binIndex] += 1.0;
            }
        }
    }

    std::vector<double> radial(numBins);
    for(int i=0; i<numBins; ++i)
    {
        radial[i] = (0.5 + i) * binSize;
    }

    const double numDensity = lastConfig.GetNumDensity();
    double r;
    double rPlusdR;
    double volumeShell;
    for(int i=0; i<numBins; ++i)
    {
        r = radial[i] - 0.5*binSize;
        rPlusdR = r + binSize;
        volumeShell = 2.0 * PI * r*r * binSize;

        bins[i] /= numDensity;
        bins[i] /= numSpheres;
        bins[i] /= volumeShell;
    }

    // As the first item is divided by zero.
    bins.erase(bins.begin());
    radial.erase(radial.begin());

    for(int i=0; i<numBins-1; ++i)
    {
        std::cout<<radial[i]<<" "<<bins[i]<<std::endl;
    }

    std::ofstream outFile("data/radialDistribution.txt", std::ios_base::trunc);
    outFile << "binSize"<< "\n" << binSize << "\n";
    for(int i=0; i<numBins-1; ++i)
    {
        outFile << radial[i];
        outFile << ",";
        outFile << bins[i];
        outFile << "\n";
    }
}

double CalculateDistance(std::vector<double> sphere1, std::vector<double> sphere2, double lengthBox)
{
    double diffX = (sphere1[0] - sphere2[0]);
    double diffY = (sphere1[1] - sphere2[1]);
    double diffZ = (sphere1[2] - sphere2[2]);

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
