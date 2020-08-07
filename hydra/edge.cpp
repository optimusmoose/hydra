#include "edge.hpp"
#include "envelope.hpp"

Edge::Edge(const float weight_, Envelope* e1_, Envelope* e2_)
{
    e1 = e1_;
    e2 = e2_;
    weight = weight_;
}

