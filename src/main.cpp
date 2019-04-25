// Place #define NDEBUG if asserts should not be evaluated.

#include <iostream>
#include <chrono>
#include <cmath>
#include <fstream>
#include <assert.h>

#include "system.h"

void MonteCarlo(const int numSpheres,
        const int numSmallSpheres,
        const int numLargeSpheres,
        const double ratioSizeSphere,
        const double temperature,
        const double lengthBox,
        const double swapProbability,
        const double epsilonConstant);
void Export2DVector(std::vector<std::vector<double>> &vector2D);

int main()
{
    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    const int numSpheres = 1400;
    assert(numSpheres > 1);

    const double numDensity = 1;
    const double volumeBox = numSpheres/numDensity;
    const double lengthBox = cbrt(volumeBox);

    const double temperatureFixed = 0.25;

    const double epsilonConstant = 1;

    const double ratioSizeSphere = 1.2;

    // Binary mixture specific 50:50 ratio
    const int numSmallSpheres = numSpheres/2;
    const int numLargeSpheres = numSpheres - numSmallSpheres;
    assert(numSpheres == numSmallSpheres+numLargeSpheres);

    const double swapProbability = 0.0;

    MonteCarlo(numSpheres, numSmallSpheres, numLargeSpheres, ratioSizeSphere,
            temperatureFixed, lengthBox, swapProbability, epsilonConstant);

    // Record end time
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    return 0;
}

void MonteCarlo(const int numSpheres,
        const int numSmallSpheres,
        const int numLargeSpheres,
        const double ratioSizeSphere,
        const double temperatureFixed,
        const double lengthBox,
        const double swapProbability,
        const double epsilonConstant)
{
    System system(numSpheres,
            numSmallSpheres,
            numLargeSpheres,
            ratioSizeSphere,
            temperatureFixed,
            lengthBox,
            swapProbability,
            epsilonConstant);

    system.PrintStates();

    std::vector<std::vector<double>> exportedStates = system.GetStates();

    Export2DVector(exportedStates);

    if(swapProbability > 0)
    {
        system.AttemptSwap();
    }

    system.AttemptTranslation();
}

void Export2DVector(std::vector<std::vector<double>> &vector2D)
{
    std::ofstream outFile("data/outputStates.txt", std::ios_base::trunc);
    for(std::vector<double> vector1D : vector2D)
    {
        for(int i=0; i<4; ++i)
        {
            outFile << vector1D[i];
            if(i==3)
            {
                break;
            }
            outFile << ",";
        }
        outFile << "\n";
    }
}
