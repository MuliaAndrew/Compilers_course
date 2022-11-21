#ifndef ERROR_H
#define ERROR_H


#include <iostream>
#include <fstream>
#include <vector>
#include <string>


enum Flags
{
    F_SHORT_LOG     = 0x01,
    F_WIDE_LOG      = 0x02,
    F_LOG_STDOUT    = 0x04,
};


namespace Err
{
    class Error;

    enum ModeMask
    {
        SHORT_LOGGING,  // debug mode
        WIDE_LOGGIGN,   // debug mode 
        NO_LOG,         // release mode
    };

    enum ErrType
    {
        NOERR,      // no errors, set by default
        WRRX0,      // try to write to rx0 <--> reg[0]
        NOMEM,      // address to wrong mem
        NORX,       // address to unexicting register
        UNDEFINSTN, // undefined instruction (should be added)
    };

    class Error
    {
        private:
            ErrType     err_type;
            uint32_t    val;        // in case NOMEM means capacity of allocated memory; in case UNDEFINSTN means func bytecode
            uint32_t    help_val;   // in case NOMEM means the offset from mem[0]; in case UNDEFINSTN means func opcode bytecode
            uint32_t    pc;
            ModeMask    mode;
            std::string log_fname;
            int         flags;

            void log();
            void wideLog();
            void printErr();

        public:
            Error(int flags, std::string fname = "log.txt");
            Error();

            void setRegErr(uint32_t reg);
            void setRx0Err();
            void setMemErr(uint32_t offset, int memsize);
            void setInstnErr(uint32_t instn, uint32_t opcode);
    };
}

#endif // ERROR_H