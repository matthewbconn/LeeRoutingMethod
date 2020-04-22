#ifndef LEEBASETWO_MNPOINT_H
#define LEEBASETWO_MNPOINT_H

#include <iostream>

class mnPoint
{
public:
    int m;
    int n;

    // Class Constructor
    // post: Initializes x and y to zero
    mnPoint();

    // Class Constructor
    // post: Initializes x and y
    mnPoint(int x,int y);


    // Equality operators
    bool operator== (const mnPoint& rhs) const;
    bool operator!= (const mnPoint& rhs) const;

};

// insertion operator for output
std::ostream& operator<< (std::ostream& os, const mnPoint &p);

#endif //LEEBASETWO_MNPOINT_H
