//***********************************************************************
// ECGR 5181 - Assignment 3 - Decoder Simulation  		(C++ Verision)	*
//																		*
// This program will utilize custom functions/processes to simulate how	*
// a computer processor would decode a set of machine code input		*
// instructions to allow the processor to know how to excute the 		*
// incoming instructions.                                               *
//  																	*															
// Kendall Britton, Jaron Price, Sreekanksha Gangadhar      2022-10-12	* 
//***********************************************************************

// List of libraries that are required to run the program
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <cmath>
using namespace std;


enum opcodeMappingDataTransfer {Lb = 0b0000011, Lh = 0b0000011, Lw = 0b0000011, Lbu = 0b0000011, Lhu = 0b0000011,                       // Mapped opcode to each instruction within each category
                                Sb = 0b0100011, Sh = 0b0100011, Sw = 0b0100011, Lwu = 0b0000011, Ld = 0b0000011,
                                Sd = 0b0100011, Flw = 0b0000111, Fsw = 0b0100111, Fld = 0b0000111, Fsd = 0b0100111,
                                Fmv_x = 0b1010011, Fmvx_ = 0b1010011};

enum opcodeMappingArithmeticLogical {Add = 0b0110011, Addi = 0b0010011, Addw = 0b0111011, Addiw = 0b0011011, Sub = 0b0110011,           // Mapped opcode to each instruction within each category
                                     Subw = 0b0111011, Mul = 0b0110011, Mulw = 0b0111011, Mulh = 0b0110011, Mulhsu = 0b0110011,
                                     Mulhu = 0b0110011, Div = 0b0110011, Divu = 0b0110011, Rem = 0b0110011, Remu = 0b0110011,
                                     Divw = 0b0111011, Divuw = 0b0111011, Remw = 0b0111011, Remuw = 0b0111011, And = 0b0110011,
                                     Andi = 0b0010011, Or = 0b0110011, Ori = 0b0010011, Xor = 0b0110011, Xori = 0b0010011,
                                     Lui = 0b0110111, Auipc = 0b0010111, Sll = 0b0110011, Slli = 0b0010011, Srl = 0b0110011,
                                     Srli = 0b0010011, Sra = 0b0110011, Srai = 0b0010011, Sllw = 0b0111011, Slliw = 0b0011011,
                                     Srlw = 0b0111011, Srliw = 0b0011011, Sraw = 0b0111011, Sraiw = 0b0011011, Slt = 0b0110011,
                                     Slti = 0b0010011, Sltu = 0b0110011, Sltiu = 0b0010011};

enum opcodeMappingControl {Beq = 0b1100011, Bne = 0b1100011, Blt = 0b1100011, Bge = 0b1100011, Bltu = 0b1100011,                        // Mapped opcode to each instruction within each category
                           Bgeu = 0b1100011, Jal = 0b1101111, Jalr = 0b1100111};


// List of variable used to perform simulation
bool rType = false;         
bool iType = false;
bool sType = false;             // Variables to keep track of which instruction type each instruction is
bool bType = false;
bool uType = false;
bool jType = false;

int iTypeCate = 0;          // Determines which type of I instruction it is

int rd[5] = {};                     // Rd is x8 (0b01000)
int rs1[5] = {};                    // Rs1 is x9 (0b01001)
int rs2[5] = {};                    // Rs2 is x10 (0b01010)
int imm_Itype[12] = {};
int imm_StypeUpper[7] = {};
int imm_StypeLower[5] = {};
int imm_BtypeUpper[7] = {};
int imm_BtypeLower[5] = {};
int imm_Utype[20] = {};
int imm_Jtype[20] = {};

int rdToDecimal = 0;                // Rd decoded decimal equivalent
int rs1ToDecimal = 0;               // Rs1 decoded decimal equivalent
int rs2ToDecimal = 0;               // Rs2 decoded decimal equivalent
int immToDecimal = 0;               // Immediate decoded decimal equivalent

int inputArray[32] = {};   // LSB is the rightmost bit


// Variables that store info/states for printing
string opcodeType;
string inst;
int aluOp;
int branch;
int memRead;
int writeBackSel;
int memWrite;
int aluSrc;
int regWrite;
int signExt;
int pcSel;
int branchCondition;
int branchSel;




