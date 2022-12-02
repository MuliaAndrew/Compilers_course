#ifndef PHYSMEM_H
#define PHYSMEM_H

#include <vector>
#include <string>
#include <cstdint>
#include "../ErrorHandler/error_.h"

class PhysMem
{
    private:
        uint8_t*   mem;
        uint32_t    mem_size;

        Err::Error  * err;

    public:
        PhysMem();
        PhysMem(int size, Err::Error * error);

        uint32_t getMem(uint32_t mem_pointer, uint8_t nbytes);
        void    setMem(uint32_t mem_pointer, uint32_t value, uint8_t nbytes);
};

#endif // PHYSMEM_H