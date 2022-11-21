/*
 LRU Cache for instructions. Linear block contains 1 instruction. For erasing will be choosed the instruction which
 has not been used for the longest time
*/

#include <map>
#include "error_.h"
#include "instructions.h"

class Cache
{
    private:
        std::map<uint32_t, std::pair<time_t, Instn::Instruction>> instn_map;
        int map_size;
        Err::Error *err;
        time_t cur_time;

        void removeOldestInstn();

    public:
        Cache() = default;
        Cache(const Cache &) = default;
        Cache(int size, Err::Error *error);
        ~Cache() = default;

        void addInstn(uint32_t instn, time_t time);

        bool accessInstn(uint32_t instn);

        // Only used on execution stage
        Instn::Instruction operator[](uint32_t instn);

        void updateInstn(uint32_t instn, time_t cur_time);
};