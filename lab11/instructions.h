#define I_HALT 0x0
#define I_NOP 0x1
#define I_RRMOVL 0x2
#define I_IRMOVL 0x3
#define I_RMMOVL 0x4
#define I_MRMOVL 0x5
#define I_OPL 0x6
#define I_JXX 0x7
#define I_CMOVXX 0x2
#define I_CALL 0x8
#define I_RET 0x9
#define I_PUSHL 0xA
#define I_POPL 0xB
#define I_DUMP 0xC

#define S_AOK 1
#define S_HLT 2
#define S_ADR 3
#define S_INS 4

#define OP_ADDL 0
#define OP_SUBL 1
#define OP_ANDL 2
#define OP_XORL 3

#define RRM 0
#define CLE 1
#define CL 2
#define CE 3
#define CNE 4
#define CGE 5
#define CG 6