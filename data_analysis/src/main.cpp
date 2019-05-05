#include <vector>
#include <cassert>

#include "states.h"

#include "../../src/config.h"
#include "../../src/config.cpp"

void RadialDistributionFunction(const std::vector<std::vector<double>>& spherePositions,
                                Config lastConfig);
double CalculateDistance(std::vector<double> sphere1, std::vector<double> sphere2, double lengthBox);
void CorrectForPeriodic(double& length, const double lengthBox);

template <typename T>
std::vector<T> linspace(T a, T b, size_t N);

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
    int numRealBins = numBins - 1;

    std::ofstream outFile("data/radialDistribution.txt", std::ios_base::trunc);
    outFile << "binSize"<< "\n" << binSize << "\n";
    for(int i=0; i<numRealBins; ++i)
    {
        outFile << radial[i];
        outFile << ",";
        outFile << bins[i];
        outFile << "\n";
    }

    // Structure factor
    double dr = radial[1]-radial[0];

    std::vector<double> Xk;

    int numQ = 1000;
    std::vector<double> qVector = linspace(0.1,50.0,numQ);

    for(double q : qVector)
    {
        double sum = 0;
        int incre = 0;
        for(double r : radial)
        {
            sum += r * (bins[incre]-1) * sin(q * r);
            incre += 1;
        }
        double XkItem = 1 + 4 * PI * numDensity * sum * dr / q;
        Xk.push_back(XkItem);
    }

    std::ofstream outFileStructure("data/structureFactor.txt", std::ios_base::trunc);
    outFileStructure << "binSize"<< "\n" << binSize << "\n";
    for(int i=0; i<numQ; ++i)
    {
        outFileStructure << qVector[i];
        outFileStructure << ",";
        outFileStructure << Xk[i];
        outFileStructure << "\n";
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

template <typename T>
std::vector<T> linspace(T a, T b, size_t N)
{
    T h = (b - a) / static_cast<T>(N-1);
    std::vector<T> xs(N);
    typename std::vector<T>::iterator x;
    T val;
    for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h)
        *x = val;
    return xs;
}
