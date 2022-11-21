#include "executor.h"
#include "stdlib.h"


extern const char* instnName[];


#define CASEI(KEY) case Instn::InstnKey::k##KEY:                                            \
                    exec##KEY(cur_instn.getRd(), cur_instn.getRs1(), cur_instn.getImm());   \
                    break


#define CASER(KEY) case Instn::InstnKey::k##KEY:                                            \
                    exec##KEY(cur_instn.getRs1(), cur_instn.getRs2(), cur_instn.getRd());   \
                    break

#define CASEB(KEY) case Instn::InstnKey::k##KEY:                                            \
                    exec##KEY(cur_instn.getRd(), cur_instn.getRs1(), cur_instn.getRs2(), cur_instn.getImm()); \
                    break


#define CASES(KEY) case Instn::InstnKey::k##KEY:                                            \
                    exec##KEY(cur_instn.getRs1(), cur_instn.getRs2(), cur_instn.getImm());  \
                    break


const int CACHE_SIZE = 1 << 20;
const int PHYSMEM_SZIE = 1 << 20;


void Executor::getCode(std::string fname)
{
    std::ifstream fin{fname};

    if (!fin.is_open())
    {
        std::cerr << "Bad input file name\n";
        exit(EXIT_FAILURE);
    }

    std::string buf;

    fin >> buf;
    sscanf(buf.c_str(), "%x", &start_pc);

    while(fin)
    {
        fin >> buf;
        uint32_t tmp;
        sscanf(buf.c_str(), "%x", &tmp);
        code.push_back(tmp);
    }

    // for (int i = 0; i < code.size(); i++)
    // {
    //     std::cout << std::hex << code[i];
    //     std::cout << "\n";
    // }
}


void Executor::getCode(std::vector<uint32_t> arr) 
{
    code = arr;
}


void Executor::getCode(std::vector<std::string> s_arr)
{
    for (auto str : s_arr)
    {
        uint32_t instn;
        sscanf(str.c_str(), "%x", &instn);
        code.push_back(instn);
    }
}


void Executor::getCodeFromElf(std::string fname)
{
    auto elf_ret = elf_parse(fname);

    start_pc = 0;
    pc = elf_ret.entry;
    code = elf_ret.code;
}


Executor::Executor(std::string fname, Err::Error * error)
{
    err = error;
    cache = Cache(CACHE_SIZE, error);
    ph_mem = PhysMem(PHYSMEM_SZIE, error);

    getCode(fname);
    pc = start_pc;
}


void Executor::execute(uint32_t iter)
{
    std::cout << "///////////////////////////////////////////////////////////\n\n";
    std::cout << "\t\tBefore execution\n";
    Instn::Instruction cur_instn{};
    
    if (cache.accessInstn(code[(pc - start_pc) / 4]))
        cur_instn = cache[code[(pc - start_pc) / 4]];
    
    else
    {
        cache.addInstn(code[(pc - start_pc) / 4], iter);
        cur_instn = cache[code[(pc - start_pc) / 4]];
    }

    instnDump(cur_instn.getKey(), cur_instn.getRs1(), cur_instn.getRs2(), cur_instn.getRd(), cur_instn.getImm());

    switch(cur_instn.getKey())
    {
        CASEI(ADDI);
        CASEI(SLTI);
        CASEI(SLTIU);
        CASEI(ANDI);
        CASEI(ORI);
        CASEI(XORI);
        CASEI(SLLI);
        CASEI(SRLI);
        CASEI(SRAI);
        CASEI(LB);
        CASEI(LH);
        CASEI(LW);
        CASEI(LBU);
        CASEI(LHU);
        CASEI(JALR);
        CASER(ADD);
        CASER(SUB);
        CASER(SLT);
        CASER(SLTU);
        CASER(AND);
        CASER(OR);
        CASER(XOR);
        CASER(SLL);
        CASER(SRL);
        CASER(SRA);
        CASES(SB);
        CASES(SH);
        CASES(SW);
        CASEB(BGE);
        CASEB(BLT);
        CASEB(BGEU);
        CASEB(BLTU);
        CASEB(BEQ);
        CASEB(BNE);

        case Instn::InstnKey::kLUI:
            execLUI(cur_instn.getRd(), cur_instn.getImm());
            break;

        case Instn::InstnKey::kAUIPC:
            execAUIPC(cur_instn.getRd(), cur_instn.getImm());
            break;
        
        case Instn::InstnKey::kJAL:
            execJAL(cur_instn.getRd(), cur_instn.getImm());
            break;
    }

    std::cout << "\n\t\tAfter ecexution\n";

    instnDump(cur_instn.getKey(), cur_instn.getRs1(), cur_instn.getRs2(), cur_instn.getRd(), cur_instn.getImm());

    std::cout << "\n\n";
}

