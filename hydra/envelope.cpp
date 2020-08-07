#include <fstream>

#include "envelope.hpp"
#include "peptide.hpp"

Envelope::Envelope(nlohmann::json& j, int run_id_, nlohmann::json& id_, nlohmann::json& matches)
{
    id = id_;
    run_id = run_id_;
    monoisotopic_mz = j["monoisotopicMZ"];
    monoisotopic_rt = j["monoisotopicRT"];
    min_rt = j["minRT"];
    max_rt = j["maxRT"];
    min_mz = j["minMZ"];
    max_mz = j["maxMZ"];
    charge_state = j["chargeState"];
    total_abundance = j["totalAbundance"];
    
    for (float f : j["isotopicRatios"])
    {
        isotope_ratios.emplace_back(f);
    }
    
    for (auto peptide : matches)
    {
        Peptide p = Peptide(peptide);
        peptides.emplace_back(p);
    }
}

std::string Envelope::to_string()
{
    return "mono_mz: " + std::to_string(monoisotopic_mz) + ", mono_rt: " + std::to_string(monoisotopic_rt);
}

bool Envelope::operator==(const Envelope& rhs) const
{
    if (id != rhs.id)
        return false;
    if (monoisotopic_mz != rhs.monoisotopic_mz)
        return false;
    if (monoisotopic_rt != rhs.monoisotopic_rt)
        return false;
    if (charge_state != rhs.charge_state)
        return false;
    if (total_abundance != rhs.total_abundance)
        return false;
    if (isotope_ratios != rhs.isotope_ratios)
        return false;
    return true;
}

nlohmann::json Envelope::as_json()
{
    nlohmann::json j;
    j["monoisotopicMZ"] = monoisotopic_mz;
    j["monoisotopicRT"] = monoisotopic_rt;
    j["minRT"] = min_rt;
    j["maxRT"] = max_rt;
    j["minMZ"] = min_mz;
    j["maxMZ"] = max_mz;
    j["chargeState"] = charge_state;
    j["runID"] = run_id;
    j["id"] = id;
    j["totalAbundance"] = total_abundance;
    j["isotopicRatios"] = isotope_ratios;
    return j;
}
