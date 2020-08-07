#include <unordered_set>

#include "cluster.hpp"

// Cluster function implementations

Cluster::Cluster(int id_)
{
    id = id_;
}
Cluster::Cluster() {}

Edge* Cluster::get_edge_by_run_id(int &run_id)
{
    for (Edge *e : edges)
    {
        if (e -> e1 -> run_id == run_id)
            return e;
        if (e -> e2 -> run_id == run_id)
            return e;
    }
    return NULL;
}

void Cluster::remove_edge_by_envelope_ptr(Envelope *e)
{
    int index = 0;
    for (Edge *edge : edges)
    {
        if (e == edge -> e2)
            edges.erase(edges.begin() + index);
        ++index;
    }
}

bool Cluster::valid_run_id_add(int &run_id)
{
    for (auto &e : edges)
    {
        if (e -> e1 -> run_id == run_id || e -> e2 -> run_id == run_id)
            return false;
    }
    return true;
}

bool Cluster::add_or_dont(Edge* edge)
{
    if (valid_run_id_add(edge -> e2 -> run_id))
    {
        // Doesn't exist
        // Add
        edges.emplace_back(edge);
        return true;
    }
    else
    {
        // Does exist
        // Compare against current
        int index = 0;
        for (Edge *edge2 : edges)
        {
            if (edge -> e2 -> run_id == edge2 -> e2 -> run_id)
            {
                if (edge -> weight < edge2 -> weight)
                {
                    edges[index] = edge;
                    return true;
                }
            }
            ++index;
        }
    }
    return false;
}

std::unordered_set<int> Cluster::get_run_ids()
{
    std::unordered_set<int> us;

    for (auto &e : edges)
    {
        us.insert(e -> e1 -> run_id);
        us.insert(e -> e2 -> run_id);
    }

    return us;
}

std::unordered_set<Envelope*> Cluster::uniq_envelopes()
{
    std::unordered_set<Envelope*> envelopes;
    
    for (Edge *e : edges)
    {
        envelopes.insert(e->e1);
        envelopes.insert(e->e2);
    }
    
    return envelopes;
}
// End Cluster function implementations
