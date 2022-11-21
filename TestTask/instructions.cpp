#include "error_.h"
#include "instructions.h"

#define CASE(cmd)   case f##cmd:        \
                        kinstn = k##cmd;\
                        break

using namespace Instn;

const char* instnName[] = {
    "ADDI", "SLTI", "SLTIU", "ANDI", "ORI", "XORI", "SLLI", "SRLI", 
    "SRAI", "LUI", "AUIPC", "ADD", "SLT", "SLTU", "AND", 
    "OR", "XOR", "SLL", "SRL", "SUB", "SRA", "JAL", "JALR",
    "BEQ", "BNE", "BLT", "BLTU", "BGE", "BGEU", "LB", "LH", "LW",
    "LBU", "LHU", "SB", "SH", "SW"
};

std::map<Instn::Opcode, Instn::InstnType> typeMap = {
    {LUI, U_TYPE},
    {AUIPC, U_TYPE},
    {OP, R_TYPE},
    {OPIMM, I_TYPE},
    {JAL, J_TYPE},
    {JALR, I_TYPE},
    {BRANCH, B_TYPE},
    {LOAD, I_TYPE},
    {STORE, S_TYPE}
};

const char* instnTypeName[] = {
    "U_TYPE", "I_TYPE", "R_TYPE", "S_TYPE", "B_TYPE", "J_TYPE"
};

void Instruction::decodeU(uint32_t instn)
{
    rs1 = 0;
    rs2 = 0;
    func3 = 0;
    func7 = 0;
    imm = instn & (M_FUNC3 | M_FUNC7 | M_RS1 | M_RS2);
    rd = (instn & M_RD) >> 7;

    if (opcode == LUI)
        kinstn = kLUI;
    else
        kinstn = kAUIPC;
}
    
void Instruction::decodeI(uint32_t instn)
{
    rs2 = 0;
    func7 = (instn & M_FUNC7) >> 25;
    imm = (instn & (M_RS2 | M_FUNC7)) >> 20;

    if (instn & M_IMM_SIGN)
        imm = imm | ((int32_t)(-1) << 11);
    
    rs1 = (instn & M_RS1) >> 15;
    rd = (instn & M_RD) >> 7;
    func3 = (instn & M_FUNC3) >> 12;

    if (opcode == OPIMM)
    {
        switch (func3)
        {
            CASE(ANDI);
            CASE(SLTI);
            CASE(SLTIU);
            CASE(ADDI);
            CASE(ORI);
            CASE(XORI);
            CASE(SLLI);
            CASE(SRLI);
        }
    }
    else if (opcode == LOAD)
    {
        switch (func3)
        {
            CASE(LB);
            CASE(LH);
            CASE(LW);
            CASE(LHU);
            CASE(LBU);
        }
    }
    else // if opcode == JALR
        kinstn = kJALR;

    if (kinstn == kSRLI && imm != 0)
        kinstn = kSRAI;
}

void Instruction::decodeR(uint32_t instn)
{
    imm = 0;
    rs1 = (instn & M_RS1) >> 15;
    rs2 = (instn & M_RS2) >> 20;
    func7 = (instn & M_FUNC7) >> 25;
    func3 = (instn & M_FUNC3) >> 12;
    rd = (instn & M_RD) >> 7;

    switch (func3)
    {
        CASE(ADD);
        CASE(SLTI);
        CASE(SLTIU);
        CASE(SRL);
        CASE(SLL);
        CASE(AND);
        CASE(OR);
        CASE(XOR);
    }

    if (kinstn == kADD && func7 != 0)
        kinstn = kSUB;

    if (kinstn == kSRL && func7 != 0)
        kinstn = kSRA;
}

void Instruction::decodeJ(uint32_t instn)
{
    int32_t temp = (instn & M_IMM20) >> 11;
    imm = temp;
    temp = (instn & M_IMM10_1) >> 20;
    imm = imm | temp;
    temp = (instn & M_IMM11J) >> 9;
    imm = imm | temp;
    temp = (instn & M_IMM19_12);
    imm = imm | temp;
    temp = (instn & M_IMM20) >> 11;
    imm = imm | temp;

    if (instn & M_IMM_SIGN)
        imm = imm | ((int32_t)(-1) << 20);

    rd = (instn & M_RD) >> 7;
    func3 = 0; 
    func7 = 0; 
    rs1 = 0; 
    rs2 = 0;

    kinstn = kJAL;
}

void Instruction::decodeB(uint32_t instn)
{
    int32_t temp = (instn & M_IMM12) >> 19;
    imm = temp;
    temp = (instn & M_IMM10_5) >> 20;
    imm |= temp;
    temp = (instn & M_IMM4_1) >> 7;
    imm |= temp;
    temp = (instn & M_IMM11B) << 4;
    imm |= temp;

    if (instn & M_IMM_SIGN)
        imm = imm | ((int32_t)(-1) << 12);

    func3 = (instn & M_FUNC3) >> 12;
    rs1 = (instn & M_RS1) >> 15;
    rs2 = (instn & M_RS2) >> 20;
    rd = 0; func7 = 0;

    switch (func3)
    {
        CASE(BEQ);
        CASE(BNE);
        CASE(BLT);
        CASE(BLTU);
        CASE(BGE);
        CASE(BGEU);
    }
}

void Instruction::decodeS(uint32_t instn)
{
    int32_t temp = (instn & M_FUNC7) >> 20;
    imm = temp;
    temp = (instn & M_RD) >> 7;
    imm = imm | temp;

    if (instn & M_IMM_SIGN)
        imm = imm | ((int32_t)(-1) << 11);
    
    func3 = (instn & M_FUNC3) >> 12;
    rs1 = (instn & M_RS1) >> 15;
    rs2 = (instn & M_RS2) >> 20;
    rd = 0; func7 = 0;

    switch (func3)
    {
        CASE(SB);
        CASE(SH);
        CASE(SW);
    }
}

Instruction::Instruction()
{
    opcode = OP_DEFAULT;
    tinstn = TYPE_DEFAULT;
    kinstn = kDEFAULT;
}

Instruction::Instruction(uint32_t instn, Err::Error * error)
{
    err = error;
    
    opcode = getOpcode(instn);
    
    if (!typeMap.count(opcode))
        err->setInstnErr(instn, opcode);

    tinstn = typeMap[opcode];

    decode(instn);
}

Instn::Opcode Instruction::getOpcode(uint32_t instn)
{
    return static_cast<Opcode>(instn & M_OPCODE);
}

void Instruction::decode(uint32_t instn)
{
    switch(tinstn)
    {
        case U_TYPE:
            decodeU(instn);
            break;
        case I_TYPE:
            decodeI(instn);
            break;
        case R_TYPE:
            decodeR(instn);
            break;
        case J_TYPE:
            decodeJ(instn);
            break;
        case B_TYPE:
            decodeB(instn);
            break;
        case S_TYPE:
            decodeS(instn);
            break;
    }
}

void Instn::Instruction::printInstn()
{
    printf("instn: type: %s; instn: %s; rs1: %d; rs2: %d; imm: %d; rd: %d\n", instnTypeName[tinstn], instnName[kinstn], rs1, rs2, imm, rd);
}

uint8_t Instn::Instruction::getRs1()
{
    return rs1;
}

uint8_t Instn::Instruction::getRs2()
{
    return rs2;
}

uint8_t Instn::Instruction::getRd()
{
    return rd;
}

int32_t Instn::Instruction::getImm()
{
    return imm;
}

Instn::InstnKey Instn::Instruction::getKey()
{
    return kinstn;
}