// Call to the functions that will decode and print output
void determineOperation();
void printEvent();
void assignRegisterFields();


// Main Function
int main() {
    
    fstream myFile; 

    myFile.open("ECGR 5181 Assignment 3 Instruction List.txt", ios::in);        // Reads info from file

    if (!myFile.is_open()) {
        cout << "Error opening file" << endl;
    }
    else {
        string line;
        int tempArray[32];

        while (getline(myFile,line)) {          // Continously gets each line of machine code in file
            
            for (int i = 0; i < 32; i++) {          // Converts from string to integers in array

                tempArray[i] = (int)line[i];

                if (tempArray[i] == 48) {               // If '0' in ACSII set 0 integer
                    inputArray[i] = 0;
                } else if (tempArray[i] == 49) {        // If '1' in ACSII set 1 integer
                    inputArray[i] = 1;
                }

            }

            determineOperation();           // Decodes and determines the instruction
        }

        myFile.close();         // Closes file once finished
    }
}


// Function that displays the output of each instruction includeing control signals
void printEvent() {

    cout << "--------------------------------------------------------------" << endl;
    cout << "Opcode Type: " << opcodeType << endl;

    cout << "Inst: " << inst;

    if (rType == true) {
        printf("x%d, x%d, x%d \n", rdToDecimal, rs1ToDecimal, rs2ToDecimal);        // Displays the correct instruction structure based on type of instruction
    }
    else if (iType == true) {
        if (iTypeCate == 1) {
            printf("x%d, x%d, %d \n", rdToDecimal, rs1ToDecimal, immToDecimal);
        } else if (iTypeCate == 2) {
            printf("x%d, %d(x%d) \n", rdToDecimal, immToDecimal, rs1ToDecimal);
        }
    }
    else if (sType == true) {
        printf("x%d, %d(x%d) \n", rs2ToDecimal, immToDecimal, rs1ToDecimal);
    }
    else if (bType == true) {
        printf("x%d, x%d, %d \n", rs1ToDecimal, rs2ToDecimal, immToDecimal);
    }
    else if (uType == true) {
        printf("x%d, %d \n", rdToDecimal, immToDecimal);
    }
    else if (jType == true) {
        printf("x%d, %d(x%d) \n", rdToDecimal, immToDecimal, rs1ToDecimal);
    }

    cout << "Control Signals:" << endl;
    cout << "Reg_Write_Enable: " << regWrite << ", Reg_Read_Enable: " << regWrite << ", Alu_op: " << aluOp << endl;                   // Outputs active/non-active control signals
    cout << "Mem_Write_Enable: " << memWrite << ", Mem_Read_Enable: " << memRead << ", Writeback_Enable: " << writeBackSel << endl;
    cout << "Imm_Sel: " << aluSrc << ", Sign_Extend: " << signExt << ", Branch: " << branch << ", Branch_Condition: " << branchCondition << endl;
    cout << "Branch_Select: " << branchSel << ", PC_Select " << pcSel << endl;
}


