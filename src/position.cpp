#define NDEBUG

#include "position.h"

bool Position::operator==(const Position &right) const
{
    return (x == right.x) && (y == right.y) && (z == right.z);
}
