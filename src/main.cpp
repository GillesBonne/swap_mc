// Place #define NDEBUG if asserts should not be evaluated.

#include <iostream>
#include <chrono>
#include <cmath>
#include <assert.h>

#include "system.h"

void MonteCarlo(const int numSpheres,
        const int numSmallSpheres,
        const int numLargeSpheres,
        const double minSizeSphere,
        const double maxSizeSphere,
        const double temperature,
        const double lengthBox);


int main()
{
    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    const int numSpheres = 24;
    assert(numSpheres > 1);

    const double numDensity = 1;
    const double volumeBox = numSpheres/numDensity;
    const double lengthBox = cbrt(volumeBox);

    const double temperature = 0.25;

    const double minSizeSphere = 1;
    const double maxSizeSphere = 1.2;
    assert(minSizeSphere != maxSizeSphere);

    // Binary mixture specific 50:50 ratio
    const int numSmallSpheres = numSpheres/2;
    const int numLargeSpheres = numSpheres - numSmallSpheres;
    assert(numSpheres == numSmallSpheres+numLargeSpheres);

    MonteCarlo(numSpheres, numSmallSpheres, numLargeSpheres,
            minSizeSphere, maxSizeSphere,
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
        const double minSizeSphere,
        const double maxSizeSphere,
        const double temperature,
        const double lengthBox)
{
    System system(numSpheres,
            numSmallSpheres,
            numLargeSpheres,
            minSizeSphere,
            maxSizeSphere,
            temperature,
            lengthBox);

    std::cout<<lengthBox<<std::endl;
    std::cout<<std::endl;

    system.PrintStates();
}
