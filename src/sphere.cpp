#include "sphere.h"

bool Sphere::operator==(const Sphere &right) const
{
    return (radius == right.radius) && (position == right.position);
}
