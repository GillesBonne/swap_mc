#define NDEBUG

#include <fstream>
#include <vector>
#include <string>

#include "export2D.h"

void ClearContents(const std::string& filePath)
{
    std::ofstream outFile(filePath, std::ios_base::trunc);
}

void Export2D(const std::vector<std::vector<double>> &vector2D,
        const std::string& filePath,
        const int iteration)
{
    int vectorSize = vector2D[0].size();
    std::ofstream outFile(filePath, std::ios_base::app);
    outFile << "iteration";
    outFile << "\n";
    outFile << iteration;
    outFile << "\n";
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

void ExportItem(const int item, std::string& filePath)
{
    std::ofstream outFile(filePath, std::ios_base::app);
    outFile << item;
    outFile << "\n";
}
void ExportItem(const double item, std::string& filePath)
{
    std::ofstream outFile(filePath, std::ios_base::app);
    outFile << item;
    outFile << "\n";
}
