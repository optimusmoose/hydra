#ifndef edge_hpp
#define edge_hpp

#include <stdio.h>

#include "envelope.hpp"

class Edge
{
public:
    Envelope *e1;
    Envelope *e2;
    float weight;
    Edge(const float weight_, Envelope* e1_, Envelope* e2_);
};

#endif /* edge_hpp */
