#ifndef STATES_H
#define STATES_H

#include <string>
#include <vector>

class States
{
private:
    const std::string statesFile;
    const int numSpheres;
    int sampleIteration;
public:
    States(const std::string statesFile, const int numSpheres);
    std::vector<std::vector<double>>  GetSample(int sampleIndex, bool withRadius);
    int GetSampleIteration(int sampleIndex);
    int GetMaxSampleIndex();
};

#endif
