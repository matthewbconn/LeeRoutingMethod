#include "mnPoint.h"

// Class Constructor
// post: Initializes x and y to zero
mnPoint::mnPoint(): m(0), n(0)
{}

// Class Constructor
// post: Initializes x and y
mnPoint::mnPoint(int mVal,int nVal): m(mVal), n(nVal)
{}


// Equality operators
bool mnPoint::operator== (const mnPoint& rhs) const
{
    return (this->m==rhs.m && this->n==rhs.n);
}
bool mnPoint::operator!= (const mnPoint& rhs) const
{
    return !(*this==rhs);
}


// insertion operator for printing a Point
std::ostream & operator<< (std::ostream &out, const mnPoint& p)
{
    out << "(" << p.m << "," << p.n << ")";
    return out;
}


