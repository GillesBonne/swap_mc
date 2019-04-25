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
        const double lengthBox);
void Export2DVector(std::vector<std::vector<double>> &vector2D);

int main()
{
    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    const int numSpheres = 1024;
    assert(numSpheres > 1);

    const double numDensity = 1;
    const double volumeBox = numSpheres/numDensity;
    const double lengthBox = cbrt(volumeBox);

    const double temperature = 0.25;

    const double ratioSizeSphere = 1.2;

    // Binary mixture specific 50:50 ratio
    const int numSmallSpheres = numSpheres/2;
    const int numLargeSpheres = numSpheres - numSmallSpheres;
    assert(numSpheres == numSmallSpheres+numLargeSpheres);

    MonteCarlo(numSpheres, numSmallSpheres, numLargeSpheres,
            ratioSizeSphere,
            temperature, lengthBox);

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
        const double temperature,
        const double lengthBox)
{
    System system(numSpheres,
            numSmallSpheres,
            numLargeSpheres,
            ratioSizeSphere,
            temperature,
            lengthBox);

    std::cout<<lengthBox<<std::endl;
    std::cout<<std::endl;

    system.PrintStates();
    std::cout<<std::endl;

    std::vector<std::vector<double>> exportedStates = system.GetStates();

    Export2DVector(exportedStates);
}

void Export2DVector(std::vector<std::vector<double>> &vector2D)
{
    std::ofstream outFile("data/outputStates.txt", std::ios_base::trunc);
    for(std::vector<double> vector1D : vector2D)
    {
        for(int i=0; i<4; ++i)
        {
            std::cout<<vector1D[i]<<" ";
            outFile << vector1D[i];
            if(i==3) break;
            outFile << ",";
        }
        std::cout<<std::endl;
        outFile << "\n";
    }
}