#undef CASEI
#undef CASER
#undef CASEB
#undef CASES


void Executor::execADDI(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    ph_mem.setRx(rd, val_rs1 + static_cast<int32_t>(imm));
}


void Executor::execSLTI(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    if (static_cast<int32_t>(val_rs1) < static_cast<int32_t>(imm))
        ph_mem.setRx(rd, 1);
    else
        ph_mem.setRx(rd, 0);
}


void Executor::execSLTIU(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    if (val_rs1 < imm)
        ph_mem.setRx(rd, 1);
    else
        ph_mem.setRx(rd, 0);
}


void Executor::execANDI(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    ph_mem.setRx(rd, val_rs1 & static_cast<int32_t>(imm));
}


void Executor::execORI(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    ph_mem.setRx(rd, val_rs1 | static_cast<int32_t>(imm));
}


void Executor::execXORI(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    ph_mem.setRx(rd, val_rs1 ^ static_cast<int32_t>(imm));
}


void Executor::execSLLI(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint8_t shamt = static_cast<int8_t>(imm & 0x1F);
    ph_mem.setRx(rd, val_rs1 << shamt);
}


void Executor::execSRLI(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint8_t shamt = static_cast<int8_t>(imm & 0x1F);
    ph_mem.setRx(rd, val_rs1 >> shamt);
}


void Executor::execSRAI(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint8_t shamt = static_cast<int8_t>(imm & 0x1F);
    ph_mem.setRx(rd, (val_rs1 >> shamt) | (val_rs1 & (1 << 31)));
}


void Executor::execLUI(uint8_t rd, uint32_t imm)
{
    ph_mem.setRx(rd, (imm & (0xFFFFF << 12)));
}


void Executor::execAUIPC(uint8_t rd, uint32_t imm)
{
    ph_mem.setRx(rd, (imm & (0xFFFFF << 12) + pc));
}


void Executor::execADD(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t val_rs2 = static_cast<int32_t>(ph_mem.getRx(rs2));
    ph_mem.setRx(rd, val_rs1 + val_rs2);
}


void Executor::execSUB(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t val_rs2 = static_cast<int32_t>(ph_mem.getRx(rs2));
    ph_mem.setRx(rd, val_rs1 - val_rs2);
}


void Executor::execSLT(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t val_rs2 = static_cast<int32_t>(ph_mem.getRx(rs2));
    
    if (val_rs1 < val_rs2)
        ph_mem.setRx(rd, 1);
    else
        ph_mem.setRx(rd, 0);
}


void Executor::execSLTU(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    if (val_rs1 < val_rs2)
        ph_mem.setRx(rd, 1);
    else
        ph_mem.setRx(rd, 0);
    
     
}


void Executor::execAND(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    ph_mem.setRx(rd, val_rs1 & val_rs2);

     
}


void Executor::execOR(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    ph_mem.setRx(rd, val_rs1 | val_rs2);

     
}


void Executor::execXOR(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    ph_mem.setRx(rd, val_rs1 ^ val_rs2);
}


void Executor::execSLL(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    ph_mem.setRx(rd, val_rs1 << (val_rs2 & 0x1F));
}


void Executor::execSRL(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    ph_mem.setRx(rd, val_rs1 >> (val_rs2 & 0x1F));
}


void Executor::execSRA(uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t val_rs2 = static_cast<int32_t>(ph_mem.getRx(rs2));

    ph_mem.setRx(rd, val_rs1 >> (val_rs2 & 0x1F));
}


void Executor::execJAL(uint8_t rd, uint32_t imm)
{
    ph_mem.setRx(rd, pc + 4);
    pc += (static_cast<int32_t>(imm)) - 4;
}


void Executor::execJALR(uint8_t rd, uint8_t rs1 ,uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    ph_mem.setRx(rd, pc + 4);
    pc = val_rs1 + ((static_cast<int32_t>(imm) & 0xFFE) << 1) - 4;
}


void Executor::execBEQ(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t val_rs2 = static_cast<int32_t>(ph_mem.getRx(rs2));

    if (val_rs1 == val_rs2)
        pc += (static_cast<int32_t>(imm)) - 4;
}


void Executor::execBNE(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t val_rs2 = static_cast<int32_t>(ph_mem.getRx(rs2));

    if (val_rs1 != val_rs2)
        pc += (static_cast<int32_t>(imm)) - 4;
}


void Executor::execBLT(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t val_rs2 = static_cast<int32_t>(ph_mem.getRx(rs2));

    if (val_rs1 < val_rs2)
        pc += (static_cast<int32_t>(imm)) - 4;
}


void Executor::execBLTU(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    if (val_rs1 < val_rs2)
        pc += (static_cast<int32_t>(imm)) - 4;
}