// Function that maps and decodes the various parts of the input instruction based on instruction types
void assignRegisterFields() {

    if (rType == true) {
        rd[0] = inputArray[24];         // Decodes register destination
        rd[1] = inputArray[23];
        rd[2] = inputArray[22];
        rd[3] = inputArray[21];
        rd[4] = inputArray[20];

        rs1[0] = inputArray[16];        // Decodes register 1 field
        rs1[1] = inputArray[15];
        rs1[2] = inputArray[14];
        rs1[3] = inputArray[13];
        rs1[4] = inputArray[12];

        rs2[0] = inputArray[11];        // Decodes register 2 field
        rs2[1] = inputArray[10];
        rs2[2] = inputArray[9];
        rs2[3] = inputArray[8];
        rs2[4] = inputArray[7];
    }
    else if (iType == true) {
        rd[0] = inputArray[24];         // Decodes register destination
        rd[1] = inputArray[23];
        rd[2] = inputArray[22];
        rd[3] = inputArray[21];
        rd[4] = inputArray[20];

        rs1[0] = inputArray[16];        // Decodes register 1 field
        rs1[1] = inputArray[15];
        rs1[2] = inputArray[14];
        rs1[3] = inputArray[13];
        rs1[4] = inputArray[12];

        imm_Itype[0] = inputArray[11];      // Decodes immediate value
        imm_Itype[1] = inputArray[10];
        imm_Itype[2] = inputArray[9];
        imm_Itype[3] = inputArray[8];
        imm_Itype[4] = inputArray[7];
        imm_Itype[5] = inputArray[6];
        imm_Itype[6] = inputArray[5];
        imm_Itype[7] = inputArray[4];
        imm_Itype[8] = inputArray[3];
        imm_Itype[9] = inputArray[2];
        imm_Itype[10] = inputArray[1];
        imm_Itype[11] = inputArray[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimal += ((imm_Itype[i]) * pow(2, i));
        }

    }
    else if (sType == true) {
        imm_StypeLower[0] = inputArray[24];         // Decodes immediate value
        imm_StypeLower[1] = inputArray[23];
        imm_StypeLower[2] = inputArray[22];
        imm_StypeLower[3] = inputArray[21];
        imm_StypeLower[4] = inputArray[20];

        rs1[0] = inputArray[16];                    // Decodes register 1 field
        rs1[1] = inputArray[15];
        rs1[2] = inputArray[14];
        rs1[3] = inputArray[13];
        rs1[4] = inputArray[12];

        rs2[0] = inputArray[11];                    // Decodes register 2 field
        rs2[1] = inputArray[10];
        rs2[2] = inputArray[9];
        rs2[3] = inputArray[8];
        rs2[4] = inputArray[7];

        imm_StypeUpper[0] = inputArray[6];          // Decodes immediate value
        imm_StypeUpper[1] = inputArray[5];
        imm_StypeUpper[2] = inputArray[4];
        imm_StypeUpper[3] = inputArray[3];
        imm_StypeUpper[4] = inputArray[2];
        imm_StypeUpper[5] = inputArray[1];
        imm_StypeUpper[6] = inputArray[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimal += ((imm_StypeUpper[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimal += ((imm_StypeLower[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (bType == true) {
        imm_BtypeLower[0] = inputArray[24];       // Decodes immediate value
        imm_BtypeLower[1] = inputArray[23];
        imm_BtypeLower[2] = inputArray[22];
        imm_BtypeLower[3] = inputArray[21];
        imm_BtypeLower[4] = inputArray[20];

        rs1[0] = inputArray[16];                  // Decodes register 1 field
        rs1[1] = inputArray[15];
        rs1[2] = inputArray[14];
        rs1[3] = inputArray[13];
        rs1[4] = inputArray[12];

        rs2[0] = inputArray[11];                  // Decodes register 2 field
        rs2[1] = inputArray[10];
        rs2[2] = inputArray[9];
        rs2[3] = inputArray[8];
        rs2[4] = inputArray[7];

        imm_BtypeUpper[0] = inputArray[6];        // Decodes immediate value
        imm_BtypeUpper[1] = inputArray[5];
        imm_BtypeUpper[2] = inputArray[4];
        imm_BtypeUpper[3] = inputArray[3];
        imm_BtypeUpper[4] = inputArray[2];
        imm_BtypeUpper[5] = inputArray[1];
        imm_BtypeUpper[6] = inputArray[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimal += ((imm_BtypeUpper[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimal += ((imm_BtypeLower[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (uType == true) {
        rd[0] = inputArray[24];                     // Decodes register destination
        rd[1] = inputArray[23];
        rd[2] = inputArray[22];
        rd[3] = inputArray[21];
        rd[4] = inputArray[20];

        imm_Utype[0] = inputArray[19];              // Decodes immediate value
        imm_Utype[1] = inputArray[18];
        imm_Utype[2] = inputArray[17];
        imm_Utype[3] = inputArray[16];
        imm_Utype[4] = inputArray[15];
        imm_Utype[5] = inputArray[14];
        imm_Utype[6] = inputArray[13];
        imm_Utype[7] = inputArray[12];
        imm_Utype[8] = inputArray[11];
        imm_Utype[9] = inputArray[10];
        imm_Utype[10] = inputArray[9];
        imm_Utype[11] = inputArray[8];
        imm_Utype[12] = inputArray[7];
        imm_Utype[13] = inputArray[6];
        imm_Utype[14] = inputArray[5];
        imm_Utype[15] = inputArray[4];
        imm_Utype[16] = inputArray[3];
        imm_Utype[17] = inputArray[2];
        imm_Utype[18] = inputArray[1];
        imm_Utype[19] = inputArray[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimal += ((imm_Utype[i]) * pow(2, i));
        }

    }
    else if (jType == true) {
        rd[0] = inputArray[24];                 // Decodes register destination
        rd[1] = inputArray[23];
        rd[2] = inputArray[22];
        rd[3] = inputArray[21];
        rd[4] = inputArray[20];

        imm_Jtype[0] = inputArray[19];          // Decodes immediate value
        imm_Jtype[1] = inputArray[18];
        imm_Jtype[2] = inputArray[17];
        imm_Jtype[3] = inputArray[16];
        imm_Jtype[4] = inputArray[15];
        imm_Jtype[5] = inputArray[14];
        imm_Jtype[6] = inputArray[13];
        imm_Jtype[7] = inputArray[12];
        imm_Jtype[8] = inputArray[11];
        imm_Jtype[9] = inputArray[10];
        imm_Jtype[10] = inputArray[9];
        imm_Jtype[11] = inputArray[8];
        imm_Jtype[12] = inputArray[7];
        imm_Jtype[13] = inputArray[6];
        imm_Jtype[14] = inputArray[5];
        imm_Jtype[15] = inputArray[4];
        imm_Jtype[16] = inputArray[3];
        imm_Jtype[17] = inputArray[2];
        imm_Jtype[18] = inputArray[1];
        imm_Jtype[19] = inputArray[0];

        for (int i = 19; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimal += ((imm_Jtype[i]) * pow(2, i));
        }

    }

    for (int i = 4; i > -1; i--) {
        rdToDecimal += ((rd[i]) * pow(2, i));       // Decodes register destination (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs1ToDecimal += ((rs1[i]) * pow(2, i));     // Decodes register 1 location (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs2ToDecimal += ((rs2[i]) * pow(2, i));     // Decodes register 2 location (coverts to decimal)
    }
 
}


// Function that determines which instruction is to be performed from machine code
void determineOperation() {

    int opcode[7] = {};
    int func3[3] = {};
    int func7[7] = {};
    int func3ToDecimal = 0;
    int func7ToDecimal = 0;
    int opcodeToDecimal = 0;
    bool validInstruction = true;

    opcode[0] = inputArray[31];             // Decodes opcode from input array
    opcode[1] = inputArray[30];
    opcode[2] = inputArray[29];
    opcode[3] = inputArray[28];
    opcode[4] = inputArray[27];
    opcode[5] = inputArray[26];
    opcode[6] = inputArray[25];

    func3[0] = inputArray[19];              // Decodes func3 from input array
    func3[1] = inputArray[18];
    func3[2] = inputArray[17];

    func7[0] = inputArray[6];               // Decodes func7 from input array
    func7[1] = inputArray[5];
    func7[2] = inputArray[4];
    func7[3] = inputArray[3];
    func7[4] = inputArray[2];
    func7[5] = inputArray[1];
    func7[6] = inputArray[0];

    for (int i = 6; i > -1; i--) {
        opcodeToDecimal += ((opcode[i]) * pow(2, i));           // Calculates opcode value in decimal
    }

    for (int i = 2; i > -1; i--) {                              // Calculates func3 value in decimal
        func3ToDecimal += ((func3[i]) * pow(2, i));
    }

    for (int i = 6; i > -1; i--) {                              // Calculates func7 value in decimal
        func7ToDecimal += ((func7[i]) * pow(2, i));
    }

    /* Jumps to each section of instruction depending on instruction */
    switch (opcodeToDecimal) {      

    /* Start of Data Transfer Opcode Section */

        case 3:                                                         // Load instructions    
            switch (func3ToDecimal) {

                case 0:             // Lb instruction 
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Lb ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;
                      
                case 1:             // Lh instruction
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Lh ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 2:             // Lw instruction 
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Lw ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 3:             // Ld instruction
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Ld ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 4:             // Lbu instruction
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Lbu ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 5:             // Lhu instruction
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Lhu ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 6:             // Lwu instruction
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Lwu ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }

            break;

        case 35:                                                        // Store instructions     
            switch (func3ToDecimal) {

                case 0:             // Sb instruction 
                    sType = true;
                    opcodeType = "S";
                    inst = "Sb ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 1;
                    writeBackSel = 2;
                    memWrite = 1;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 1:             // Sh instruction
                    sType = true;
                    opcodeType = "S";
                    inst = "Sh ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 1;
                    writeBackSel = 2;
                    memWrite = 1;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 2:             // Sw instruction 
                    sType = true;
                    opcodeType = "S";
                    inst = "Sw ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 1;
                    writeBackSel = 2;
                    memWrite = 1;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 3:             // Sd instruction
                    sType = true;
                    opcodeType = "S";
                    inst = "Sd ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 1;
                    writeBackSel = 2;
                    memWrite = 1;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }

            break;

        case 7:                                                         // Float load instructions    
            switch (func3ToDecimal) {

                case 2:             // Flw instruction 
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Flw ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 3:             // Fld instruction
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Fld ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }

            break;

        case 39:                                                        // Float store instructions   
            switch (func3ToDecimal) {

                case 2:             // Fsw instruction 
                    sType = true;
                    opcodeType = "S";
                    inst = "Fsw ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 1;
                    writeBackSel = 2;
                    memWrite = 1;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 3:             // Fsd instruction
                    sType = true;
                    opcodeType = "S";
                    inst = "Fsd ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 1;
                    writeBackSel = 2;
                    memWrite = 1;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }

            break;

        case 83:                                                        // Float/integer copy to/from instructions 
            switch (func3ToDecimal) {

                case 0:             // FMV._.X and FMV.X._ instructions 
                    switch (func7ToDecimal) {

                        case 112:                       // FMV.X.W instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "FMV.X.W ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 120:                       // FMV.W.X instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "FMV.W.X ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 113:                       // FMV.X.D instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "FMV.X.D ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 121:                       // FMV.D.X instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "FMV.D.X ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                break;

            default:
                cout << "Error has occured in func3" << endl;
                break;
            }

            break;

    /* Start of Arithmetic and Logical Opcode Section */

        case 51:                                                        // Arithmetic and Logical Instructions   
            switch (func3ToDecimal) {

                case 0:      
                    switch (func7ToDecimal) {

                        case 0:                     // Add instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Add ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 32:                    // Sub instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Sub ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 1:                     // Mul instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Mul ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 1:        
                    switch (func7ToDecimal) {

                        case 0:                     // Sll instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Sll ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 1:                    // Mulh instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Mulh ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 2:     
                    switch (func7ToDecimal) {

                        case 0:                     // Slt instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Slt ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 1:                    // Mulhsu instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Mulhsu ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 3:      
                    switch (func7ToDecimal) {

                        case 0:                     // Sltu instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Sltu ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 1:                    // Mulhu instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Mulhu ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 4:   
                    switch (func7ToDecimal) {

                        case 0:                     // Xor instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Xor ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 1:                     // Div instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Div ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 5:      
                    switch (func7ToDecimal) {

                        case 0:                     // Srl instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Srl ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 1:                     // Divu instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Divu ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 32:                    // Sra instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Sra ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 6:      
                    switch (func7ToDecimal) {

                        case 0:                     // Or instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Or ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 1:                    // Rem instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Rem ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 7:
                    switch (func7ToDecimal) {

                        case 0:                     // And instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "And ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 1:                    // Remu instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Remu ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }

            break;

        case 19:                                                         // Immediate Instructions     
            switch (func3ToDecimal) {
                case 0:                             // Addi instruction
                    iType = true;
                    iTypeCate = 1;
                    opcodeType = "I";
                    inst = "Addi ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 1:
                    switch (func7ToDecimal) {

                        case 0:                     // Slli instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Slli ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 1;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 2:                             // Slti instruction
                    iType = true;
                    iTypeCate = 1;
                    opcodeType = "I";
                    inst = "Slti ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 3:                             // Sltiu instruction
                    iType = true;
                    iTypeCate = 1;
                    opcodeType = "I";
                    inst = "Sltiu ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 4:                             // Xori instruction
                    iType = true;
                    iTypeCate = 1;
                    opcodeType = "I";
                    inst = "Xori ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 5:
                    switch (func7ToDecimal) {

                        case 0:                         // Srli instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Srli ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 1;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 32:                        // Srai instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Srai ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 1;
                            regWrite = 1;
                            signExt = 0;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 6:                             // Ori instruction
                    iType = true;
                    iTypeCate = 1;
                    opcodeType = "I";
                    inst = "Ori ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 7:                             // Andi instruction
                    iType = true;
                    iTypeCate = 1;
                    opcodeType = "I";
                    inst = "Andi ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }

            break;

        case 59:                                                        // Arithmetic 64 bit instructions   
            switch (func3ToDecimal) {
                case 0:
                    switch (func7ToDecimal) {

                    case 0:                         // Addw instruction 
                        rType = true;
                        opcodeType = "R";
                        inst = "Addw ";
                        aluOp = 1;
                        branch = 0;
                        memRead = 0;
                        writeBackSel = 0;
                        memWrite = 0;
                        aluSrc = 0;
                        regWrite = 1;
                        signExt = 1;
                        pcSel = 0;
                        branchCondition = 0;
                        branchSel = 0;
                        break;

                    case 32:                        // Subw instruction
                        rType = true;
                        opcodeType = "R";
                        inst = "Subw ";
                        aluOp = 1;
                        branch = 0;
                        memRead = 0;
                        writeBackSel = 0;
                        memWrite = 0;
                        aluSrc = 0;
                        regWrite = 1;
                        signExt = 1;
                        pcSel = 0;
                        branchCondition = 0;
                        branchSel = 0;
                        break;

                    case 1: 
                        rType = true;               // Mulw instruction
                        opcodeType = "R";
                        inst = "Mulw ";
                        aluOp = 1;
                        branch = 0;
                        memRead = 0;
                        writeBackSel = 0;
                        memWrite = 0;
                        aluSrc = 0;
                        regWrite = 1;
                        signExt = 1;
                        pcSel = 0;
                        branchCondition = 0;
                        branchSel = 0;
                        break;                      

                    default:
                        cout << "Error has occured in func7" << endl;
                        break;
                    }

                    break;

                case 1:
                    switch (func7ToDecimal) {

                        case 0:                         // Sllw instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Sllw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 4:
                    switch (func7ToDecimal) {

                        case 1:                         // Divw instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Divw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 5:
                    switch (func7ToDecimal) {

                        case 0:                         // Srlw instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Srlw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 32:                        // Sraw instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Sraw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 1:
                            rType = true;               // Divuw instruction
                            opcodeType = "R";
                            inst = "Divuw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;                      

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 6:
                    switch (func7ToDecimal) {

                        case 1:                         // Remw instruction 
                            rType = true;
                            opcodeType = "R";
                            inst = "Remw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 7:
                    switch (func7ToDecimal) {

                        case 1:                         // Remuw instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Remuw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 0;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }

            break;

        case 27:                                                        // Shift right immediate 64 bit instructions        
            switch (func3ToDecimal) {
                case 0:                                 // Addiw instruction
                    iType = true;
                    iTypeCate = 1;
                    opcodeType = "I";
                    inst = "Addiw ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 1;
                    pcSel = 0;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                case 1:
                    switch (func7ToDecimal) {           
                        case 0:                         // Slliw instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Slliw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 1;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                case 5:
                    switch (func7ToDecimal) {           
                        case 0:                         // Srliw instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Srliw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 1;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        case 32:                        // Sraiw instruction
                            rType = true;
                            opcodeType = "R";
                            inst = "Sraiw ";
                            aluOp = 1;
                            branch = 0;
                            memRead = 0;
                            writeBackSel = 0;
                            memWrite = 0;
                            aluSrc = 1;
                            regWrite = 1;
                            signExt = 1;
                            pcSel = 0;
                            branchCondition = 0;
                            branchSel = 0;
                            break;

                        default:
                            cout << "Error has occured in func7" << endl;
                            break;
                    }

                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }

            break;

        case 55:                                                    // Lui instruction  
            uType = true;
            opcodeType = "U";
            inst = "Lui ";
            aluOp = 1;
            branch = 0;
            memRead = 0;
            writeBackSel = 0;
            memWrite = 0;
            aluSrc = 1;
            regWrite = 1;
            signExt = 1;
            pcSel = 0;
            branchCondition = 0;
            branchSel = 0;
            break;

        case 23:                                                    // Auipc instruction
            uType = true;
            opcodeType = "U";
            inst = "Auipc ";
            aluOp = 1;
            branch = 0;
            memRead = 0;
            writeBackSel = 1;
            memWrite = 0;
            aluSrc = 1;
            regWrite = 1;
            signExt = 0;
            pcSel = 1;
            branchCondition = 0;
            branchSel = 0;
            break;

    /* Start of Control Opcode Section */
                    
        case 99:                                                    // Branch instructions    
            switch (func3ToDecimal) {
                case 0:                            // Beq instruction
                    bType = true;
                    opcodeType = "B";
                    inst = "Beq ";
                    aluOp = 1;
                    branch = 1;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 0;
                    signExt = 0;
                    pcSel = 2;
                    branchCondition = 1;
                    branchSel = 1;
                    break;

                case 1:                            // Bne instruction
                    bType = true;
                    opcodeType = "B";
                    inst = "Bne ";
                    aluOp = 1;
                    branch = 1;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 0;
                    signExt = 0;
                    pcSel = 2;
                    branchCondition = 1;
                    branchSel = 1;
                    break;

                case 4:                            // Blt instruction
                    bType = true;
                    opcodeType = "B";
                    inst = "Blt ";
                    aluOp = 1;
                    branch = 1;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 0;
                    signExt = 0;
                    pcSel = 2;
                    branchCondition = 1;
                    branchSel = 1;
                    break;

                case 5:                            // Bge instruction
                    bType = true;
                    opcodeType = "B";
                    inst = "Bge ";
                    aluOp = 1;
                    branch = 1;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 0;
                    signExt = 0;
                    pcSel = 2;
                    branchCondition = 1;
                    branchSel = 1;
                    break;

                case 6:                            // Bltu instruction
                    bType = true;
                    opcodeType = "B";
                    inst = "Bltu ";
                    aluOp = 1;
                    branch = 1;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 0;
                    signExt = 0;
                    pcSel = 2;
                    branchCondition = 1;
                    branchSel = 1;
                    break;

                case 7:                            // Bgeu instruction
                    bType = true;
                    opcodeType = "B";
                    inst = "Bgeu ";
                    aluOp = 1;
                    branch = 1;
                    memRead = 0;
                    writeBackSel = 0;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 0;
                    signExt = 0;
                    pcSel = 2;
                    branchCondition = 1;
                    branchSel = 1;
                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }
            break;  

        case 111:                                                   // Jal instruction 
            jType = true;
            opcodeType = "J";
            inst = "Jal ";
            aluOp = 1;
            branch = 0;
            memRead = 0;
            writeBackSel = 1;
            memWrite = 0;
            aluSrc = 1;
            regWrite = 1;
            signExt = 0;
            pcSel = 2;
            branchCondition = 0;
            branchSel = 0;
            break;

        case 103:                                             
            switch (func3ToDecimal) {
                case 0:                                             // Jalr instruction
                    iType = true;
                    iTypeCate = 2;
                    opcodeType = "I";
                    inst = "Jalr ";
                    aluOp = 1;
                    branch = 0;
                    memRead = 0;
                    writeBackSel = 1;
                    memWrite = 0;
                    aluSrc = 1;
                    regWrite = 1;
                    signExt = 0;
                    pcSel = 1;
                    branchCondition = 0;
                    branchSel = 0;
                    break;

                default:
                    cout << "Error has occured in func3" << endl;
                    break;
            }
            break;

    /* Default Case */

        default:
            cout << "Invalid Instruction" << endl;
            validInstruction = false;
            break;
    }

    if (validInstruction == true) {     // If instruction is valid, process the rest

        assignRegisterFields();     // Call function to decode the remaining parts of machine code input
        printEvent();               // Prints the output of instruction and corresponding control signals

    }

    rType = false;              // Resets all instruction types 
    iType = false;
    sType = false;
    bType = false;
    uType = false;
    jType = false;

    iTypeCate = 0;              // Resets all values 
    rdToDecimal = 0;
    rs1ToDecimal = 0;
    rs2ToDecimal = 0;
    immToDecimal = 0;
  
    validInstruction = true;
}