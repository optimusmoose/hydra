#ifndef cluster_hpp
#define cluster_hpp

#include <stdio.h>
#include <vector>
#include <unordered_set>

#include "envelope.hpp"
#include "edge.hpp"



//class Envelope;
//class Edge;

class Cluster
{
public:
    Cluster(int id_);
    Cluster();

    int id;
    bool deleted = false;
    bool valid_run_id_add(int& run_id);
    bool add_or_dont(Edge* edge);
    void remove_edge_by_envelope_ptr(Envelope *e);
    std::vector<Edge*> edges;
    std::unordered_set<int> get_run_ids();
    std::unordered_set<Envelope*> uniq_envelopes();
    Edge* get_edge_by_run_id(int &run_id);
};

#endif /* cluster_hpp */
