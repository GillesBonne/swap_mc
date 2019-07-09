#ifndef DISTRIBUTION_H
#define DISTRUBUTION_H

class Distribution
{
    double sigmaMax;
    double sigmaMin;

    double a;
    double b;
    double c;

public:
    Distribution(const int numSpecies);

    double GetSigmaMax() const;
    double GetSigmaMin() const;

    double GetConstA() const;
    double GetConstB() const;
    double GetConstC() const;
};

#endif
