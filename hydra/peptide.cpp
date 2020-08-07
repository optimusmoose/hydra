#include "peptide.hpp"

Peptide::Peptide(std::string sequence_, nlohmann::json& masses){
    sequence = sequence_;
    
    for (float f : masses["masses"])
    {
        mods.emplace_back(f);
    }
    
};

Peptide::Peptide(nlohmann::json& peptide){
    sequence = peptide["sequence"];
    
    for (float f : peptide["masses"])
    {
        mods.emplace_back(f);
    }
    
};
