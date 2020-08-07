#ifndef correspondence_hpp
#define correspondence_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

#include "cluster.hpp"
#include "grid.hpp"

// Correspondence algorithm
class Correspondence {
    
    std::vector<std::vector<std::unique_ptr<Cluster>>> clusters;
    std::vector<std::vector<std::unique_ptr<Edge>>> edges;
    
public:
    float min_mz = std::numeric_limits<float>::max();
    float min_rt = std::numeric_limits<float>::max();
    float max_mz = 0;
    float max_rt = 0;
    int max_charge_state = 0;
    CMDParser &cmds;
    std::vector<CellMap> cell_maps;
    
    Correspondence(CMDParser& cmds_) : cmds(cmds_) {};
    
    void perform();

private:
    std::vector<std::string> get_files();
    int hash_envelope(const float& dist, const float& val);
    void insert_envelopes(std::vector<std::string> &files);
    void add_to_cell_maps(int &cur_cell_map_max);
    void write_as_json();
    void write_as_featureXML();
};
#endif
