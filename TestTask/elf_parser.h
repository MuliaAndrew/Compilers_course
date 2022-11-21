#include <libelf.h>
#include "error_.h"

struct Elf_ret_t 
{
    std::vector<uint32_t> code;
    uint32_t entry;
};


Elf_ret_t elf_parse(std::string fname);