#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <climits>
//#include <unordered_set>
#include <memory>


#include "alignment.hpp"
#include "grid.hpp"
#include "edge.hpp"
#include "cluster.hpp"
#include "correspondence.hpp"

namespace Alignment {

// Start Alignment::Algorithm function implementations

float Algorithm::distance(float& p1, float& p2)
{
    return abs(p1 - p2);
}

float Algorithm::normalize(float& num, float& denom)
{
    return num/denom;
}

// End Alignment::Algorithm function implementations

// Start Alignment::Feature function implementations

Feature::Feature(std::vector<CellMap>& cell_maps_, CMDParser& cmds_, std::vector<std::vector<std::unique_ptr<Cluster>>> &clusters_, std::vector<std::vector<std::unique_ptr<Edge>>> &edges_) : cell_maps(cell_maps_), clusters(clusters_), cmds(cmds_), edges(edges_) {}

void Feature::print_relationship(int& rt_h, int& mz_h)
{
    printf("%i,%i -> %i,%i\n", rt_h, mz_h, rt_h, mz_h);
}

void Feature::print_relationship(int& rt_h1, int& mz_h1, int& rt_h2, int& mz_h2)
{
    printf("%i,%i -> %i,%i\n", rt_h1, mz_h1, rt_h2, mz_h2);
}

void Feature::print_relationship(Envelope *e1, Envelope *e2)
{
    std::cout << e1 -> run_id << "," << e1 -> id << " -> " << e2 -> run_id << "," << e2 -> id << "\n";
}

float Feature::vector_magnitude(std::vector<float> v)
{
    float sum = 0;

    for (float& f : v)
    {
        f = pow(f, 2);
    }
    for (float& f : v)
    {
        sum += f;
    }
    return sqrt(sum);
}

void Feature::balance_vectors(std::vector<float> &v1, std::vector<float> &v2)
{
    // Balance vectors with zeroes if necessary
    int v1_size = (int)v1.size();
    int v2_size = (int)v2.size();

    if (v1_size > v2_size)
    {
        for (int i = 0; i < (v1_size - v2_size); ++i)
        {
            v2.emplace_back(0);
        }
    }
    else if (v1_size < v2_size)
    {
        for (int i = 0; i < (v2_size - v1_size); ++i)
        {
            v1.emplace_back(0);
        }
    }
}

float Feature::cosine_similarity(Envelope *e1, Envelope *e2)
{
    // Copy vectors
    std::vector<float> temp_vec1 = e1 -> isotope_ratios;
    std::vector<float> temp_vec2 = e2 -> isotope_ratios;
    
    // Catch for unlikely event of literally the same isotope ratios
    if (temp_vec1 == temp_vec2)
        return 0.0;
    
    // Balance vector
    balance_vectors(temp_vec1, temp_vec2);
    
    // Get magnitude
    float mag1 = vector_magnitude(temp_vec1);
    float mag2 = vector_magnitude(temp_vec2);
    
    // Get inner product i.e dot product
    float dot = std::inner_product(std::begin(temp_vec1), std::end(temp_vec1), std::begin(temp_vec2), 0.0);
    
    // Get cosine similarity between vectors
    float cos_sim = dot / (mag1 * mag2);
    if (cos_sim > 1.0) // Rare case where float representation causes cos_sim to be slightly larger than 1.0
        return 0.0;
    else // Normal return
        return std::acos(cos_sim) * 180 / M_PI;
}

float Feature::pythags(float& f1, float& f2)
{
    return sqrt(pow(f1, 2) + pow(f2, 2));
}

std::vector<Envelope*> Feature::get_vector_of_ptrs(std::vector<Envelope>* envelopes)
{
    std::vector<Envelope*> temp_vec;
    temp_vec.reserve(envelopes -> size());

    for (Envelope &e : *envelopes)
    {
        temp_vec.emplace_back(&e);
    }
    return temp_vec;
}

void Feature::build_edges(const int &rt_h, const int &mz_h, CellMap &cm)
{
    std::vector<Envelope*> core_envelopes = cm.get_envelopes(rt_h, mz_h);
    std::vector<std::vector<Envelope*>> neighbors = cm.get_neighborhood(rt_h, mz_h);
    
    while (!core_envelopes.empty())
    {
        Envelope *e1 = core_envelopes.back();
        core_envelopes.pop_back();

        for (Envelope *e2 : core_envelopes)
        {
            if (e1 -> run_id == e2 -> run_id)
                continue;
            
            float cos_sim = cosine_similarity(e1, e2);
            float mz_sim = distance(e1 -> monoisotopic_mz, e2 -> monoisotopic_mz);

            if (cos_sim <= cmds.ir_distance)
            {

                mz_sim = normalize(mz_sim, cmds.mz_distance);
                cos_sim = normalize(cos_sim, cmds.ir_distance);
                float weight = pythags(mz_sim, cos_sim);
//                print_relationship(e1, e2);
//                printf("\tweight: %f mz sim: %f cos sim: %f\n", weight, mz_sim, cos_sim);
                edges.back().emplace_back(std::make_unique<Edge>(weight, e1, e2));
            }
        }
        int index = 0;
        for (std::vector<Envelope*> v : neighbors)
        {
            for (Envelope *e2 : v)
            {
                if (e1 -> run_id == e2 -> run_id)
                    continue;
                if (distance(e1 -> monoisotopic_rt, e2 -> monoisotopic_rt) > cmds.rt_distance)
                    continue; // Since adjacent neighbors can be outside rt_distance, check here
                
                float cos_sim = cosine_similarity(e1, e2);
                float mz_sim = distance(e1 -> monoisotopic_mz, e2 -> monoisotopic_mz);
                
                if (cos_sim <= cmds.ir_distance && mz_sim <= cmds.mz_distance)
                {
                    mz_sim = normalize(mz_sim, cmds.mz_distance);
                    cos_sim = normalize(cos_sim, cmds.ir_distance);
                    float weight = pythags(mz_sim, cos_sim);
//                    print_relationship(e1, e2);
//                    printf("\tweight: %f mz sim: %f cos sim: %f\n", weight, mz_sim, cos_sim);
                    edges.back().emplace_back(std::make_unique<Edge>(weight, e1, e2));
                }
            }
            ++index;
        }
    }
}

bool Feature::valid_run_id_intersection(Cluster* c1, Cluster* c2)
{
    std::unordered_set<int> us1 = c1 -> get_run_ids();
    std::unordered_set<int> us2 = c2 -> get_run_ids();
    
    for (const int &key : us2)
    {
        std::unordered_set<int>::const_iterator loc = us1.find(key);
        
        if (loc != us1.end())
        {
            return false;
        }
    }
    return true;
}

bool Feature::added(Edge* e, std::vector<std::unique_ptr<Cluster>>& cs_clusters)
{
    // Both included
    // Sub case theyre already in the same cluster, return true, do nothing
    // Sub case theyre in different clusters
    // Check if the intersectin of run_ids is zero
    // If true
    // Consalidate
    // else
    // Do nothing
    if (e -> e1 -> cluster_id >= 0 && e -> e2 -> cluster_id >= 0)
    {
        if (e -> e1 -> cluster_id != e -> e2 -> cluster_id)
        {
            int new_id = e -> e1 -> cluster_id;
            Cluster *c1 = cs_clusters[e -> e1 -> cluster_id].get();
            Cluster *c2 = cs_clusters[e -> e2 -> cluster_id].get();

            if (valid_run_id_intersection(c1, c2))
            {
                e -> e2 -> cluster_id = new_id;
                c1 -> edges.emplace_back(e);
                for (auto &e : c2 -> edges)
                {
                    c1 -> edges.emplace_back(e);
                    e -> e1 -> cluster_id = new_id;
                    e -> e2 -> cluster_id = new_id;
                }
                c2 -> deleted = true;
            }
        }
        return true;
    }
    // One included
    else if (e -> e1 -> cluster_id >= 0 || e -> e2 -> cluster_id >= 0)
    {
        if (e -> e1 -> cluster_id >= 0)
        {
            Cluster *c = cs_clusters[e -> e1 -> cluster_id].get();
            if (c -> valid_run_id_add(e -> e2 -> run_id))
            {
                e -> e2 -> cluster_id = e ->  e1 -> cluster_id;
                c -> edges.emplace_back(e);
            }
        }
        else
        {
            Cluster *c = cs_clusters[e -> e2 -> cluster_id].get();
            if (c -> valid_run_id_add(e -> e1 -> run_id))
            {
                e -> e1 -> cluster_id = e -> e2 -> cluster_id;
                c -> edges.emplace_back(e);
            }
        }
        return true;
    }
    return false;
}

void Feature::finalize_cs_clusters(std::vector<std::unique_ptr<Cluster>>& cs_clusters)
{
    // I believe this functionality could be easily removed

    // Push new vector of clusters into clusters
    std::vector<std::unique_ptr<Cluster>> temp_clusters;
    clusters.emplace_back(std::move(temp_clusters));
    int new_id = 0;
    for (auto &cluster : cs_clusters)
    {
        if (!cluster -> deleted)
        {
            cluster -> id = new_id;
            clusters.back().emplace_back(std::move(cluster));
            ++new_id;
        }
    }
}


void Feature::perform()
{
    for (CellMap &cell_map : cell_maps)
    {
        std::vector<std::unique_ptr<Edge>> cs_edges;
        edges.emplace_back(std::move(cs_edges));

        for (const auto &um1 : cell_map.map)
        {
            int rt_h = um1.first;
            
            for (const auto &um2 : um1.second)
            {
                int mz_h = um2.first;
                build_edges(rt_h, mz_h, cell_map);
            }
        }

        // Sort vector of edges
        std::sort(edges.back().begin(), edges.back().end(), [](const std::unique_ptr<Edge>& e1, const std::unique_ptr<Edge>& e2) {return e1 -> weight < e2 -> weight;});

        // Init vector of clusters
        std::vector<std::unique_ptr<Cluster>> cs_clusters;
        // Can't immediately push cs_clusters because we have to reconsile soft-deleted clusters

        // Build clusters
        int id = 0;
        for (auto &e_ : edges.back())
        {
            Edge *e = e_.get();

            if (!added(e, cs_clusters))
            {
                std::unique_ptr<Cluster> c = std::make_unique<Cluster>(id);
                e -> e1 -> cluster_id = id;
                e -> e2 -> cluster_id = id;
                c -> edges.emplace_back(e);
                cs_clusters.emplace_back(std::move(c));
                ++id;
            }
        }

        // Reconsile soft-deleted clusters i.e clusters that were consalidated
        finalize_cs_clusters(cs_clusters);
    }
}
// End Alignment::Feature function implementations
}
