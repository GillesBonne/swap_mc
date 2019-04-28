#include <fstream>
#include <vector>
#include <string>

#include "export2D.h"

void Export2D(const std::vector<std::vector<double>> &vector2D, const std::string& filePath)
{
    int vectorSize = vector2D[0].size();
    std::ofstream outFile(filePath, std::ios_base::trunc);
    for(std::vector<double> vector1D : vector2D)
    {
        for(int i=0; i<vectorSize; ++i)
        {
            outFile << vector1D[i];
            if(i<(vectorSize-1))
            {
                outFile << ",";
            }
        }
        outFile << "\n";
    }
}

