#include <fstream>
#include <sstream>
#include <cassert>

#include "states.h"



States::States(const std::string _statesFile, const int _numSpheres)
    : statesFile(_statesFile), numSpheres(_numSpheres)
{
}

std::vector<std::vector<double>>  States::GetSample(int sampleIndex)
{
    assert(sampleIndex != 0);
    std::vector<std::vector<double>> spherePositions;
    spherePositions.reserve(numSpheres);
    std::vector<double> spherePosition(3);

    int sampleIndexCounter = 0;

    std::ifstream inFile(statesFile);
    if(inFile.is_open())
    {
        std::string line;
        bool iterationOnLine = false;
        while(getline(inFile, line))
        {
            if(line=="iteration")
            {
                iterationOnLine = true;
                ++sampleIndexCounter;
            }
            else if(iterationOnLine)
            {
                iterationOnLine = false;
                sampleIteration = std::stoi(line);
            }
            else
            {
                if(sampleIndexCounter==sampleIndex)
                {
                    std::stringstream linestream(line);
                    std::string value;
                    int item = 0;
                    while(getline(linestream,value,','))
                    {
                        // First 3 elements are x, y, z respectively
                        if(item<3)
                        {
                            spherePosition[item] = std::stod(value);
                        }
                        ++item;
                }
                spherePositions.push_back(spherePosition);
                }
            }

            if(sampleIndexCounter>sampleIndex)
            {
                break;
            }
        }
    }
    return spherePositions;
}

int States::GetSampleIteration(int sampleIndex)
{
    assert(sampleIndex != 0);
    int sampleIndexCounter = 0;
    std::ifstream inFile(statesFile);
    if(inFile.is_open())
    {
        std::string line;
        bool iterationOnLine = false;
        while(getline(inFile, line))
        {
            if(line=="iteration")
            {
                iterationOnLine = true;
                ++sampleIndexCounter;
            }
            else if(iterationOnLine)
            {
                iterationOnLine = false;
                sampleIteration = std::stoi(line);
            }

            if(sampleIndexCounter>sampleIndex)
            {
                break;
            }
        }
    }
    return sampleIteration;
}

int States::GetMaxSampleIndex()
{
    std::ifstream inFile(statesFile);

    if(inFile.is_open())
    {
        std::string line;
        bool iterationOnLine = false;
        int MaxSampleIndex = 0;
        int previousSampleIteration = -1;
        while(getline(inFile, line))
        {
            if(line=="iteration")
            {
                iterationOnLine = true;
            }
            else if(iterationOnLine)
            {
                iterationOnLine = false;
                sampleIteration = std::stoi(line);
                previousSampleIteration = sampleIteration;
                ++MaxSampleIndex;
            }
        }
    return MaxSampleIndex;
    }
}
