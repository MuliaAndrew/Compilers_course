#include "elf_parser.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <libelf.h>


Elf_ret_t elf_parse(std::string fname)
{
    int fd;

    Elf* elf;
    char* k;

    Elf_Kind ek;

    if (elf_version(EV_CURRENT) == EV_NONE)
    {
        std::cerr << "libelf init error\n";
        exit(EXIT_FAILURE);
    }

    if ((fd = open(fname.c_str(), O_RDONLY)) < 0)
    {
        std::cerr << "cannot open elf: " << fname << "\n";
        exit(EXIT_FAILURE);
    }

    if ((elf = elf_begin(fd, Elf_Cmd::ELF_C_READ, nullptr)) == nullptr)
    {
        std::cerr << "cannot read elf_file: " << fname << "\n";
        exit(EXIT_FAILURE);
    }
    
    ek = elf_kind(elf);

    if (ek != ELF_K_ELF)
    {
        std::cerr << "Not an elf object file\n";
        exit(EXIT_FAILURE);
    }

    Elf32_Ehdr * ehdr = elf32_getehdr(elf);

    uint32_t entry = ehdr->e_entry;

    size_t section_header_str_index;

    if (elf_getshstrndx(elf, &section_header_str_index) != 0)
    {
        std::cerr << "libelf error\n";
        exit(EXIT_FAILURE);
    }

    // Elf_Scn * section;
    // Elf32_Shdr * s_header;
    // char * name;
    
    std::vector<uint32_t> code;

    // while ((section = elf_nextscn(elf, section)) != nullptr)
    // {
    //     if (!(s_header = elf32_getshdr(section)))
    //     {
    //         std::cerr << "libelf error\n";
    //         exit(EXIT_FAILURE);
    //     }

    //     if ((name = elf_strptr(elf, section_header_str_index, s_header->sh_name)) == nullptr)
    //     {
    //         std::cerr << "libelf error\n";
    //         exit(EXIT_FAILURE);
    //     }

    //     if (std::string(name) != ".text")
    //         continue;
        
    //     auto code_size = s_header->sh_size;

    //     code_size
    // }

    struct stat fd_stat;
    fstat(fd, &fd_stat);

    auto code_size = (fd_stat.st_size + 3) / 4;
    code.resize(code_size);

    auto code_p = code.data();

    int n_readed = read(fd, (uint8_t*)code_p, code_size);

    if (errno)
    {
        std::cerr << "in read()\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Num of readed bytes from " << fname << " : " << n_readed;

    elf_end(elf);

    close(fd);

    std::cout << "[ELF PARSER] //////////////////////////////////////////////\n\n";

    for (auto i = entry; i < code.size(); i++)
        std::cout << std::hex << code[i] << "\n";

    Elf_ret_t ret = {code, entry};

    return ret;
}