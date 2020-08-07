#ifndef peptide_hpp
#define peptide_hpp

#include "json.hpp"

#include <iostream>
#include <vector>

class Peptide{
    public:
    
    std::string sequence;
    std::vector<float> mods;

    Peptide(std::string s, nlohmann::json& masses);
    Peptide(nlohmann::json& peptide);

};

#endif
