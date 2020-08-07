#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "msms_validation.hpp"

// Currently not used!!!

MSMSValidation::MSMSValidation(std::vector<std::vector<std::unique_ptr<Cluster>>>& clusters_) : clusters(clusters_) {}

bool MSMSValidation::valid_cluster(Cluster *cluster)
{
    
    // Cases
    // 1. No peptides: Doesn't meet criteria off all clusters MUST contain same ms/ms identification
    //
    
    // Return true if all unmodified
    
    // Calc number of runs, which equals c.edges.size() + 1
    int num_runs = (int)cluster -> edges.size() + 1;
    std::vector<Peptide> peptides;
    std::unordered_set<Envelope*> envelopes = cluster -> uniq_envelopes();
    
    for (Envelope *e : envelopes)
    {
        for (Peptide p : e -> peptides)
        {
            peptides.emplace_back(p);
        }
        // Could put guard clause here
    }
    if (peptides.empty())
        return true; // Guard clause for case 1, could be caught sooner in first loop
    while (!peptides.empty())
    {
        std::vector<Peptide> matches;
        Peptide ref_peptide = peptides.back();
        peptides.pop_back();
        matches.emplace_back(ref_peptide);

        for (Peptide &p : peptides)
        {
            if (ref_peptide.sequence == p.sequence && ref_peptide.mods == p.mods)
            {
                matches.emplace_back(p);
            }
        }

        if (matches.size() == num_runs)
        {
            printf("Found matching peptides!\n");
            return true;
        }

    }
    
    // Get unique set of copied peptide objects
    // while filtered.pop_back
    printf("Did not find matching peptides!\n");
    return false;
}

void MSMSValidation::perform()
{

    // Iterate of every charge state
    // Validate that all clusters contain only envelpes with the same ms/ms identification
    
    
    for (std::vector<std::unique_ptr<Cluster>> &cs_clusters : clusters)
    {
        for (auto &c_: cs_clusters)
        {
            Cluster *c = c_.get();
            if (!valid_cluster(c))
            {
                c -> deleted = true;
            }
        }
        // The deletion could be done much better and in one pass, instead of looping and deleting the objects marked as deleted
        cs_clusters.erase(std::remove_if(cs_clusters.begin(), cs_clusters.end(), [] (const std::unique_ptr<Cluster>& c) {return c -> deleted == true;}), cs_clusters.end());
    }
    

    
    printf("Finished MSMSValidation::perform\n");
}


