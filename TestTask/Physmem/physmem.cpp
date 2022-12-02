#include "physmem.h"

PhysMem::PhysMem(int size, Err::Error * error)
{   
    mem_size = size;
    mem = new uint8_t[size];
    err = error;
}

PhysMem::PhysMem()
{
    mem = {};
    err = nullptr;
}

uint32_t PhysMem::getMem(uint32_t mem_pointer, uint8_t nbytes)
{
    if (mem_pointer < 0 || mem_pointer > mem_size)
    {
        err->setMemErr(mem_pointer, mem_size);
    }

    if (nbytes == 4)
        return *((uint32_t*)(mem + mem_pointer));
    if (nbytes == 2)
        return *((uint16_t*)(mem + mem_pointer));
    if (nbytes == 1)
        return mem[mem_pointer];
    
    err->setMemErr(mem_pointer, mem_size);
    return 0;
}

void PhysMem::setMem(uint32_t mem_pointer, uint32_t value, uint8_t nbytes)
{
    if (mem_pointer < 0 || mem_pointer > mem_size - 1)
    {
        err->setMemErr(mem_pointer, mem_size);
    }
    
    if (nbytes == 4)
        *((uint32_t*)(mem + mem_pointer)) = value;
    
    else if (nbytes == 2)
        *((uint16_t*)(mem + mem_pointer)) = static_cast<uint16_t>(value);

    else if (nbytes == 1)
        mem[mem_pointer] = static_cast<uint8_t>(value);
    
    else
        err->setMemErr(mem_pointer, mem_size);
}