void Executor::execBGE(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t val_rs2 = static_cast<int32_t>(ph_mem.getRx(rs2));

    if (val_rs1 > val_rs2)
        pc += (static_cast<int32_t>(imm)) - 4;
}


void Executor::execBGEU(uint8_t rd, uint8_t rs1, uint8_t rs2, uint32_t imm)
{
    uint32_t val_rs1 = ph_mem.getRx(rs1);
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    if (val_rs1 > val_rs2)
        pc += (static_cast<int32_t>(imm)) - 4;
}


void Executor::execLB(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t offset = val_rs1 + static_cast<int32_t>(imm);

    uint32_t value = ph_mem.getMem(offset, 1);

    if (value & 0x80)
        value |= 0xFFFFFF00;

    ph_mem.setRx(rd, value);
}


void Executor::execLBU(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t offset = val_rs1 + static_cast<int32_t>(imm);

    uint32_t value = ph_mem.getMem(offset, 1);

    ph_mem.setRx(rd, value);
}


void Executor::execLH(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t offset = val_rs1 + static_cast<int32_t>(imm);

    uint32_t value = ph_mem.getMem(offset, 2);

    if (value & 0x8000)
        value |= 0xFFFF0000;
    
    ph_mem.setRx(rd, value);
}


void Executor::execLHU(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t offset = val_rs1 + static_cast<int32_t>(imm);

    uint32_t value = ph_mem.getMem(offset, 2);

    ph_mem.setRx(rd, value);
}


void Executor::execLW(uint8_t rd, uint8_t rs1, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    int32_t offset = val_rs1 + static_cast<int32_t>(imm);

    uint32_t value = ph_mem.getMem(offset, 4);

    std::cout << "mem[" << offset << "] = " << value << "\n";

    ph_mem.setRx(rd, value);
}


void Executor::execSB(uint8_t rs1, uint8_t rs2, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    int32_t offset = val_rs1 + static_cast<int32_t>(imm);

    ph_mem.setMem(offset, val_rs2, 1);

    std::cout << "mem[" << offset << "] = " << static_cast<int32_t>(ph_mem.getMem(offset, 1));
}


void Executor::execSH(uint8_t rs1, uint8_t rs2, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    int32_t offset = val_rs1 + static_cast<int32_t>(imm);

    ph_mem.setMem(offset, val_rs2, 2);

    std::cout << "mem[" << offset << "] = " << static_cast<int32_t>(ph_mem.getMem(offset, 2));
}


void Executor::execSW(uint8_t rs1, uint8_t rs2, uint32_t imm)
{
    int32_t val_rs1 = static_cast<int32_t>(ph_mem.getRx(rs1));
    uint32_t val_rs2 = ph_mem.getRx(rs2);

    int32_t offset = val_rs1 + static_cast<int32_t>(imm);

    ph_mem.setMem(offset, val_rs2, 4);

    std::cout << "mem[" << offset << "] = " << static_cast<int32_t>(ph_mem.getMem(offset, 4)) << "\n";
}


void Executor::proccesor()
{
    std::cout << std::hex << "start_pc: " << start_pc << " pc: " << pc << std::dec 
    << " code.size() = " << code.size() << "\n\n";

    for (int i = 0; (pc - start_pc) / 4 < code.size() && pc - start_pc >= 0; i++)
    {
        std::cout << "iter: " << i << " ";
        execute(i);
        pc += 4;
    }

    regDump();
}


void Executor::instnDump(Instn::InstnKey kinstn, uint8_t rs1, uint8_t rs2, uint8_t rd, uint32_t imm)
{
    std::cout << std::dec << "Instn: " << instnName[kinstn] << "\nrs1: " << static_cast<int32_t>(rs1) << "\t" 
    << int(ph_mem.getRx(rs1)) << "\nrs2: " << static_cast<int32_t>(rs2) << "\t" 
    << static_cast<int32_t>(ph_mem.getRx(rs2)) << "\nrd: " << static_cast<int32_t>(rd) << "\t"
    << static_cast<int32_t>(ph_mem.getRx(rd)) << "\nimm: " << static_cast<int32_t>(imm) 
    << "\npc: " << std::hex << pc << std::dec << "\n";
}


void Executor::regDump()
{
    std::cout << "////////////////////////////////////////////////////////////////\n\n";

    for (int i = 0; i < 32; i++)
    {
        std::cout << "reg[ " << i << " ] = " << ph_mem.getRx(i) << "\n";
    }
}


void Executor::codeDump()
{
    std::cout << "////////////////////////////////////////////////////////////////\n\n";

    for (uint32_t i = pc; i < code.size(); i++)
    {
         std::cout << std::hex << code[i] << "\n";
    }
}