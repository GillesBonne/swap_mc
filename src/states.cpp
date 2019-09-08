#define NDEBUG

#include <fstream>
#include <sstream>
#include <cassert>

#include <iostream>

#include "states.h"

States::States(const std::string _statesFile, const int _numSpheres)
    : statesFile(_statesFile), numSpheres(_numSpheres)
{
}

std::vector<std::vector<double>>  States::GetSample()
{
    std::vector<std::vector<double>> spherePositions;
    spherePositions.reserve(numSpheres);

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
            }
            else if(iterationOnLine)
            {
                iterationOnLine = false;
            }
            else
            {
                std::vector<double> spherePosition(4);
                std::stringstream linestream(line);
                std::string value;
                int item = 0;
                while(getline(linestream,value,','))
                {
                    // First 3 elements are x, y, z respectively
                    if(item<4)
                    {
                        spherePosition[item] = std::stod(value);
                    }
                    ++item;
                }
                spherePositions.push_back(spherePosition);
            }
        }
    }
    return spherePositions;
}
