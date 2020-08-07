#include "grid.hpp"
#include <cmath>

// GridCell
GridCell::GridCell(){};
// End GridCell

// CellMap
CellMap::CellMap(int cs_)
{
    charge_state = cs_;
}
void CellMap::insert_envelope(const int &rt, const int &mz, std::unique_ptr<Envelope> &e)
{
    if (map.find(rt) != map.end()) // Inner map exists
    {
//        puts("inner map exists");
        if (map[rt].find(mz) != map[rt].end()) // Bin exists
        {
//            puts("bin exists");
            map[rt][mz].envelopes.emplace_back(std::move(e));
        }
        else // Bin doesn't exist
        {
//            puts("bin doesn't exist");
            GridCell g = GridCell();
            g.envelopes.emplace_back(std::move(e));
            map[rt].insert(std::make_pair(mz, std::move(g)));
        }
    }
    else // Inner map doesn't exist
    {
//        puts("inner map doesn't exist");
        std::unordered_map<int, GridCell> inner_map;

        GridCell g = GridCell();
        g.envelopes.emplace_back(std::move(e));
        inner_map.insert(std::make_pair(mz, std::move(g)));
        map.insert(std::make_pair(rt, std::move(inner_map)));
    }
}
std::vector<std::vector<Envelope*>> CellMap::get_neighborhood(const int &rt_h, const int &mz_h)
{
    std::vector<std::vector<Envelope*>> temp_envelopes;

    for (int i = 0; i < 4; ++i)
    {

        int temp_mz_h = mz_h + DIRECTIONS[i][1];
        int temp_rt_h = rt_h + DIRECTIONS[i][0];

        if (temp_rt_h < 0 || temp_mz_h < 0)
        {
            continue;
        }
        
        if (map.find(temp_rt_h) != map.end()) // Inner map exists
        {
            if (map[temp_rt_h].find(temp_mz_h) != map[temp_rt_h].end()) // Bin exists
            {
                temp_envelopes.emplace_back(get_envelopes(temp_rt_h, temp_mz_h));
            }
        }
    }
    return temp_envelopes;
}

std::vector<Envelope*> CellMap::get_envelopes(const int &rt_h, const int &mz_h)
{
    std::vector<Envelope*> temp_envelopes;

    for (auto &envelope : map[rt_h][mz_h].envelopes)
    {
        temp_envelopes.emplace_back(envelope.get());
    }
    
    return temp_envelopes;
}

int CellMap::count_envelopes()
{
    int count = 0;
    
    for (const auto &um1 : map)
    {
        for (const auto &um2 : um1.second)
        {
            count += um2.second.envelopes.size();
        }
    }
    return count;
}
// End CellMap
