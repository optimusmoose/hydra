#include <unordered_set>
#include <vector>
#include <climits>

#include "writer.hpp"

#include "rapidXML/rapidxml.hpp"
#include "rapidXML/rapidxml_ext.hpp"
#include "rapidXML/rapidxml_utils.hpp"
#include "rapidXML/rapidxml_print.hpp"

namespace Writer
{

// XML function definitions

void XML::write_decl(rapidxml::xml_document<> &doc)
{

    using namespace rapidxml;
    
    xml_node<> *decl = doc.allocate_node(node_declaration);
    decl -> append_attribute(doc.allocate_attribute("version", "1.0"));
    decl -> append_attribute(doc.allocate_attribute("encoding", "utf-8"));
    doc.append_node(decl);
}

std::string XML::generate_id(std::string prefix)
{
    // Generates random integer id joined with '_' and appended to prefex
    unsigned int numeric_id = rand() % UINT_MAX;
    std::string id = prefix + "_" + std::to_string(numeric_id);
    return id;
}

// End XML function definitions

// FeatureXML function definitions

void FeatureXML::add_consensus_element(rapidxml::xml_document<> &doc, rapidxml::xml_node<> *consensus_element_list, std::unordered_set<Envelope *> &envelopes_)
{
    
    using namespace rapidxml;
    
    // Copy unordered_set to vector for ease of use
    std::vector<Envelope*> envelopes;
    envelopes.reserve(envelopes_.size());
    envelopes.insert(envelopes.end(), envelopes_.begin(), envelopes_.end());
    
    // Instantiate consensusElement element, add attributes
    xml_node<> *consensus_element = doc.allocate_node(node_element, "consensusElement");
    
    std::string id_ = generate_id("e");
    const char *id = doc.allocate_string(id_.c_str());
    consensus_element -> append_attribute(doc.allocate_attribute("id", id));
    
    const char *quality = doc.allocate_string("0.0");
    consensus_element -> append_attribute(doc.allocate_attribute("quality", doc.allocate_string(quality)));
    
    int charge_state_ = envelopes[0] -> charge_state;
    const char *charge_state = doc.allocate_string(std::to_string(charge_state_).c_str());
    consensus_element -> append_attribute(doc.allocate_attribute("charge", charge_state));
    // removed double string allocation
    
    // Instantiate centroid element and add attributes
    xml_node<> *centroid = doc.allocate_node(node_element, "centroid");
    
    float mz_avg = 0;
    float rt_avg = 0;
    float intensity = 0;
    for (Envelope *e : envelopes)
    {
        mz_avg += e -> monoisotopic_mz;
        rt_avg += e -> monoisotopic_rt;
        intensity += e -> total_abundance;
    }
    
    mz_avg /= envelopes.size();
    rt_avg /= envelopes.size();
    
    const char *mz_avg_s = doc.allocate_string(std::to_string(mz_avg).c_str());
    const char *rt_avg_s = doc.allocate_string(std::to_string(rt_avg).c_str());
    const char *intensity_s = doc.allocate_string(std::to_string(intensity).c_str());
    
    centroid -> append_attribute(doc.allocate_attribute("rt", rt_avg_s));
    centroid -> append_attribute(doc.allocate_attribute("mz", mz_avg_s));
    centroid -> append_attribute(doc.allocate_attribute("it", intensity_s));
    
    // Instantiate groupedElementList
    xml_node<> *grouped_element_list = doc.allocate_node(node_element, "groupedElementList");
    

    // Calculate and insert values for groupedElementList
    
    for (Envelope *e : envelopes)
    {
        xml_node<> *element = doc.allocate_node(node_element, "element");
        
        const char *map = doc.allocate_string(std::to_string(e -> run_id).c_str());
        std::string temp_id = e -> id;
        const char *id = doc.allocate_string(temp_id.c_str());
        const char *rt = doc.allocate_string(std::to_string(e -> monoisotopic_rt).c_str());
        const char *mz = doc.allocate_string(std::to_string(e -> monoisotopic_mz).c_str());
        const char *it = doc.allocate_string(std::to_string(e -> total_abundance).c_str());
        const char *charge_state = doc.allocate_string(std::to_string(e -> charge_state).c_str());
        
        element -> append_attribute(doc.allocate_attribute("map", map));
        element -> append_attribute(doc.allocate_attribute("id", id));
        element -> append_attribute(doc.allocate_attribute("rt", rt));
        element -> append_attribute(doc.allocate_attribute("mz", mz));
        element -> append_attribute(doc.allocate_attribute("it", it));
        element -> append_attribute(doc.allocate_attribute("charge", charge_state));
        
        grouped_element_list -> append_node(element);
        
    }
    
    // Append nodes
    consensus_element -> append_node(centroid);
    consensus_element -> append_node(grouped_element_list);
    consensus_element_list -> append_node(consensus_element);
    
    
}

void FeatureXML::add_consensusXML_element(rapidxml::xml_document<> &doc, rapidxml::xml_node<> *consensusXML)
{
    using namespace rapidxml;
    
    std::string id_ = generate_id("cm");
    
    const char *id = doc.allocate_string(id_.c_str());
    
    consensusXML -> append_attribute(doc.allocate_attribute("versions", doc.allocate_string("1.7")));
    consensusXML -> append_attribute(doc.allocate_attribute("id", id));
    
}

void FeatureXML::add_map_list_element(rapidxml::xml_document<> &doc, rapidxml::xml_node<> *consensusXML, rapidxml::xml_node<> *map_list_element)
{
    using namespace rapidxml;
    
    map_list_element -> append_attribute(doc.allocate_attribute("count", doc.allocate_string(std::to_string(cmds.files.size()).c_str())));
    
    int id = 0;
    for (std::string &s : cmds.files)
    {
        xml_node<> *map = doc.allocate_node(node_element, "map");
        
        map -> append_attribute(doc.allocate_attribute("id", doc.allocate_string(std::to_string(id).c_str())));
        const char *name = s.c_str();
        map -> append_attribute(doc.allocate_attribute("name", doc.allocate_string(name)));
        
        ++id;
        
        map_list_element -> append_node(map);
    }
}

void FeatureXML::write_clusters_to_consensusXML(std::vector<std::vector<std::unique_ptr<Cluster>>> &clusters)
{
    
    using namespace rapidxml;
    
    xml_document<> doc;
    
    // Write top-level xml declarations
    write_decl(doc);
    
    // Add consensusXML element
    xml_node<> *consensusXML = doc.allocate_node(node_element, "consensusXML");
    add_consensusXML_element(doc, consensusXML);
    
    // Add mapList element
    xml_node<> *map_element_list = doc.allocate_node(node_element, "mapList");
    add_map_list_element(doc, consensusXML, map_element_list);
    
    // Append mapList to consensusXML
    consensusXML -> append_node(map_element_list);
    
    // Add consensusElementList element
    xml_node<> *consensus_element_list = doc.allocate_node(node_element, "consensusElementList");

    consensusXML -> append_node(consensus_element_list);
    
    for (auto &cs_cluster : clusters)
    {
        for (auto &cluster : cs_cluster)
        {
            std::unordered_set<Envelope*> envelopes = cluster -> uniq_envelopes();
            add_consensus_element(doc, consensus_element_list, envelopes);
        }
    }
    
    // Append consensusElementList to doc
    doc.append_node(consensusXML);
    
//    std::cout << doc;
    std::string outfile_name =  cmds.outfile + ".consensusXML";
    std::cout << "Writing output to " << outfile_name << "\n";
    std::string doc_as_string;
    std::ofstream f;

    print(std::back_inserter(doc_as_string), doc);
    f.open(outfile_name);
    f << doc_as_string;
    
    f.close();
    doc.clear();
    
}
// End FeatureXML function definitions

}
