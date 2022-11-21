#include <libelf.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    int fd;
    
    Elf* elf;
    char* k;

    Elf_Kind ek;

    if (argc < 2)
    {
        fprintf(stderr, "usage: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (elf_version(EV_CURRENT) == EV_NONE)
    {
        fprintf(stderr, "cannot init libelf: version = %d\n", elf_version(EV_CURRENT));
        exit(EXIT_FAILURE);
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0)
    {
        perror("in open()");
        exit(EXIT_FAILURE);
    }

    if ((elf = elf_begin(fd, Elf_Cmd::ELF_C_READ, nullptr)) == nullptr)
    {
        const char * err = elf_errmsg(0);
        perror(err);
        exit(EXIT_FAILURE);
    }

    ek = elf_kind(elf);

    switch(ek)
    {
        case ELF_K_AR:
            k = "ar(1) archive";
            break;
        case ELF_K_ELF:
            k = "elf_obj";
            break;
        case ELF_K_NONE:
            k = "data";
            break;
        default:
            k = "unrecognized";
    }

    printf("%s: %s\n", argv[1], k);

    Elf32_Ehdr * ehdr = elf32_getehdr(elf);

    printf("entry: %X\n", ehdr->e_entry);

    

    elf_end(elf);
    close(fd);

    exit(EXIT_SUCCESS);
}