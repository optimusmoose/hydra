#ifndef envelope_hpp
#define envelope_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "json.hpp"
#include "peptide.hpp"

class Envelope{
    
public:
    int cluster_id = -1;
    std::vector<Peptide> peptides;
    std::string id;
    int run_id;
    float monoisotopic_mz;
    float monoisotopic_rt;
    float min_rt;
    float max_rt;
    float min_mz;
    float max_mz;
    int charge_state;
    float total_abundance;
    std::vector<float> isotope_ratios;

    Envelope(nlohmann::json& envelope, int run_id_, nlohmann::json& id_, nlohmann::json& matches);
    bool operator==(const Envelope& rhs) const;
    nlohmann::json as_json();
    std::string to_string();
};

#endif
