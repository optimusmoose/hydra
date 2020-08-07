#ifndef grid_hpp
#define grid_hpp

#ifndef JUMP_DIRECTIONS
#define JUMP_DIRECTIONS

const int DIRECTIONS[8][2] = {
    {-1, 1},
    {0, 1},
    {1, 1},
    {1, 0},
    {-1,0},
    {-1,-1},
    {0,-1},
    {1,-1}
};

#endif

#include <vector>
#include <memory>

#include "envelope.hpp"
#include "cmd_parser.hpp"

// Grid cell within Grid containing Envelopes
class GridCell{
public:
    std::vector<std::unique_ptr<Envelope>> envelopes;
    GridCell();
};

typedef std::unordered_map<int, std::unordered_map<int, GridCell>> Map;

// Container for hash of hashes of envelopes
class CellMap
{
public:
    int charge_state;
    Map map;
    
    CellMap(const int cs_);
    void insert_envelope(const int &rt, const int &mz, std::unique_ptr<Envelope> &e);
    std::vector<std::vector<Envelope*>> get_neighborhood(const int &rt_h, const int &mz_h);
    std::vector<Envelope*> get_envelopes(const int &rt_h, const int &mz_h);
    int count_envelopes();
};

#endif
