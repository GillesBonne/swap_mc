#ifndef EXPORT2D_H
#define EXPORT2D_H

#include <vector>
#include <string>

void ClearContents(const std::string& filePath);
void Export2D(const std::vector<std::vector<double>> &vector2D,
        const std::string& filePath,
        const long long int iteration);

void ExportItem(const long long int item, std::string& filePath);
void ExportItem(const double item, std::string& filePath);
#endif
