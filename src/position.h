#ifndef POSITION_H
#define POSITION_H

struct Position
{
    double x;
    double y;
    double z;

    bool operator==(const Position &right) const;
};

#endif
