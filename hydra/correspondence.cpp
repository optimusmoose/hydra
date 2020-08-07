#include <filesystem>
#include <regex>
#include <fstream>
#include <unordered_set>

#include "json.hpp"
#include "cmd_parser.hpp"
#include "correspondence.hpp"
#include "alignment.hpp"
#include "msms_validation.hpp"
#include "writer.hpp"

// Correspondence function implementations
int Correspondence::hash_envelope(const float &dist, const float &val)
{
    // Basic hashing function for looking in hash table
    return floor( (val) / dist );
}

void Correspondence::add_to_cell_maps(int &cur_cell_map_max)
{
    // Adds CellMap (for a certain charge state) to cell maps, fills from current highest CS to new highest CS
    auto cell_maps_size = cell_maps.size();
    auto diff = cur_cell_map_max - cell_maps_size;
    
    for (int i = 0; i <= diff; ++i)
    {
        cell_maps.emplace_back(CellMap(cell_maps_size + i + 1));
    }
}

void Correspondence::insert_envelopes(std::vector<std::string> &files)
{
    // Parse envelopes from files
    
    namespace fs = std::filesystem;
    using json = nlohmann::json;
    
    int run_id = 0;
    int envelopes_count = 0;
    int cur_cell_map_max = -1;
    
    if (!cmds.cs_enforced)
    {
        ++cur_cell_map_max;
        add_to_cell_maps(cur_cell_map_max);
    }
    
    for (std::string &s : files)
    {

        std::cout << "\t" << " Parsing " << s << "\n";
        
        // Catch for file doesn't exist
        if (!fs::exists(s))
        {
            std::cout << s << "\n" << "File doesn't exist" << "\n";
            exit(0);
        }

        // Try-Catch for malformed JSON
        try
        {
            std::ifstream f (s);
            json j;
            f >> j;

            for (auto &element : j.at("envelopes"))
            {
                // JSON objects
                auto &envelope = element.at("envelope");
                auto &id = element.at("id");
                auto &matches = element.at("matches");
                
                // Build envelope
                std::unique_ptr<Envelope> e = std::make_unique<Envelope>(envelope, run_id, id, matches);
        
                // Hash m/z and RT
                int rt_h = hash_envelope(cmds.rt_distance, e -> monoisotopic_rt);
                int mz_h = hash_envelope(cmds.mz_distance, e -> monoisotopic_mz);
            
                // Insert in CellMap based on charge state if applicable
                if (cmds.cs_enforced) // CS matters
                {
                    if (e -> charge_state - 1 > cur_cell_map_max)
                    {
                        cur_cell_map_max = e -> charge_state - 1;
                        add_to_cell_maps(cur_cell_map_max);
                        cell_maps[e -> charge_state - 1].insert_envelope(rt_h, mz_h, e);
                    }
                    else
                    {
                        cell_maps[e -> charge_state - 1].insert_envelope(rt_h, mz_h, e);
                    }
                }
                else // CS doesn't matter
                {
                    cell_maps[0].insert_envelope(rt_h, mz_h, e);
                }
                ++envelopes_count;
            }
            ++run_id;
        }
        catch (nlohmann::detail::parse_error &e)
        {
            std::cout << "\n" << "Error" << "\n\n";
            std::cerr << e.what() << "\n";
            exit(0);
        }

    }
    printf("Number of features: %i\n", envelopes_count);
}

void Correspondence::write_as_json()
{
    using namespace nlohmann;
    
    json j;
    std::vector<std::vector<json>> clusters_json;
    int count = 0;
    
    // Build JSON objects and insert into vector
    for (auto& cs_clusters : clusters)
    {
        // clusters for a charge state
        for (auto &c : cs_clusters)
        {
            std::vector<json> cluster_json;
            std::unordered_set<Envelope*> envelopes = c -> uniq_envelopes();
            // envelopes for a cluster within in a charges state
            for (auto &e : envelopes)
            {
                cluster_json.emplace_back(e -> as_json());
            }
            clusters_json.emplace_back(cluster_json);
            ++count;
        }
    }
    
    // Write JSON
    j = clusters_json;
    std::ofstream f;

    f << j.dump(4) << "\n";
    f.close();
    
    printf("Number of clusters found: %i\n", count);

}

void Correspondence::perform(){
    
    // Get list of files
    std::vector<std::string> files;
    files = cmds.files; // Quick fix since dir search removed.
    
    // Guard clause for no files specified or found
    if (files.empty())
    {
        puts("No files provided");
        return;
    }
    else if (files.size() == 1)
    {
        puts("Only one file found matching pattern .json$");
        return;
    }
    
    // Sort list of files lexicographically
    std::sort(files.begin(), files.end());
    
    // Print out files to be processed
    puts("Processing:");
    int index = 0;
    for (std::string &s : files)
    {
        std::cout << "\t" << "Run ID: " << index << ", " << s << "\n";
        ++index;
    }

    // Insert envelopes into respective CellMap
    puts("Inserting features");
    insert_envelopes(files);

    // Decide what alignment method to use, RT alignment removed for now
    puts("Running Correspondence");

    Alignment::Feature alignment = Alignment::Feature(cell_maps, cmds, clusters, edges);
    alignment.perform();

    // Count clusters, print size
    int size = 0;
    for (auto &cs_clusters : clusters)
    {
        size += cs_clusters.size();
    }

    printf("Found %i clusters\n", size);

    // Perform ms/ms validation if desired, removed for now

//    if (cmds.validate)
//    {
//        MSMSValidation validation = MSMSValidation(clusters);
//        validation.perform();
//    }
    
    // Write json output
//    write_as_json();
    
    // Write out consensusXML
    Writer::FeatureXML writer = Writer::FeatureXML(cmds);
    writer.write_clusters_to_consensusXML(clusters);
}
// End Correspondence function implementations
