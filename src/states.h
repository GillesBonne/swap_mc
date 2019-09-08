#ifndef STATES_H
#define STATES_H

#include <string>
#include <vector>

class States
{
private:
    const std::string statesFile;
    const int numSpheres;
public:
    States(const std::string statesFile, const int numSpheres);
    std::vector<std::vector<double>>  GetSample();
};

#endif
