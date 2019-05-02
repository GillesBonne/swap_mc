#include <string>
#include <vector>
//States::States(const std::string _statesFile) : statesFile(_statesFile)
//{
//
//}
//
//States::GetNextSample()
//{
//
//}


class States
{
private:
    const std::string statesFile;
    const int numSpheres;
    int sampleIteration;
public:
    States(const std::string statesFile, const int numSpheres);
    std::vector<std::vector<double>>  GetSample(int sampleIndex);
    int GetSampleIteration(int sampleIndex);
    int GetMaxSampleIndex();
};
