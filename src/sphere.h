#ifndef SPHERE_H
#define SPHERE_H

#include "position.h"

struct Sphere
{
    Position position;
    double radius;

    bool operator==(const Sphere &right) const;
};

#endif
