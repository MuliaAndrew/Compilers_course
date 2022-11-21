#include "physmem.h"

PhysMem::PhysMem(int size, Err::Error * error)
{
    for (int i = 0; i < 32; i++)
        reg[i] = 0;

    reg[2] = size - 1024;
    
    mem_size = size;
    mem = new uint8_t[size];
    err = error;
}

PhysMem::PhysMem()
{
    for (int i = 0; i < 32; i++)
        reg[i] = 0;

    mem = {};
    err = nullptr;
}

uint32_t PhysMem::getRx(uint32_t index)
{    
    if (index > 31 || index < 0)
        err->setRegErr(index);
    
    return reg[index];
}

void PhysMem::setRx(uint32_t index, uint32_t value)
{   
    if (index < 0 || index > 31)
        err->setRegErr(index);

    reg[index] = value;

    reg[0] = 0;
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