#ifndef alignment_hpp
#define alignment_hpp

#include <stdio.h>
#include <vector>
#include <queue>
#include <memory>

#include "edge.hpp"
#include "grid.hpp"
#include "cluster.hpp"
#include "cmd_parser.hpp"

// Alignment algorithms

namespace Alignment {

class EdgeComparator
{
public:
    int operator() (Edge& e1, Edge& e2)
    {
        return e1.weight > e2.weight;
    }
};
class Algorithm
{
protected:
    float distance(float& p1, float& p2);
    void delete_cluster(std::vector<Cluster>& cs_clusters, int& id);
    float normalize(float& num, float& denom);
};

class Feature : Algorithm
{
    std::vector<CellMap> &cell_maps;
    std::vector<std::vector<std::unique_ptr<Cluster>>> &clusters;
    std::vector<std::vector<std::unique_ptr<Edge>>> &edges;
    CMDParser& cmds;
public:
    Feature(std::vector<CellMap>& cell_maps_, CMDParser& cmds, std::vector<std::vector<std::unique_ptr<Cluster>>> &clusters_, std::vector<std::vector<std::unique_ptr<Edge>>> &edges_);
    void perform();
private:
    void build_edges(const int &rt_h, const int &mz_h, CellMap &cm);
    void balance_vectors(std::vector<float>& v1, std::vector<float>& v2);
    void dot(std::vector<float>& v1, std::vector<float>& v2);
    void print_relationship(int& rt_h, int& mz_h);
    void print_relationship(int& rt_h1, int& mz_h1, int& rt_h2, int& mz_h2);
    void print_relationship(Envelope* e1, Envelope* e2);
    void build_cluster(std::vector<Cluster>& cs_clusters, Edge& e, int id);
    void finalize_cs_clusters(std::vector<std::unique_ptr<Cluster>>& cs_clusters);
    float cosine_similarity(Envelope* e1, Envelope* e2);
    float vector_magnitude(std::vector<float> v);
    float pythags(float &f1, float &f2);
    bool added(Edge* e, std::vector<std::unique_ptr<Cluster>>& cs_clusters);
    bool valid_run_id_intersection(Cluster* c1, Cluster* c2);
    std::vector<Envelope*> get_vector_of_ptrs(std::vector<Envelope>* envelopes);
};

}

#endif /* alignment_hpp */
