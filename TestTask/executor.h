#include "elf_parser.h"
#include "error_.h"
#include "instructions.h"
#include "physmem.h"
#include "cache.h"


class Executor
{
    private:
        PhysMem     ph_mem;
        Cache       cache;
        Err::Error* err;
        uint32_t    pc;
        uint32_t    start_pc;

        std::vector<uint32_t> code;

        void execADDI(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execSLTI(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execSLTIU(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execANDI(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execORI(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execXORI(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execSLLI(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execSRLI(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execSRAI(uint8_t rd, uint8_t rs1, uint32_t imm);

        void execLUI(uint8_t rd, uint32_t);
        void execAUIPC(uint8_t rd, uint32_t);

        void execADD(uint8_t rs1, uint8_t rs2, uint8_t rd);
        void execSUB(uint8_t rs1, uint8_t rs2, uint8_t rd);
        void execSLT(uint8_t rs1, uint8_t rs2, uint8_t rd);
        void execSLTU(uint8_t rs1, uint8_t rs2, uint8_t rd);
        void execAND(uint8_t rs1, uint8_t rs2, uint8_t rd);
        void execOR(uint8_t rs1, uint8_t rs2, uint8_t rd);
        void execXOR(uint8_t rs1, uint8_t rs2, uint8_t rd);
        void execSLL(uint8_t rs1, uint8_t rs2, uint8_t rd);
        void execSRL(uint8_t rs1, uint8_t rs2, uint8_t rd);
        void execSRA(uint8_t rs1, uint8_t rs2, uint8_t rd);

        void execJAL(uint8_t rd, uint32_t imm);
        void execJALR(uint8_t rd, uint8_t rs1, uint32_t imm);

        void execBEQ(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm);
        void execBNE(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm);
        void execBLT(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm);
        void execBLTU(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm);
        void execBGE(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm);
        void execBGEU(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm);

        void execLB(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execLH(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execLW(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execLBU(uint8_t rd, uint8_t rs1, uint32_t imm);
        void execLHU(uint8_t rd, uint8_t rs1, uint32_t imm);

        void execSB(uint8_t rs1, uint8_t rs2, uint32_t imm);
        void execSH(uint8_t rs1, uint8_t rs2, uint32_t imm);
        void execSW(uint8_t rs1, uint8_t rs2, uint32_t imm);

    public:
        Executor(std::string fname, Err::Error * error);

        void execute(uint32_t iter);
        void proccesor();
        void getCode(std::string fname);
        void getCode(std::vector<uint32_t> arr);
        void getCode(std::vector<std::string> s_arr);
        void getCodeFromElf(std::string fname);
        void instnDump(Instn::InstnKey kinstn, uint8_t rs1, uint8_t rs2, uint8_t rd, uint32_t imm);
        void regDump();
        void codeDump();
};
