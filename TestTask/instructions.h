#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <iostream>
#include <string>
#include "error_.h"
#include <limits>
#include <map>

#define M_IMM_SIGN M_IMM20

namespace Instn{

    enum InstnMask : uint32_t
    {
        M_OPCODE    = 0x7F,                 // [0-6] mask
        M_FUNC3     = 0x07 << 12,           // [12-14] mask
        M_RD        = 0x0F << 7,            // [7-11] mask
        M_RS1       = 0x1F << 15,           // [15-19] mask
        M_RS2       = 0x1F << 20,           // [20-24] mask
        M_FUNC7     = (uint32_t)0x7F << 25, // [25-31] mask
        M_32T       = 0x03,                 // [0-1] mask
        M_IMM12     = (uint32_t)0x01 << 31, // [31] mask
        M_IMM10_5   = 0x3F << 25,           // [30-25] mask
        M_IMM4_1    = 0xF << 8,             // [11-8] mask
        M_IMM11B    = 0x01 << 7,            // [7] mask
        M_IMM19_12  = 0xFF << 12,           // 
        M_IMM11J    = 0x01 << 20,           //
        M_IMM10_1   = 0x1FF << 21,          //
        M_IMM20     = (uint32_t)0x01 << 31, //
    };

    enum InstnFunc : uint8_t
    {
        fADDI   = 0x0,  // 000 opcode: 0010011
        fSLTI   = 0x2,  // 010 opcode: 0010011
        fSLTIU  = 0x3,  // 011 opcode: 0010011
        fANDI   = 0x7,  // 111 opcode: 0010011
        fORI    = 0x6,  // 110 opcode: 0010011
        fXORI   = 0x4,  // 100 opcode: 0010011
        fSLLI   = 0x1,  // 001 opcode: 0010011
        fSRLI   = 0x5,  // 101 opcode: 0010011 instn[31:25] = 0000000
        fSRAI   = 0x5,  // 101 opcode: 0010011 instn[31:25] = 0100000
        fLUI,           // nofunc code, opcode: 0110111
        fAUIPC,         // nofunc code, opcode: 0010111
        fADD    = 0x0,  // 000 opcode: 0110011 instn[31:25] = 0000000
        fSLT    = 0x2,  // 010 opcode: 0110011
        fSLTU   = 0x3,  // 011 opcode: 0110011
        fAND    = 0x7,  // 111 opcode: 0110011
        fOR     = 0x6,  // 110 opcode: 0110011
        fXOR    = 0x4,  // 100 opcode: 0110011
        fSLL    = 0x1,  // 001 opcode: 0110011
        fSRL    = 0x5,  // 101 opcode: 0110011 instn[31:25] = 0000000
        fSUB    = 0x0,  // 000 opcode: 0110011 instn[31:25] = 0100000
        fSRA    = 0x5,  // 101 opcode: 0110011 instn[31:25] = 0100000
        fJAL,           // nofunc code opcode:1101111
        fJALR,          // nofunc code opcode:1100111
        fBEQ    = 0x0,  // 000 opcode: 1100011
        fBNE    = 0x1,  // 001 opcode: 1100011
        fBLT    = 0x4,  // 100 opcode: 1100011
        fBLTU   = 0x6,  // 110 opcode: 1100011
        fBGE    = 0x5,  // 101 opcode: 1100011
        fBGEU   = 0x7,  // 111 opcode: 1100011
        fLB     = 0x0,  // 000 opcode: 0000011
        fLH     = 0x1,  // 001 opcode: 0000011
        fLW     = 0x2,  // 010 opcode: 0000011
        fLBU    = 0x4,  // 100 opcode: 0000011
        fLHU    = 0x5,  // 101 opcode: 0000011
        fSB     = 0x0,  // 000 opcode: 0100011
        fSH     = 0x1,  // 001 opcode: 0100011
        fSW     = 0x2,  // 010 opcode: 0100011
        fNO     = 0x0   // For default init, not an instn
    };
    
    enum InstnKey : uint8_t
    {
        kADDI,  // I-TYPE
        kSLTI,  // I-TYPE
        kSLTIU, // I-TYPE
        kANDI,  // I-TYPE
        kORI,   // I-TYPE
        kXORI,  // I-TYPE
        kSLLI,  // I-TYPE
        kSRLI,  // I-TYPE
        kSRAI,  // I-TYPE
        kLUI,   // U-TYPE
        kAUIPC, // U-TYPE
        kADD,   // R-TYPE
        kSLT,   // R-TYPE
        kSLTU,  // R-TYPE
        kAND,   // R-TYPE
        kOR,    // R-TYPE
        kXOR,   // R-TYPE
        kSLL,   // R-TYPE
        kSRL,   // R-TYPE
        kSUB,   // R-TYPE
        kSRA,   // R-TYPE
        kJAL,   // J-TYPE
        kJALR,  // I-TYPE
        kBEQ,   // B-TYPE
        kBNE,   // B-TYPE
        kBLT,   // B-TYPE
        kBLTU,  // B-TYPE
        kBGE,   // B-TYPE
        kBGEU,  // B-TYPE
        kLB,    // I-TYPE
        kLH,    // I-TYPE
        kLW,    // I-TYPE
        kLBU,   // I-TYPE
        kLHU,   // I-TYPE
        kSB,    // S-TYPE
        kSH,    // S-TYPE
        kSW,    // S-TYPE

        kDEFAULT
    };
   
    enum Opcode : uint8_t
    {
        LUI     = (0x0D << 2) + 0x3,
        AUIPC   = (0x05 << 2) + 0x3,
        OP      = (0x0C << 2) + 0x3,
        OPIMM   = (0x04 << 2) + 0x3,
        JAL     = (0x1b << 2) + 0x3,
        JALR    = (0x19 << 2) + 0x3,
        BRANCH  = (0x18 << 2) + 0x3,
        LOAD    = (0x00 << 2) + 0x3,
        STORE   = (0x08 << 2) + 0x3,
        
        OP_DEFAULT = 0
    }; 

    enum InstnType : uint8_t
    {
        U_TYPE,
        I_TYPE,
        R_TYPE,
        S_TYPE,
        B_TYPE,     
        J_TYPE,

        TYPE_DEFAULT
    };
    class Instruction
    {
        private:
            uint8_t rs1, rs2;   // sourse registers
            int32_t imm;        // immidiate value
            uint8_t func3;      // function from Instn::kInstn
            uint8_t func7;      // 7 oldest bits feild in R-Type instructions
            Opcode opcode;
            uint8_t rd;         // dest register
            InstnKey kinstn;    // the key of instruction 
            InstnType tinstn;   // instruction type
            Err::Error * err;

            Opcode getOpcode(uint32_t instn);

            void decodeU(uint32_t instn);
            void decodeS(uint32_t instn);
            void decodeR(uint32_t instn);
            void decodeI(uint32_t instn);
            void decodeB(uint32_t instn);
            void decodeJ(uint32_t instn);

            void decode(uint32_t instn);

        public:
            Instruction(uint32_t instn, Err::Error * error);
            Instruction();

            uint8_t getRs1();
            uint8_t getRs2();
            uint8_t getRd();
            int32_t getImm();
            InstnKey getKey();

            void printInstn();
    };
}

#endif // INSTRUCTIONS_H