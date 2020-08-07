#ifndef writer_hpp
#define writer_hpp

#include <stdio.h>
#include <vector>

#include "rapidXML/rapidxml.hpp"
#include "rapidXML/rapidxml_ext.hpp"
#include "rapidXML/rapidxml_utils.hpp"
#include "rapidXML/rapidxml_print.hpp"

#include "cmd_parser.hpp"
#include "cluster.hpp"

namespace Writer
{

// Base class for XML
class XML
{
protected:
    void write_decl(rapidxml::xml_document<> &doc);
    std::string generate_id(std::string prefix);
};

class FeatureXML : XML
{
public:
    CMDParser &cmds;
    FeatureXML(CMDParser &cmds_) : cmds(cmds_) {};

    void write_clusters_to_consensusXML(std::vector<std::vector<std::unique_ptr<Cluster>>> &clusters);
private:
    void add_consensus_element(rapidxml::xml_document<> &doc, rapidxml::xml_node<> *consensus_element_list, std::unordered_set<Envelope *> &envelopes_);
    void add_consensusXML_element(rapidxml::xml_document<> &doc, rapidxml::xml_node<> *consensusXML);
    void add_map_list_element(rapidxml::xml_document<> &doc, rapidxml::xml_node<> *consensusXML, rapidxml::xml_node<> *map_list_element);
};
}

#endif /* writer_hpp */
