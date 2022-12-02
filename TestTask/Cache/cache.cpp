#include "cache.h"

#define LONG_MAX __LONG_MAX__


Cache::Cache(int size, Err::Error * error)
{
    instn_map = {};
    map_size = size;
    err = error;
    cur_time = 0;
}

void Cache::removeOldestInstn()
{
    auto iter = instn_map.begin();

    time_t min_time = LONG_MAX;   
    auto iter_to_remove = iter;
    
    for (; iter != instn_map.end(); iter++)
    {
        if (iter->second.first < min_time)
        {
            min_time = iter->second.first;
            iter_to_remove = iter;
        }
    }

    instn_map.erase(iter_to_remove);
}

void Cache::addInstn(uint32_t instn, time_t time)
{
    if (map_size == instn_map.size())
        removeOldestInstn();
    
    instn_map.insert(std::pair{instn, std::pair{time, Instn::Instruction(instn, err)}});
}

Instn::Instruction Cache::operator[](uint32_t instn)
{
    auto iter = instn_map[instn];

    iter.first = cur_time;
    cur_time++;
    
    return iter.second;
}

bool Cache::accessInstn(uint32_t instn)
{
    return instn_map.count(instn);
}