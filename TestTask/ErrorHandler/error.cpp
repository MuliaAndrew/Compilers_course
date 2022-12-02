#include "error_.h"

const char * errTypeName[] = {"E_NOERR", "E_NORX", "E_WRRX0", "E_NOMEM"};


#define SWITCH_MODE                             \
        switch(mode)                            \
            {                                   \
                case ModeMask::SHORT_LOGGING:   \
                    log();                      \
                    break;                      \
                case ModeMask::WIDE_LOGGIGN:    \
                    wideLog();                  \
            }

void Err::Error::log()
{
    std::ofstream logfile;

    if (flags & F_LOG_STDOUT)
        #define logfile std::cout

    logfile << "Error while running" << "\npc=" << pc
    << ", error type: " << errTypeName[err_type] << ", \n";

    switch (err_type)
    {   
        case ErrType::WRRX0:
            logfile << " :trying to write in rx0" << ".\n\n";
            break;

        case ErrType::NORX:
            logfile << " :trying to write to unexicting register rx" << val << ".\n\n";
        
        case ErrType::NOMEM:
            logfile << " :trying to address mem[ " << help_val <<" ] where mem have capacity " << val << ".\n\n";
            break;

        case ErrType::UNDEFINSTN:
            logfile << " :undefined instruction 0x" << val << " with opcode 0x" << help_val << ".\n\n";
    }

    if (flags & F_LOG_STDOUT)
        #undef logfile

    logfile.close();

    exit(EXIT_FAILURE);
}


void Err::Error::wideLog()
{
    // to add
    exit(EXIT_FAILURE);
}


void Err::Error::printErr()
{
    std::cerr << "aborted" << err_type;
    exit(EXIT_FAILURE);
}


Err::Error::Error(int flags, std::string fname)
{
    if (flags & Flags::F_SHORT_LOG || flags & Flags::F_LOG_STDOUT)
        mode = ModeMask::SHORT_LOGGING;
    
    else if (flags & Flags::F_WIDE_LOG)
        mode = ModeMask::WIDE_LOGGIGN;
    
    else
        mode = ModeMask::NO_LOG;
    
    err_type = ErrType::NOERR;
    log_fname = fname;
}


Err::Error::Error()
{
    err_type = ErrType::NOERR;
    mode = ModeMask::NO_LOG;
}


void Err::Error::setRegErr(uint32_t reg)
{
    err_type = ErrType::NORX;
    val = reg;

    SWITCH_MODE;

    printErr();
}


void Err::Error::setRx0Err()
{
    err_type = ErrType::WRRX0;

    SWITCH_MODE;

    printErr();
}


void Err::Error::setMemErr(uint32_t offset, int memsize)
{
    err_type = ErrType::NOMEM;
    val = memsize;
    help_val = offset;

    SWITCH_MODE;

    printErr();
}


void Err::Error::setInstnErr(uint32_t instn, uint32_t opcode)
{
    err_type = ErrType::UNDEFINSTN;
    val = instn;
    help_val = opcode;

    SWITCH_MODE;

    printErr();
}

#undef SWITCH_MODE