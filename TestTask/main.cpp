#include "executor.h"
#include <getopt.h>
#include <errno.h>


int getFlags(int argc, char **argv);


int main(int argc, char **argv)
{
    int flags = getFlags(argc, argv);

    auto error = Err::Error(flags);

    Executor executer{argv[optind], &error};

    executer.proccesor();

    return 0;
}


int getFlags(int argc, char **argv)
{
    const char short_flags[] = "lwO";

    const option long_flags[] = {
        {"log", no_argument, nullptr, 'l'},
        {"wide-log", no_argument, nullptr, 'w'},
        {"Log-to-stdout", no_argument, nullptr, 'O'}
    };

    char cur_flag;
    int flags;

    while ((cur_flag = getopt_long(argc, argv, short_flags, long_flags, nullptr)) != -1)
    {
        switch (cur_flag)
        {
        case 'l':
            flags = flags | Flags::F_SHORT_LOG;
            break;
        case 'w':
            flags = flags | Flags::F_WIDE_LOG;
            break;
        case 'O':
            flags = flags | Flags::F_LOG_STDOUT;
            break;
        case '?':
            errno = EINVAL;
            perror("Undefined flag");
            exit(EXIT_FAILURE);
        default:
            perror("in getopt()");
            exit(EXIT_FAILURE);
        }
    }

    return flags;
}