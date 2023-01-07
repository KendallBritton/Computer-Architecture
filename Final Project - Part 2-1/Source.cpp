//***********************************************************************
// ECGR 5181 - Assignment 5 (Part 2 Question 1) - Cache Simulation		*		
// (C++ Verision)														*
//																		*
// This program will utilize custom structures/variables to simulate 	*
// how a computer processor would pipeline a looped event in assembly	*
// instructions via C code for single and multiple processors.			*
// 																		*															
// Kendall Britton, Jaron Price, Sreekanksha Gangadhar		2022-11-07	* 
//***********************************************************************

// List of libraries that are required to run the program
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <unordered_map>
#include <list>
using namespace std;

// Data structure that simulates a LRU Cache
typedef struct CacheLRU {


    int capacitySize = 0;					// Holds the total size of cache
    int cacheLineSize = 0;					// Holds the total line size inside cache
    int cacheAssociativity = 1;				// Holds the associativity of cache
    int numberOfOffsetBits = 0;				// Holds the number of offset bits
    int numberOfSets = 0;					// Holds the number of set 
    int numberOfIndexBitsPerSet = 0;		// Holds the number of bits inside each set
    int numberOfTagBits = 0;				// Holds the number of tag bits
    int numberOfBitsInAddress = 32;			// Holds the number of bits in data address
    int numberOfTotalBlocks = 0;

    int numberOfHits = 0;					// Keeps track of the number of hits in cache
    int numberOfMisses = 0;					// Keeps track of the number of misses in cache
    int totalAccesses = 0;					// Keeps track of the total number of accesses in cache

    int lookUpTime = 0;
    int lruList1[4] = {};
    int lruList2[4] = {};

    CacheLRU() {

    }

};

int decimalAddressValue = 0;
int binaryArray[32] = {};
int tagBitFromBinary = 0;
int indexSetBitFromBinary = 0;
int advanceMap;
vector<int> lastElementUsed1;
vector<int> lastElementUsed2;

vector<int> lastElementUsed1ICPU0Optimize;
vector<int> lastElementUsed2ICPU0Optimize;
vector<int> lastElementUsed3ICPU0Optimize;
vector<int> lastElementUsed4ICPU0Optimize;

vector<int> lastElementUsed1DCPU0Optimize;
vector<int> lastElementUsed2DCPU0Optimize;
vector<int> lastElementUsed3DCPU0Optimize;
vector<int> lastElementUsed4DCPU0Optimize;

vector<int> lastElementUsed1ICPU1Optimize;
vector<int> lastElementUsed2ICPU1Optimize;
vector<int> lastElementUsed3ICPU1Optimize;
vector<int> lastElementUsed4ICPU1Optimize;

vector<int> lastElementUsed1DCPU1Optimize;
vector<int> lastElementUsed2DCPU1Optimize;
vector<int> lastElementUsed3DCPU1Optimize;
vector<int> lastElementUsed4DCPU1Optimize;


//int cacheICPU0PositionArray[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
//int cacheICPU1PositionArray[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
//int cacheDCPU0PositionArray[16] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
//int cacheDCPU1PositionArray[16] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

int cacheICPU0PositionArray[4][2] = { {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1} };
int cacheICPU1PositionArray[4][2] = { {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1} };
int cacheDCPU0PositionArray[4][2] = { {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1} };
int cacheDCPU1PositionArray[4][2] = { {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1} };

char hexStringCache[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
int iterationACache0 = 0;
int iterationBCache0 = 0;
int iterationACache1 = 0;
int iterationBCache1 = 0;
int cacheDMode0 = 0;
int cacheDMode1 = 0;
int cacheDCount0 = 0;
int cacheDCount1 = 0;
int addressACache0 = 0x00000400;
int addressBCache0 = 0x00000800;
int addressACache1 = 0x00000400;
int addressBCache1 = 0x00000800;
int hitOrMissCacheD0 = -1;
int hitOrMissCacheD1 = -1;
int hitOrMissCacheI0 = -1;
int hitOrMissCacheI1 = -1;


fstream myFileAddressesCPU0, myFileAddressesCPU1;
CacheLRU cacheICPU0, cacheDCPU0, cacheICPU1, cacheDCPU1;


#define instructionList0Addr 0x0                 // Start address of instruction list
#define instructionList1Addr 0x0                 // Start address of instruction list
#define stack0Addr 0x0200                        // Start address of stack 0
#define stack1Addr 0x0300                        // Start address of stack 1

#define ARRAY_A_ADDR 0x0400                     // Start address of Array A
#define ARRAY_B_ADDR 0x0800                     // Start address of Array B
#define ARRAY_C_ADDR 0x0C00                     // Start address of Array C
#define ARRAY_D_ADDR 0x1000                     // Start address of Array D

typedef struct memoryArray {                    // Structure that contains array in memory
    float mainArray[256] = {};
    float* mainArrayAddress[256] = {};
};

float* ARRAY_A = (float*)(void*)ARRAY_A_ADDR;                     // Assigns address to array
float* ARRAY_B = (float*)(void*)ARRAY_B_ADDR;                     // Assigns address to array
float* ARRAY_C = (float*)(void*)ARRAY_C_ADDR;                     // Assigns address to array
float* ARRAY_D = (float*)(void*)ARRAY_D_ADDR;                     // Assigns address to array
int* instruction0List = (int*)(void*)instructionList0Addr;          // Assigns address to instruction list
int* instruction1List = (int*)(void*)instructionList1Addr;          // Assigns address to instruction list
float* stack0 = (float*)(void*)stack0Addr;                          // Assigns address to stack 0
float* stack1 = (float*)(void*)stack1Addr;                          // Assigns address to stack 1


// List of variable used to perform simulation
bool rTypeAdd = false;
bool iTypeAdd = false;
bool sTypeAdd = false;             // Variables to keep track of which instruction type each instruction is
bool bTypeAdd = false;
bool uTypeAdd = false;
bool jTypeAdd = false;

bool rTypeSub = false;
bool iTypeSub = false;
bool sTypeSub = false;             // Variables to keep track of which instruction type each instruction is
bool bTypeSub = false;
bool uTypeSub = false;
bool jTypeSub = false;

bool rTypeExecuteAdd = false;
bool iTypeExecuteAdd = false;
bool sTypeExecuteAdd = false;             // Variables to keep track of which instruction type each instruction is
bool bTypeExecuteAdd = false;
bool uTypeExecuteAdd = false;
bool jTypeExecuteAdd = false;

int iTypeCateExecuteAdd = 0;
int iTypeCateAdd = 0;          // Determines which type of I instruction it is

bool rTypeExecuteSub = false;
bool iTypeExecuteSub = false;
bool sTypeExecuteSub = false;             // Variables to keep track of which instruction type each instruction is
bool bTypeExecuteSub = false;
bool uTypeExecuteSub = false;
bool jTypeExecuteSub = false;

int iTypeCateExecuteSub = 0;
int iTypeCateSub = 0;          // Determines which type of I instruction it is

int rdAdd[5] = {};                     // Rd is x8 (0b01000)
int rs1Add[5] = {};                    // Rs1 is x9 (0b01001)
int rs2Add[5] = {};                    // Rs2 is x10 (0b01010)
int imm_ItypeAdd[12] = {};
int imm_StypeUpperAdd[7] = {};
int imm_StypeLowerAdd[5] = {};
int imm_BtypeUpperAdd[7] = {};
int imm_BtypeLowerAdd[5] = {};
int imm_UtypeAdd[20] = {};
int imm_JtypeAdd[20] = {};

int rdSub[5] = {};                     // Rd is x8 (0b01000)
int rs1Sub[5] = {};                    // Rs1 is x9 (0b01001)
int rs2Sub[5] = {};                    // Rs2 is x10 (0b01010)
int imm_ItypeSub[12] = {};
int imm_StypeUpperSub[7] = {};
int imm_StypeLowerSub[5] = {};
int imm_BtypeUpperSub[7] = {};
int imm_BtypeLowerSub[5] = {};
int imm_UtypeSub[20] = {};
int imm_JtypeSub[20] = {};

int rdToDecimalAdd = 0;                // Rd decoded decimal equivalent
int rs1ToDecimalAdd = 0;               // Rs1 decoded decimal equivalent
int rs2ToDecimalAdd = 0;               // Rs2 decoded decimal equivalent
int immToDecimalAdd = 0;               // Immediate decoded decimal equivalent

int rdToDecimalExecuteAdd = 0;                // Rd decoded decimal equivalent
int rs1ToDecimalExecuteAdd = 0;               // Rs1 decoded decimal equivalent
int rs2ToDecimalExecuteAdd = 0;               // Rs2 decoded decimal equivalent
int immToDecimalExecuteAdd = 0;               // Immediate decoded decimal equivalent

int rdToDecimalSub = 0;                // Rd decoded decimal equivalent
int rs1ToDecimalSub = 0;               // Rs1 decoded decimal equivalent
int rs2ToDecimalSub = 0;               // Rs2 decoded decimal equivalent
int immToDecimalSub = 0;               // Immediate decoded decimal equivalent

int rdToDecimalExecuteSub = 0;                // Rd decoded decimal equivalent
int rs1ToDecimalExecuteSub = 0;               // Rs1 decoded decimal equivalent
int rs2ToDecimalExecuteSub = 0;               // Rs2 decoded decimal equivalent
int immToDecimalExecuteSub = 0;               // Immediate decoded decimal equivalent

int inputArrayAdd[32] = {};   // LSB is the rightmost bit
int inputArraySub[32] = {};   // LSB is the rightmost bit


// Variables that store info/states for printing
string opcodeTypeAdd;
string opcodeTypeSub;
string instAdd;
string instSub;
string instExecuteAdd;
string instExecuteSub;
int aluOpAdd;
int branchAdd;
int memReadAdd;
int writeBackSelAdd;
int memWriteAdd;
int aluSrcAdd;
int regWriteAdd;
int signExtAdd;
int pcSelAdd;
int branchConditionAdd;
int branchSelAdd;
int aluOpSub;
int branchSub;
int memReadSub;
int writeBackSelSub;
int memWriteSub;
int aluSrcSub;
int regWriteSub;
int signExtSub;
int pcSelSub;
int branchConditionSub;
int branchSelSub;
int arrayIndexAdd = 0;
int arrayIndexSub = 0;

double clockcycleAdd = 0;
double clockcycleSub = 0;
int offsetAdd = 0;
int offsetSub = 0;
int loadNewStage = 0;
int loadNewStageAdd = 0;
int loadNewStageSub = 0;
int floatPresent = 0;
int memBusSelect = 0;

double cpiCount = 0;
double instructionAddCount = 0;
double instructionSubCount = 0;


// Call to the functions that will decode and print output
void determineOperationAdd();
void determineOperationSub();
void printEventDecodeAdd();
void printEventDecodeSub();
void assignRegisterFieldsAdd();
void assignRegisterFieldsSub();
void create32bitInstructionAdd(int instructionInLoop);
void create32bitInstructionSub(int instructionInLoop);
void decToBinaryNormal(int n, int addOrSub);
void decToBinarySplit(int n, int floatOrInt, int addOrSub);
void printEventExecuteAdd();
void printEventExecuteSub();
void fetchStageAdd(int loadPrevStage);
void decodeStageAdd(int loadPrevStage);
void executeStageAdd(int loadPrevStage);
void writebackStageAdd(int loadPrevStage);
void fetchStageSub(int loadPrevStage);
void decodeStageSub(int loadPrevStage);
void executeStageSub(int loadPrevStage);
void writebackStageSub(int loadPrevStage);
void memBus(int cpu0DataRequest, int cpu1DataRequest);
void readFromMemoryCPU0();
void readFromMemoryCPU0();
void readBinaryValue(int numSets);
void cacheSetup();
void cacheIOperationCPU0();
void cacheIOperationCPU1();
void cacheDOperationCPU0();
void cacheDOperationCPU1();

void LRUOptimizeCacheMissICPU0();
void LRUOptimizeCacheHitICPU0();

void LRUOptimizeCacheMissDCPU0();
void LRUOptimizeCacheHitDCPU0();

void LRUOptimizeCacheMissICPU1();
void LRUOptimizeCacheHitICPU1();

void LRUOptimizeCacheMissDCPU1();
void LRUOptimizeCacheHitDCPU1();



fstream myFileAdd;         // Used to open and write to instruction file
fstream myFileSub;

string lineFetchAdd;                   // Used to get Fetched instruction from list
string lineFetchSub;                   // Used to get Fetched instruction from list
string lineDecodeAdd;                  // Used to decode instruction from fetched instruction
string lineDecodeSub;                  // Used to decode instruction from fetched instruction
int tempArray[32];                  // Variable used to tranlate ASCII to integer numbers in decode process
int count32InstructionAdd = 0;         // Determines what type of instruction was created
int count32InstructionSub = 0;         // Determines what type of instruction was created
int timesFetchOccured = 0;          // Increments pipeline

int cpu0DataRequest = 0;
int cpu1DataRequest = 0;
int cpuSelect = 0;
int switchToCPU = 0;
bool normalStall0 = false;
bool normalStall1 = false;

int waitAfterDataDelayAdd = 0;
int waitAfterDataDelaySub = 0;

int runOnceAdd = 0;
int runTwiceAdd = 0;

int runOnceSub = 0;
int runTwiceSub = 0;

int stallOccured0 = 0;                       // Variable to kepp track if a stall occured
int stallOccured1 = 0;                       // Variable to kepp track if a stall occured

memoryArray A;      // Array A
memoryArray B;      // Array B
memoryArray C;      // Array C
memoryArray D;      // Array D

// Main Function
int main() {

    srand(time(0));                             // Randomizes seed
    int waitOneLoop0 = 0;                        // Variable to wait an extra loop if stall
    int waitOneLoop1 = 0;                        // Variable to wait an extra loop if stall
    int stallAmount0 = 0;                        // Variable to help determine how many stalls needed
    int stallAmount1 = 1;                        // Variable to help determine how many stalls needed

    cacheSetup();

    for (int i = 0; i < 256; i++) {             // Loop randomly assigns values to Array A and Array B

        A.mainArrayAddress[i] = ARRAY_A + (0x1) * i;        // Maps all the addresses to Array A
        B.mainArrayAddress[i] = ARRAY_B + (0x1) * i;        // Maps all the addresses to Array B
        C.mainArrayAddress[i] = ARRAY_C + (0x1) * i;        // Maps all the addresses to Array C
        D.mainArrayAddress[i] = ARRAY_D + (0x1) * i;

        int floatOrInt = rand() % 2;


        if (floatOrInt == 1) {                                      // If 1 assigns float value
            A.mainArray[i] = rand() / float(RAND_MAX) * 25.f + 1.f;
            B.mainArray[i] = rand() / float(RAND_MAX) * 25.f + 1.f;
        }
        else {                                                      // If 0 assigns int value
            A.mainArray[i] = rand() % 25;
            B.mainArray[i] = rand() % 25;
        }

    }


    while (arrayIndexAdd < 257 && arrayIndexSub < 257 && ((count32InstructionAdd != 2 && count32InstructionSub != 2) || (arrayIndexAdd != 256 || arrayIndexSub != 256))) {          // Runs for all index values



        cout << endl << "##############################################################" << endl;

        if (loadNewStage == 0) {
            loadNewStageAdd = 0;
            loadNewStageSub = 0;
            writebackStageAdd(loadNewStageAdd);
            writebackStageSub(loadNewStageSub);
        }
        else if (loadNewStage == 2) {                // In case of stall
            writebackStageAdd(loadNewStageAdd);
            writebackStageSub(loadNewStageSub);
            waitOneLoop0 = 1;
            waitOneLoop1 = 1;
        }
        else if (loadNewStage == 1) {
            if ((timesFetchOccured >= 0 && loadNewStage != 0) || (waitOneLoop0 == 1) || (waitOneLoop1 == 1)) {          // In case of normal operation
                loadNewStage = 1;
                loadNewStageAdd = 1;
                loadNewStageSub = 1;
                writebackStageAdd(loadNewStageAdd);
                writebackStageSub(loadNewStageSub);

                if (count32InstructionAdd == 3) {          // Create stall after executing load A (if integer)

                    if (instExecuteAdd == "Lb ") {
                        normalStall0 = true;
                        loadNewStage = 2;
                        loadNewStageAdd = 2;
                    }
                    else if (instExecuteAdd == "Flw ") {
                        loadNewStage = 1;
                        loadNewStageAdd = 1;
                    }

                    if (instExecuteSub == "Lb ") {
                        normalStall1 = true;
                        loadNewStage = 2;
                        loadNewStageSub = 2;
                    }
                    else if (instExecuteSub == "Flw ") {
                        loadNewStage = 1;
                        loadNewStageSub = 1;
                    }


                }
                else if (count32InstructionAdd == 4) {   // Create stall after executing load B (both float and integer)

                    if (instExecuteAdd == "Add ") {
                        normalStall0 = true;
                        loadNewStage = 2;
                        loadNewStageAdd = 2;
                    }
                    else if (instExecuteAdd == "FADD.S ") {
                        normalStall0 = true;
                        loadNewStage = 2;
                        loadNewStageAdd = 2;
                    }

                    if (instExecuteSub == "Sub ") {
                        normalStall1 = true;
                        loadNewStage = 2;
                        loadNewStageSub = 2;
                    }
                    else if (instExecuteSub == "FSUB.S ") {
                        normalStall1 = true;
                        loadNewStage = 2;
                        loadNewStageSub = 2;
                    }

                }
                else if (count32InstructionAdd == 5) {   // Create stall after executing fload addition

                    if (instExecuteAdd == "Sb ") {
                        loadNewStage = 1;
                        loadNewStageAdd = 1;
                    }
                    else if (instExecuteAdd == "Fsw ") {
                        stallAmount0++;
                        normalStall0 = true;
                        loadNewStage = 2;
                        loadNewStageAdd = 2;
                    }

                    if (instExecuteSub == "Sb ") {
                        loadNewStage = 1;
                        loadNewStageSub = 1;
                    }
                    else if (instExecuteSub == "Fsw ") {
                        stallAmount1++;
                        normalStall1 = true;
                        loadNewStage = 2;
                        loadNewStageSub = 2;
                    }


                }
            }

            if (waitOneLoop0 == 1 && stallAmount0 == 0) {             // If no stall is no longer needed
                loadNewStage = 1;
                loadNewStageAdd = 1;
                waitOneLoop0 = 0;
            }
            else if (waitOneLoop0 == 1 && stallAmount0 == 1) {        // If one stall is needed
                loadNewStage = 2;
                loadNewStageAdd = 2;
            }
            else if (waitOneLoop0 == 1 && stallAmount0 == 3) {        // If two stalls are needed
                loadNewStage = 1;
                loadNewStageAdd = 1;
                waitOneLoop0 = 0;
                stallAmount0 = 0;
            }

            if (waitOneLoop1 == 1 && stallAmount1 == 0) {             // If no stall is no longer needed
                loadNewStage = 1;
                loadNewStageSub = 1;
                waitOneLoop1 = 0;
            }
            else if (waitOneLoop1 == 1 && stallAmount1 == 1) {        // If one stall is needed
                loadNewStage = 2;
                loadNewStageSub = 2;
            }
            else if (waitOneLoop1 == 1 && stallAmount1 == 3) {        // If two stalls are needed
                loadNewStage = 1;
                loadNewStageSub = 1;
                waitOneLoop1 = 0;
                stallAmount1 = 0;
            }

        }
        else {                                      // In case of no operation

            writebackStageAdd(3);
            writebackStageSub(3);
        }


        memBus(cpu0DataRequest, cpu1DataRequest);

        if (loadNewStage == 3) {

            executeStageAdd(loadNewStageAdd);
            executeStageSub(loadNewStageSub);

        }
        else {

            if (loadNewStageAdd == 2 || loadNewStageSub == 2) {            // In case of stall

                executeStageAdd(loadNewStageAdd);
                executeStageSub(loadNewStageSub);

            }
            else {
                if (timesFetchOccured >= 2) {         // In case of normal operation

                    loadNewStage = 1;
                    loadNewStageAdd = 1;
                    loadNewStageSub = 1;
                    executeStageAdd(loadNewStageAdd);
                    executeStageSub(loadNewStageSub);

                }
                else {                                 // In case of no operation
                    loadNewStage = 0;
                    loadNewStageAdd = 0;
                    loadNewStageSub = 0;
                    executeStageAdd(loadNewStageAdd);
                    executeStageSub(loadNewStageSub);
                }
            }
        }

        if (loadNewStage == 3) {

            decodeStageAdd(loadNewStageAdd);
            decodeStageSub(loadNewStageSub);

        }
        else {


            if (loadNewStageAdd == 2 || loadNewStageSub == 2) {                // In case of stall

                decodeStageAdd(loadNewStageAdd);
                decodeStageSub(loadNewStageSub);

            }
            else {
                if (timesFetchOccured >= 1) {       // In case of normal operation 
                    loadNewStage = 1;
                    loadNewStageAdd = 1;
                    loadNewStageSub = 1;
                    decodeStageAdd(loadNewStageAdd);
                    decodeStageSub(loadNewStageSub);
                }
                else {
                    loadNewStage = 0;               // In case of no operation
                    loadNewStageAdd = 0;
                    loadNewStageSub = 0;
                    decodeStageAdd(loadNewStageAdd);
                    decodeStageSub(loadNewStageSub);
                }
            }
        }

        if (loadNewStage == 3) {

            fetchStageAdd(loadNewStageAdd);
            fetchStageSub(loadNewStageSub);

            if (loadNewStageAdd == 1 && loadNewStageSub == 3) {
                cpu0DataRequest = 0;
            }
            else if (loadNewStageAdd == 3 && loadNewStageSub == 1) {
                cpu1DataRequest = 0;
            }

        }
        else {

            if (loadNewStageAdd == 2 || loadNewStageSub == 2) {                // In case of stall

                fetchStageAdd(loadNewStageAdd);
                fetchStageSub(loadNewStageSub);
                loadNewStage = 1;
                loadNewStageAdd = 1;
                loadNewStageSub = 1;
                if (normalStall0 == true) {
                    waitOneLoop0 = 1;
                    stallOccured0 = 1;
                }

                if (normalStall1 == true) {
                    waitOneLoop1 = 1;
                    stallOccured1 = 1;
                }

                normalStall0 = false;
                normalStall1 = false;
            }
            else {
                if (timesFetchOccured >= 0) {       // In case of normal operation
                    loadNewStage = 1;
                    loadNewStageAdd = 1;
                    loadNewStageSub = 1;
                    fetchStageAdd(loadNewStageAdd);
                    fetchStageSub(loadNewStageSub);
                }
                else {                              // In case of no operation
                    loadNewStage = 0;
                    loadNewStageAdd = 0;
                    loadNewStageSub = 0;
                    fetchStageAdd(loadNewStageAdd);
                    fetchStageSub(loadNewStageSub);
                }
            }
        }

        if (instExecuteAdd == "Lb " || instExecuteAdd == "Flw " || instExecuteAdd == "Sb " || instExecuteAdd == "Fsw ") {           // Send data request signals to membus for CPU 0
            cpu0DataRequest = 1;
        }
        else {
            cpu0DataRequest = 0;
        }

        if (instExecuteSub == "Lb " || instExecuteSub == "Flw " || instExecuteSub == "Sb " || instExecuteSub == "Fsw ") {           // Send data request signals to membus for CPU 1
            cpu1DataRequest = 1;
        }
        else {
            cpu1DataRequest = 0;
        }


        timesFetchOccured++;        // Increment pipeline

        if ((count32InstructionAdd == 3 || count32InstructionAdd == 4 || count32InstructionAdd == 6) && floatPresent == 0 && stallOccured0 == 0) {        // Update clockcycleAdd according to instruction last processed
            clockcycleAdd += 110;            // 1 cpu (operation) + 10 cpu (mem access) + 4 tick (data cache access) + 3 tick (instruction cache access time)

            if (hitOrMissCacheD0 == 0) {
                clockcycleAdd += 4;
            }

            if (hitOrMissCacheI0 == 0) {
                clockcycleAdd += 3;
            }

        }
        else if ((count32InstructionAdd == 3 || count32InstructionAdd == 4 || count32InstructionAdd == 6) && floatPresent == 1 && stallOccured0 == 0) {
            clockcycleAdd += 150;            // 5 cpu (operation) + 10 cpu (mem access) + 4 tick (data cache access) + 3 tick (instruction cache access time)

            if (hitOrMissCacheD0 == 0) {
                clockcycleAdd += 4;
            }

            if (hitOrMissCacheI0 == 0) {
                clockcycleAdd += 3;
            }
        }
        else if ((count32InstructionAdd == 5 && floatPresent == 1 && stallOccured0 == 0)) {
            clockcycleAdd += 50;             // 5 cpu (normal float add operation) + 3 tick (instruction cache access time)

            if (hitOrMissCacheI0 == 0) {
                clockcycleAdd += 3;
            }
        }
        else {
            stallOccured0 = 0;
            clockcycleAdd += 10;             // 1 cpu (normal addi or bne operation) + 3 tick (instruction access time)

            if (hitOrMissCacheI0 == 0) {
                clockcycleAdd += 3;
            }
        }

        if ((count32InstructionSub == 3 || count32InstructionSub == 4 || count32InstructionSub == 6) && floatPresent == 0 && stallOccured1 == 0) {        // Update clockcycle according to instruction last processed
            clockcycleSub += 110;            // 1 cpu (operation) + 10 cpu (mem access) + 4 tick (data cache access) + 3 tick (instruction cache access time)

            if (hitOrMissCacheD1 == 0) {
                clockcycleSub += 4;
            }

            if (hitOrMissCacheI1 == 0) {
                clockcycleSub += 3;
            }
        }
        else if ((count32InstructionSub == 3 || count32InstructionSub == 4 || count32InstructionSub == 6) && floatPresent == 1 && stallOccured1 == 0) {
            clockcycleSub += 150;            // 5 cpu (operation) + 10 cpu (mem access) + 4 tick (data cache access) + 3 tick (instruction cache access time)

            if (hitOrMissCacheD1 == 0) {
                clockcycleSub += 4;
            }

            if (hitOrMissCacheI1 == 0) {
                clockcycleSub += 3;
            }
        }
        else if ((count32InstructionSub == 5 && floatPresent == 1 && stallOccured1 == 0)) {
            clockcycleSub += 50;             // 5 cpu (normal float sub operation) + 3 tick (instruction cache access time)

            if (hitOrMissCacheI1 == 0) {
                clockcycleSub += 3;
            }
        }
        else {
            stallOccured1 = 0;
            clockcycleSub += 10;             // 1 cpu (normal addi or bne operation) + 3 tick (instruction cache access time)

            if (hitOrMissCacheI1 == 0) {
                clockcycleSub += 3;
            }
        }

        hitOrMissCacheD0 = -1;
        hitOrMissCacheD1 = -1;
        hitOrMissCacheI0 = -1;
        hitOrMissCacheI1 = -1;

        loadNewStageAdd = 1;
        loadNewStageSub = 1;

        cout << endl << "##############################################################" << endl;


    }

    cpiCount = (clockcycleAdd - 1) / instructionAddCount;      // Subtract one to make it current printed clock cycle

    printf("\nCPI Count for CPU 0 is %f (ticks)", cpiCount);

    cpiCount = (clockcycleSub - 1) / instructionSubCount;      // Subtract one to make it current printed clock cycle

    printf("\nCPI Count for CPU 1 is %f (ticks)\n", cpiCount);

    cout << endl;

    cacheICPU0.lookUpTime = (ceil(log2(cacheICPU0.capacitySize / cacheICPU0.cacheLineSize)) * cacheICPU0.cacheAssociativity) + (cacheICPU0.numberOfMisses * 100);
    cacheDCPU0.lookUpTime = (ceil(log2(cacheDCPU0.capacitySize / cacheDCPU0.cacheLineSize)) * cacheDCPU0.cacheAssociativity) + (cacheDCPU0.numberOfMisses * 100);
    cacheICPU1.lookUpTime = (ceil(log2(cacheICPU1.capacitySize / cacheICPU1.cacheLineSize)) * cacheICPU1.cacheAssociativity) + (cacheICPU1.numberOfMisses * 100);
    cacheDCPU1.lookUpTime = (ceil(log2(cacheDCPU1.capacitySize / cacheDCPU1.cacheLineSize)) * cacheDCPU1.cacheAssociativity) + (cacheDCPU1.numberOfMisses * 100);


    printf("Cache I CPU 0:\n  Hit Rate: %f\n  Compulsory Miss Rate: %f\n  Total Trace Simulation Time: %d (ticks)\n\n", ((double)cacheICPU0.numberOfHits / (double)cacheICPU0.totalAccesses) * 100, ((double)cacheICPU0.numberOfMisses / (double)cacheICPU0.totalAccesses) * 100, cacheICPU0.lookUpTime);
    printf("Cache D CPU 0:\n  Hit Rate: %f\n  Compulsory Miss Rate: %f\n  Total Trace Simulation Time: %d (ticks)\n\n", ((double)cacheDCPU0.numberOfHits / (double)cacheDCPU0.totalAccesses) * 100, ((double)cacheDCPU0.numberOfMisses / (double)cacheDCPU0.totalAccesses) * 100, cacheDCPU0.lookUpTime);
    printf("Cache I CPU 1:\n  Hit Rate: %f\n  Compulsory Miss Rate: %f\n  Total Trace Simulation Time: %d (ticks)\n\n", ((double)cacheICPU1.numberOfHits / (double)cacheICPU1.totalAccesses) * 100, ((double)cacheICPU1.numberOfMisses / (double)cacheICPU1.totalAccesses) * 100, cacheICPU1.lookUpTime);
    printf("Cache D CPU 1:\n  Hit Rate: %f\n  Compulsory Miss Rate: %f\n  Total Trace Simulation Time: %d (ticks)\n\n", ((double)cacheDCPU1.numberOfHits / (double)cacheDCPU1.totalAccesses) * 100, ((double)cacheDCPU1.numberOfMisses / (double)cacheDCPU1.totalAccesses) * 100, cacheDCPU1.lookUpTime);


    //printf("\nAddress dump:\n\n");

    //for (int i = 0; i < 256; i++) {         // Address Dump

    //    printf("Array A Address Value %p: %f        Array B Address Value %p: %f        Array C Address Value %p: %f        Array D Address Value %p: %f\n", A.mainArrayAddress[i], A.mainArray[i], B.mainArrayAddress[i], B.mainArray[i], C.mainArrayAddress[i], C.mainArray[i], D.mainArrayAddress[i], D.mainArray[i]);
    //}

    return 0;
}

// Function that displays the output of each instruction including control signals (CPU 0)
void printEventDecodeAdd() {

    cout << "Opcode Type: " << opcodeTypeAdd << endl;

    cout << "Inst: " << instAdd;

    if (rTypeAdd == true) {
        printf("x%d, x%d, x%d \n", rdToDecimalAdd, rs1ToDecimalAdd, rs2ToDecimalAdd);        // Displays the correct instruction structure based on type of instruction
    }
    else if (iTypeAdd == true) {
        if (iTypeCateAdd == 1) {
            printf("x%d, x%d, %d \n", rdToDecimalAdd, rs1ToDecimalAdd, immToDecimalAdd);
        }
        else if (iTypeCateAdd == 2) {
            printf("x%d, %d(x%d) \n", rdToDecimalAdd, immToDecimalAdd, rs1ToDecimalAdd);
        }
    }
    else if (sTypeAdd == true) {
        printf("x%d, %d(x%d) \n", rs2ToDecimalAdd, immToDecimalAdd, rs1ToDecimalAdd);
    }
    else if (bTypeAdd == true) {
        printf("x%d, x%d, Loop \n", rs1ToDecimalAdd, rs2ToDecimalAdd);
    }
    else if (uTypeAdd == true) {
        printf("x%d, %d \n", rdToDecimalAdd, immToDecimalAdd);
    }
    else if (jTypeAdd == true) {
        printf("x%d, %d(x%d) \n", rdToDecimalAdd, immToDecimalAdd, rs1ToDecimalAdd);
    }

    cout << "Control Signals:" << endl;
    cout << "Reg_Write_Enable: " << regWriteAdd << ", Reg_Read_Enable: " << regWriteAdd << ", Alu_op: " << aluOpAdd << endl;                   // Outputs active/non-active control signals
    cout << "Mem_Write_Enable: " << memWriteAdd << ", Mem_Read_Enable: " << memReadAdd << ", Writeback_Enable: " << writeBackSelAdd << endl;
    cout << "Imm_Sel: " << aluSrcAdd << ", Sign_Extend: " << signExtAdd << ", branchAdd: " << branchAdd << ", Branch_Condition: " << branchConditionAdd << endl;
    cout << "Branch_Select: " << branchSelAdd << ", PC_Select " << pcSelAdd << endl;

    instExecuteAdd = instAdd;
    rdToDecimalExecuteAdd = rdToDecimalAdd;
    rs1ToDecimalExecuteAdd = rs1ToDecimalAdd;
    rs2ToDecimalExecuteAdd = rs2ToDecimalAdd;
    immToDecimalExecuteAdd = immToDecimalAdd;

    rTypeExecuteAdd = rTypeAdd;
    iTypeExecuteAdd = iTypeAdd;
    sTypeExecuteAdd = sTypeAdd;
    bTypeExecuteAdd = bTypeAdd;
    uTypeExecuteAdd = uTypeAdd;
    jTypeExecuteAdd = jTypeAdd;
    iTypeCateExecuteAdd = iTypeCateAdd;
}

// Function that displays the output of each instruction including control signals (CPU 1)
void printEventDecodeSub() {

    cout << "Opcode Type: " << opcodeTypeSub << endl;

    cout << "Inst: " << instSub;

    if (rTypeSub == true) {
        printf("x%d, x%d, x%d \n", rdToDecimalSub, rs1ToDecimalSub, rs2ToDecimalSub);        // Displays the correct instruction structure based on type of instruction
    }
    else if (iTypeSub == true) {
        if (iTypeCateSub == 1) {
            printf("x%d, x%d, %d \n", rdToDecimalSub, rs1ToDecimalSub, immToDecimalSub);
        }
        else if (iTypeCateSub == 2) {
            printf("x%d, %d(x%d) \n", rdToDecimalSub, immToDecimalSub, rs1ToDecimalSub);
        }
    }
    else if (sTypeSub == true) {
        printf("x%d, %d(x%d) \n", rs2ToDecimalSub, immToDecimalSub, rs1ToDecimalSub);
    }
    else if (bTypeSub == true) {
        printf("x%d, x%d, Loop \n", rs1ToDecimalSub, rs2ToDecimalSub);
    }
    else if (uTypeSub == true) {
        printf("x%d, %d \n", rdToDecimalSub, immToDecimalSub);
    }
    else if (jTypeSub == true) {
        printf("x%d, %d(x%d) \n", rdToDecimalSub, immToDecimalSub, rs1ToDecimalSub);
    }

    cout << "Control Signals:" << endl;
    cout << "Reg_Write_Enable: " << regWriteSub << ", Reg_Read_Enable: " << regWriteSub << ", Alu_op: " << aluOpSub << endl;                   // Outputs active/non-active control signals
    cout << "Mem_Write_Enable: " << memWriteSub << ", Mem_Read_Enable: " << memReadSub << ", Writeback_Enable: " << writeBackSelSub << endl;
    cout << "Imm_Sel: " << aluSrcSub << ", Sign_Extend: " << signExtSub << ", branchAdd: " << branchSub << ", Branch_Condition: " << branchConditionSub << endl;
    cout << "Branch_Select: " << branchSelSub << ", PC_Select " << pcSelSub << endl;

    instExecuteSub = instSub;
    rdToDecimalExecuteSub = rdToDecimalSub;
    rs1ToDecimalExecuteSub = rs1ToDecimalSub;
    rs2ToDecimalExecuteSub = rs2ToDecimalSub;
    immToDecimalExecuteSub = immToDecimalSub;

    rTypeExecuteSub = rTypeSub;
    iTypeExecuteSub = iTypeSub;
    sTypeExecuteSub = sTypeSub;
    bTypeExecuteSub = bTypeSub;
    uTypeExecuteSub = uTypeSub;
    jTypeExecuteSub = jTypeSub;
    iTypeCateExecuteSub = iTypeCateSub;
}

// Function that displays the output of execute stage (CPU 0)
void printEventExecuteAdd() {

    cout << endl << "--------------------------------------------------------------" << endl;
    cout << endl << "CPU 0 Execute Stage: " << endl;

    cout << "Instruction: " << instExecuteAdd;

    if (rTypeExecuteAdd == true) {
        printf("x%d, x%d, x%d \n", rdToDecimalExecuteAdd, rs1ToDecimalExecuteAdd, rs2ToDecimalExecuteAdd);        // Displays the correct instruction structure based on type of instruction
    }
    else if (iTypeExecuteAdd == true) {
        if (iTypeCateExecuteAdd == 1) {
            printf("x%d, x%d, %d \n", rdToDecimalExecuteAdd, rs1ToDecimalExecuteAdd, immToDecimalExecuteAdd);
        }
        else if (iTypeCateExecuteAdd == 2) {
            printf("x%d, %d(x%d) \n", rdToDecimalExecuteAdd, immToDecimalExecuteAdd, rs1ToDecimalExecuteAdd);
        }
    }
    else if (sTypeExecuteAdd == true) {
        printf("x%d, %d(x%d) \n", rs2ToDecimalExecuteAdd, immToDecimalExecuteAdd, rs1ToDecimalExecuteAdd);
    }
    else if (bTypeExecuteAdd == true) {
        printf("x%d, x%d, Loop \n", rs1ToDecimalExecuteAdd, rs2ToDecimalExecuteAdd);
    }
    else if (uTypeExecuteAdd == true) {
        printf("x%d, %d \n", rdToDecimalExecuteAdd, immToDecimalExecuteAdd);
    }
    else if (jTypeExecuteAdd == true) {
        printf("x%d, %d(x%d) \n", rdToDecimalExecuteAdd, immToDecimalExecuteAdd, rs1ToDecimalExecuteAdd);
    }

    cout << "Clock Cycle: " << clockcycleAdd << endl;

    instructionAddCount++;
}

// Function that displays the output of execute stage (CPU 1)
void printEventExecuteSub() {

    cout << endl << "--------------------------------------------------------------" << endl;
    cout << endl << "CPU 1 Execute Stage: " << endl;

    cout << "Instruction: " << instExecuteSub;

    if (rTypeExecuteSub == true) {
        printf("x%d, x%d, x%d \n", rdToDecimalExecuteSub, rs1ToDecimalExecuteSub, rs2ToDecimalExecuteSub);        // Displays the correct instruction structure based on type of instruction
    }
    else if (iTypeExecuteSub == true) {
        if (iTypeCateExecuteSub == 1) {
            printf("x%d, x%d, %d \n", rdToDecimalExecuteSub, rs1ToDecimalExecuteSub, immToDecimalExecuteSub);
        }
        else if (iTypeCateExecuteSub == 2) {
            printf("x%d, %d(x%d) \n", rdToDecimalExecuteSub, immToDecimalExecuteSub, rs1ToDecimalExecuteSub);
        }
    }
    else if (sTypeExecuteSub == true) {
        printf("x%d, %d(x%d) \n", rs2ToDecimalExecuteSub, immToDecimalExecuteSub, rs1ToDecimalExecuteSub);
    }
    else if (bTypeExecuteSub == true) {
        printf("x%d, x%d, Loop \n", rs1ToDecimalExecuteSub, rs2ToDecimalExecuteSub);
    }
    else if (uTypeExecuteSub == true) {
        printf("x%d, %d \n", rdToDecimalExecuteSub, immToDecimalExecuteSub);
    }
    else if (jTypeExecuteSub == true) {
        printf("x%d, %d(x%d) \n", rdToDecimalExecuteSub, immToDecimalExecuteSub, rs1ToDecimalExecuteSub);
    }

    cout << "Clock Cycle: " << clockcycleSub << endl;

    instructionSubCount++;
}

// Function that maps and decodes the various parts of the input instruction based on instruction types (CPU 0)
void assignRegisterFieldsAdd() {

    if (rTypeAdd == true) {
        rdAdd[0] = inputArrayAdd[24];         // Decodes register destination
        rdAdd[1] = inputArrayAdd[23];
        rdAdd[2] = inputArrayAdd[22];
        rdAdd[3] = inputArrayAdd[21];
        rdAdd[4] = inputArrayAdd[20];

        rs1Add[0] = inputArrayAdd[16];        // Decodes register 1 field
        rs1Add[1] = inputArrayAdd[15];
        rs1Add[2] = inputArrayAdd[14];
        rs1Add[3] = inputArrayAdd[13];
        rs1Add[4] = inputArrayAdd[12];

        rs2Add[0] = inputArrayAdd[11];        // Decodes register 2 field
        rs2Add[1] = inputArrayAdd[10];
        rs2Add[2] = inputArrayAdd[9];
        rs2Add[3] = inputArrayAdd[8];
        rs2Add[4] = inputArrayAdd[7];
    }
    else if (iTypeAdd == true) {
        rdAdd[0] = inputArrayAdd[24];         // Decodes register destination
        rdAdd[1] = inputArrayAdd[23];
        rdAdd[2] = inputArrayAdd[22];
        rdAdd[3] = inputArrayAdd[21];
        rdAdd[4] = inputArrayAdd[20];

        rs1Add[0] = inputArrayAdd[16];        // Decodes register 1 field
        rs1Add[1] = inputArrayAdd[15];
        rs1Add[2] = inputArrayAdd[14];
        rs1Add[3] = inputArrayAdd[13];
        rs1Add[4] = inputArrayAdd[12];

        imm_ItypeAdd[0] = inputArrayAdd[11];      // Decodes immediate value
        imm_ItypeAdd[1] = inputArrayAdd[10];
        imm_ItypeAdd[2] = inputArrayAdd[9];
        imm_ItypeAdd[3] = inputArrayAdd[8];
        imm_ItypeAdd[4] = inputArrayAdd[7];
        imm_ItypeAdd[5] = inputArrayAdd[6];
        imm_ItypeAdd[6] = inputArrayAdd[5];
        imm_ItypeAdd[7] = inputArrayAdd[4];
        imm_ItypeAdd[8] = inputArrayAdd[3];
        imm_ItypeAdd[9] = inputArrayAdd[2];
        imm_ItypeAdd[10] = inputArrayAdd[1];
        imm_ItypeAdd[11] = inputArrayAdd[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalAdd += ((imm_ItypeAdd[i]) * pow(2, i));
        }

    }
    else if (sTypeAdd == true) {
        imm_StypeLowerAdd[0] = inputArrayAdd[24];         // Decodes immediate value
        imm_StypeLowerAdd[1] = inputArrayAdd[23];
        imm_StypeLowerAdd[2] = inputArrayAdd[22];
        imm_StypeLowerAdd[3] = inputArrayAdd[21];
        imm_StypeLowerAdd[4] = inputArrayAdd[20];

        rs1Add[0] = inputArrayAdd[16];                    // Decodes register 1 field
        rs1Add[1] = inputArrayAdd[15];
        rs1Add[2] = inputArrayAdd[14];
        rs1Add[3] = inputArrayAdd[13];
        rs1Add[4] = inputArrayAdd[12];

        rs2Add[0] = inputArrayAdd[11];                    // Decodes register 2 field
        rs2Add[1] = inputArrayAdd[10];
        rs2Add[2] = inputArrayAdd[9];
        rs2Add[3] = inputArrayAdd[8];
        rs2Add[4] = inputArrayAdd[7];

        imm_StypeUpperAdd[0] = inputArrayAdd[6];          // Decodes immediate value
        imm_StypeUpperAdd[1] = inputArrayAdd[5];
        imm_StypeUpperAdd[2] = inputArrayAdd[4];
        imm_StypeUpperAdd[3] = inputArrayAdd[3];
        imm_StypeUpperAdd[4] = inputArrayAdd[2];
        imm_StypeUpperAdd[5] = inputArrayAdd[1];
        imm_StypeUpperAdd[6] = inputArrayAdd[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimalAdd += ((imm_StypeUpperAdd[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimalAdd += ((imm_StypeLowerAdd[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (bTypeAdd == true) {
        imm_BtypeLowerAdd[0] = inputArrayAdd[24];       // Decodes immediate value
        imm_BtypeLowerAdd[1] = inputArrayAdd[23];
        imm_BtypeLowerAdd[2] = inputArrayAdd[22];
        imm_BtypeLowerAdd[3] = inputArrayAdd[21];
        imm_BtypeLowerAdd[4] = inputArrayAdd[20];

        rs1Add[0] = inputArrayAdd[16];                  // Decodes register 1 field
        rs1Add[1] = inputArrayAdd[15];
        rs1Add[2] = inputArrayAdd[14];
        rs1Add[3] = inputArrayAdd[13];
        rs1Add[4] = inputArrayAdd[12];

        rs2Add[0] = inputArrayAdd[11];                  // Decodes register 2 field
        rs2Add[1] = inputArrayAdd[10];
        rs2Add[2] = inputArrayAdd[9];
        rs2Add[3] = inputArrayAdd[8];
        rs2Add[4] = inputArrayAdd[7];

        imm_BtypeUpperAdd[0] = inputArrayAdd[6];        // Decodes immediate value
        imm_BtypeUpperAdd[1] = inputArrayAdd[5];
        imm_BtypeUpperAdd[2] = inputArrayAdd[4];
        imm_BtypeUpperAdd[3] = inputArrayAdd[3];
        imm_BtypeUpperAdd[4] = inputArrayAdd[2];
        imm_BtypeUpperAdd[5] = inputArrayAdd[1];
        imm_BtypeUpperAdd[6] = inputArrayAdd[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimalAdd += ((imm_BtypeUpperAdd[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimalAdd += ((imm_BtypeLowerAdd[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (uTypeAdd == true) {
        rdAdd[0] = inputArrayAdd[24];                     // Decodes register destination
        rdAdd[1] = inputArrayAdd[23];
        rdAdd[2] = inputArrayAdd[22];
        rdAdd[3] = inputArrayAdd[21];
        rdAdd[4] = inputArrayAdd[20];

        imm_UtypeAdd[0] = inputArrayAdd[19];              // Decodes immediate value
        imm_UtypeAdd[1] = inputArrayAdd[18];
        imm_UtypeAdd[2] = inputArrayAdd[17];
        imm_UtypeAdd[3] = inputArrayAdd[16];
        imm_UtypeAdd[4] = inputArrayAdd[15];
        imm_UtypeAdd[5] = inputArrayAdd[14];
        imm_UtypeAdd[6] = inputArrayAdd[13];
        imm_UtypeAdd[7] = inputArrayAdd[12];
        imm_UtypeAdd[8] = inputArrayAdd[11];
        imm_UtypeAdd[9] = inputArrayAdd[10];
        imm_UtypeAdd[10] = inputArrayAdd[9];
        imm_UtypeAdd[11] = inputArrayAdd[8];
        imm_UtypeAdd[12] = inputArrayAdd[7];
        imm_UtypeAdd[13] = inputArrayAdd[6];
        imm_UtypeAdd[14] = inputArrayAdd[5];
        imm_UtypeAdd[15] = inputArrayAdd[4];
        imm_UtypeAdd[16] = inputArrayAdd[3];
        imm_UtypeAdd[17] = inputArrayAdd[2];
        imm_UtypeAdd[18] = inputArrayAdd[1];
        imm_UtypeAdd[19] = inputArrayAdd[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalAdd += ((imm_UtypeAdd[i]) * pow(2, i));
        }

    }
    else if (jTypeAdd == true) {
        rdAdd[0] = inputArrayAdd[24];                 // Decodes register destination
        rdAdd[1] = inputArrayAdd[23];
        rdAdd[2] = inputArrayAdd[22];
        rdAdd[3] = inputArrayAdd[21];
        rdAdd[4] = inputArrayAdd[20];

        imm_JtypeAdd[0] = inputArrayAdd[19];          // Decodes immediate value
        imm_JtypeAdd[1] = inputArrayAdd[18];
        imm_JtypeAdd[2] = inputArrayAdd[17];
        imm_JtypeAdd[3] = inputArrayAdd[16];
        imm_JtypeAdd[4] = inputArrayAdd[15];
        imm_JtypeAdd[5] = inputArrayAdd[14];
        imm_JtypeAdd[6] = inputArrayAdd[13];
        imm_JtypeAdd[7] = inputArrayAdd[12];
        imm_JtypeAdd[8] = inputArrayAdd[11];
        imm_JtypeAdd[9] = inputArrayAdd[10];
        imm_JtypeAdd[10] = inputArrayAdd[9];
        imm_JtypeAdd[11] = inputArrayAdd[8];
        imm_JtypeAdd[12] = inputArrayAdd[7];
        imm_JtypeAdd[13] = inputArrayAdd[6];
        imm_JtypeAdd[14] = inputArrayAdd[5];
        imm_JtypeAdd[15] = inputArrayAdd[4];
        imm_JtypeAdd[16] = inputArrayAdd[3];
        imm_JtypeAdd[17] = inputArrayAdd[2];
        imm_JtypeAdd[18] = inputArrayAdd[1];
        imm_JtypeAdd[19] = inputArrayAdd[0];

        for (int i = 19; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalAdd += ((imm_JtypeAdd[i]) * pow(2, i));
        }

    }

    for (int i = 4; i > -1; i--) {
        rdToDecimalAdd += ((rdAdd[i]) * pow(2, i));       // Decodes register destination (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs1ToDecimalAdd += ((rs1Add[i]) * pow(2, i));     // Decodes register 1 location (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs2ToDecimalAdd += ((rs2Add[i]) * pow(2, i));     // Decodes register 2 location (coverts to decimal)
    }

}

// Function that maps and decodes the various parts of the input instruction based on instruction types (CPU 1)
void assignRegisterFieldsSub() {

    if (rTypeSub == true) {
        rdSub[0] = inputArraySub[24];         // Decodes register destination
        rdSub[1] = inputArraySub[23];
        rdSub[2] = inputArraySub[22];
        rdSub[3] = inputArraySub[21];
        rdSub[4] = inputArraySub[20];

        rs1Sub[0] = inputArraySub[16];        // Decodes register 1 field
        rs1Sub[1] = inputArraySub[15];
        rs1Sub[2] = inputArraySub[14];
        rs1Sub[3] = inputArraySub[13];
        rs1Sub[4] = inputArraySub[12];

        rs2Sub[0] = inputArraySub[11];        // Decodes register 2 field
        rs2Sub[1] = inputArraySub[10];
        rs2Sub[2] = inputArraySub[9];
        rs2Sub[3] = inputArraySub[8];
        rs2Sub[4] = inputArraySub[7];
    }
    else if (iTypeSub == true) {
        rdSub[0] = inputArraySub[24];         // Decodes register destination
        rdSub[1] = inputArraySub[23];
        rdSub[2] = inputArraySub[22];
        rdSub[3] = inputArraySub[21];
        rdSub[4] = inputArraySub[20];

        rs1Sub[0] = inputArraySub[16];        // Decodes register 1 field
        rs1Sub[1] = inputArraySub[15];
        rs1Sub[2] = inputArraySub[14];
        rs1Sub[3] = inputArraySub[13];
        rs1Sub[4] = inputArraySub[12];

        imm_ItypeSub[0] = inputArraySub[11];      // Decodes immediate value
        imm_ItypeSub[1] = inputArraySub[10];
        imm_ItypeSub[2] = inputArraySub[9];
        imm_ItypeSub[3] = inputArraySub[8];
        imm_ItypeSub[4] = inputArraySub[7];
        imm_ItypeSub[5] = inputArraySub[6];
        imm_ItypeSub[6] = inputArraySub[5];
        imm_ItypeSub[7] = inputArraySub[4];
        imm_ItypeSub[8] = inputArraySub[3];
        imm_ItypeSub[9] = inputArraySub[2];
        imm_ItypeSub[10] = inputArraySub[1];
        imm_ItypeSub[11] = inputArraySub[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalSub += ((imm_ItypeSub[i]) * pow(2, i));
        }

    }
    else if (sTypeSub == true) {
        imm_StypeLowerSub[0] = inputArraySub[24];         // Decodes immediate value
        imm_StypeLowerSub[1] = inputArraySub[23];
        imm_StypeLowerSub[2] = inputArraySub[22];
        imm_StypeLowerSub[3] = inputArraySub[21];
        imm_StypeLowerSub[4] = inputArraySub[20];

        rs1Sub[0] = inputArraySub[16];                    // Decodes register 1 field
        rs1Sub[1] = inputArraySub[15];
        rs1Sub[2] = inputArraySub[14];
        rs1Sub[3] = inputArraySub[13];
        rs1Sub[4] = inputArraySub[12];

        rs2Sub[0] = inputArraySub[11];                    // Decodes register 2 field
        rs2Sub[1] = inputArraySub[10];
        rs2Sub[2] = inputArraySub[9];
        rs2Sub[3] = inputArraySub[8];
        rs2Sub[4] = inputArraySub[7];

        imm_StypeUpperSub[0] = inputArraySub[6];          // Decodes immediate value
        imm_StypeUpperSub[1] = inputArraySub[5];
        imm_StypeUpperSub[2] = inputArraySub[4];
        imm_StypeUpperSub[3] = inputArraySub[3];
        imm_StypeUpperSub[4] = inputArraySub[2];
        imm_StypeUpperSub[5] = inputArraySub[1];
        imm_StypeUpperSub[6] = inputArraySub[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimalSub += ((imm_StypeUpperSub[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                      // Calculates immediate value in decimal (locations are split)
            immToDecimalSub += ((imm_StypeLowerSub[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (bTypeSub == true) {
        imm_BtypeLowerSub[0] = inputArraySub[24];       // Decodes immediate value
        imm_BtypeLowerSub[1] = inputArraySub[23];
        imm_BtypeLowerSub[2] = inputArraySub[22];
        imm_BtypeLowerSub[3] = inputArraySub[21];
        imm_BtypeLowerSub[4] = inputArraySub[20];

        rs1Sub[0] = inputArraySub[16];                  // Decodes register 1 field
        rs1Sub[1] = inputArraySub[15];
        rs1Sub[2] = inputArraySub[14];
        rs1Sub[3] = inputArraySub[13];
        rs1Sub[4] = inputArraySub[12];

        rs2Sub[0] = inputArraySub[11];                  // Decodes register 2 field
        rs2Sub[1] = inputArraySub[10];
        rs2Sub[2] = inputArraySub[9];
        rs2Sub[3] = inputArraySub[8];
        rs2Sub[4] = inputArraySub[7];

        imm_BtypeUpperSub[0] = inputArraySub[6];        // Decodes immediate value
        imm_BtypeUpperSub[1] = inputArraySub[5];
        imm_BtypeUpperSub[2] = inputArraySub[4];
        imm_BtypeUpperSub[3] = inputArraySub[3];
        imm_BtypeUpperSub[4] = inputArraySub[2];
        imm_BtypeUpperSub[5] = inputArraySub[1];
        imm_BtypeUpperSub[6] = inputArraySub[0];

        int immIndex = 11;

        for (int i = 6; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimalSub += ((imm_BtypeUpperSub[i]) * pow(2, immIndex));
            immIndex--;
        }

        for (int i = 4; i > -1; i--) {                                          // Calculates immediate value in decimal (locations are split)
            immToDecimalSub += ((imm_BtypeLowerSub[i]) * pow(2, immIndex));
            immIndex--;
        }

    }
    else if (uTypeSub == true) {
        rdSub[0] = inputArraySub[24];                     // Decodes register destination
        rdSub[1] = inputArraySub[23];
        rdSub[2] = inputArraySub[22];
        rdSub[3] = inputArraySub[21];
        rdSub[4] = inputArraySub[20];

        imm_UtypeSub[0] = inputArraySub[19];              // Decodes immediate value
        imm_UtypeSub[1] = inputArraySub[18];
        imm_UtypeSub[2] = inputArraySub[17];
        imm_UtypeSub[3] = inputArraySub[16];
        imm_UtypeSub[4] = inputArraySub[15];
        imm_UtypeSub[5] = inputArraySub[14];
        imm_UtypeSub[6] = inputArraySub[13];
        imm_UtypeSub[7] = inputArraySub[12];
        imm_UtypeSub[8] = inputArraySub[11];
        imm_UtypeSub[9] = inputArraySub[10];
        imm_UtypeSub[10] = inputArraySub[9];
        imm_UtypeSub[11] = inputArraySub[8];
        imm_UtypeSub[12] = inputArraySub[7];
        imm_UtypeSub[13] = inputArraySub[6];
        imm_UtypeSub[14] = inputArraySub[5];
        imm_UtypeSub[15] = inputArraySub[4];
        imm_UtypeSub[16] = inputArraySub[3];
        imm_UtypeSub[17] = inputArraySub[2];
        imm_UtypeSub[18] = inputArraySub[1];
        imm_UtypeSub[19] = inputArraySub[0];

        for (int i = 11; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalSub += ((imm_UtypeSub[i]) * pow(2, i));
        }

    }
    else if (jTypeSub == true) {
        rdSub[0] = inputArraySub[24];                 // Decodes register destination
        rdSub[1] = inputArraySub[23];
        rdSub[2] = inputArraySub[22];
        rdSub[3] = inputArraySub[21];
        rdSub[4] = inputArraySub[20];

        imm_JtypeSub[0] = inputArraySub[19];          // Decodes immediate value
        imm_JtypeSub[1] = inputArraySub[18];
        imm_JtypeSub[2] = inputArraySub[17];
        imm_JtypeSub[3] = inputArraySub[16];
        imm_JtypeSub[4] = inputArraySub[15];
        imm_JtypeSub[5] = inputArraySub[14];
        imm_JtypeSub[6] = inputArraySub[13];
        imm_JtypeSub[7] = inputArraySub[12];
        imm_JtypeSub[8] = inputArraySub[11];
        imm_JtypeSub[9] = inputArraySub[10];
        imm_JtypeSub[10] = inputArraySub[9];
        imm_JtypeSub[11] = inputArraySub[8];
        imm_JtypeSub[12] = inputArraySub[7];
        imm_JtypeSub[13] = inputArraySub[6];
        imm_JtypeSub[14] = inputArraySub[5];
        imm_JtypeSub[15] = inputArraySub[4];
        imm_JtypeSub[16] = inputArraySub[3];
        imm_JtypeSub[17] = inputArraySub[2];
        imm_JtypeSub[18] = inputArraySub[1];
        imm_JtypeSub[19] = inputArraySub[0];

        for (int i = 19; i > -1; i--) {                         // Calculates immediate value in decimal
            immToDecimalSub += ((imm_JtypeSub[i]) * pow(2, i));
        }

    }

    for (int i = 4; i > -1; i--) {
        rdToDecimalSub += ((rdSub[i]) * pow(2, i));       // Decodes register destination (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs1ToDecimalSub += ((rs1Sub[i]) * pow(2, i));     // Decodes register 1 location (coverts to decimal)
    }

    for (int i = 4; i > -1; i--) {
        rs2ToDecimalSub += ((rs2Sub[i]) * pow(2, i));     // Decodes register 2 location (coverts to decimal)
    }

}

// Function that determines which instruction is to be performed from machine code (modified for RV32I and RV32F Base Instruction Sets) (CPU 0)
void determineOperationAdd() {

    int opcode[7] = {};
    int func3[3] = {};
    int func7[7] = {};
    int func3ToDecimal = 0;
    int func7ToDecimal = 0;
    int opcodeToDecimal = 0;
    bool validInstruction = true;

    opcode[0] = inputArrayAdd[31];             // Decodes opcode from input array
    opcode[1] = inputArrayAdd[30];
    opcode[2] = inputArrayAdd[29];
    opcode[3] = inputArrayAdd[28];
    opcode[4] = inputArrayAdd[27];
    opcode[5] = inputArrayAdd[26];
    opcode[6] = inputArrayAdd[25];

    func3[0] = inputArrayAdd[19];              // Decodes func3 from input array
    func3[1] = inputArrayAdd[18];
    func3[2] = inputArrayAdd[17];

    func7[0] = inputArrayAdd[6];               // Decodes func7 from input array
    func7[1] = inputArrayAdd[5];
    func7[2] = inputArrayAdd[4];
    func7[3] = inputArrayAdd[3];
    func7[4] = inputArrayAdd[2];
    func7[5] = inputArrayAdd[1];
    func7[6] = inputArrayAdd[0];

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
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lb ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 1:             // Lh instruction
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lh ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 2:             // Lw instruction 
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lw ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;


        case 4:             // Lbu instruction
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lbu ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 5:             // Lhu instruction
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Lhu ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 35:                                                        // Store instructions     
        switch (func3ToDecimal) {

        case 0:             // Sb instruction 
            sTypeAdd = true;
            opcodeTypeAdd = "S";
            instAdd = "Sb ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 1;
            writeBackSelAdd = 2;
            memWriteAdd = 1;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 1:             // Sh instruction
            sTypeAdd = true;
            opcodeTypeAdd = "S";
            instAdd = "Sh ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 1;
            writeBackSelAdd = 2;
            memWriteAdd = 1;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 2:             // Sw instruction 
            sTypeAdd = true;
            opcodeTypeAdd = "S";
            instAdd = "Sw ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 1;
            writeBackSelAdd = 2;
            memWriteAdd = 1;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 7:                                                         // Float load instructions    
        switch (func3ToDecimal) {

        case 2:             // Flw instruction 
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Flw ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 39:                                                        // Float store instructions   
        switch (func3ToDecimal) {

        case 2:             // Fsw instruction 
            sTypeAdd = true;
            opcodeTypeAdd = "S";
            instAdd = "Fsw ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 1;
            writeBackSelAdd = 2;
            memWriteAdd = 1;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 1;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
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

            case 120:                       // FMV.W.X instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "FMV.W.X ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 1;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            case 0:                       // FADD.S instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "FADD.S ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 1;
                regWriteAdd = 1;
                signExtAdd = 1;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            case 4:                       // FSUB.S instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "FSUB.S ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 1;
                regWriteAdd = 1;
                signExtAdd = 1;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
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
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Add ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            case 32:                    // Sub instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Sub ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 1:
            switch (func7ToDecimal) {

            case 0:                     // Sll instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Sll ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 2:
            switch (func7ToDecimal) {

            case 0:                     // Slt instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Slt ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 3:
            switch (func7ToDecimal) {

            case 0:                     // Sltu instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Sltu ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 4:
            switch (func7ToDecimal) {

            case 0:                     // Xor instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Xor ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 5:
            switch (func7ToDecimal) {

            case 0:                     // Srl instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Srl ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            case 32:                    // Sra instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Sra ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 6:
            switch (func7ToDecimal) {

            case 0:                     // Or instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Or ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 7:
            switch (func7ToDecimal) {

            case 0:                     // And instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "And ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 0;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
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
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Addi ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 1:
            switch (func7ToDecimal) {

            case 0:                     // Slli instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Slli ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 1;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 2:                             // Slti instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Slti ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 3:                             // Sltiu instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Sltiu ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 4:                             // Xori instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Xori ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 5:
            switch (func7ToDecimal) {

            case 0:                         // Srli instruction 
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Srli ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 1;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            case 32:                        // Srai instruction
                rTypeAdd = true;
                opcodeTypeAdd = "R";
                instAdd = "Srai ";
                aluOpAdd = 1;
                branchAdd = 0;
                memReadAdd = 0;
                writeBackSelAdd = 0;
                memWriteAdd = 0;
                aluSrcAdd = 1;
                regWriteAdd = 1;
                signExtAdd = 0;
                pcSelAdd = 0;
                branchConditionAdd = 0;
                branchSelAdd = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 6:                             // Ori instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Ori ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        case 7:                             // Andi instruction
            iTypeAdd = true;
            iTypeCateAdd = 1;
            opcodeTypeAdd = "I";
            instAdd = "Andi ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 0;
            branchConditionAdd = 0;
            branchSelAdd = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 55:                                                    // Lui instruction  
        uTypeAdd = true;
        opcodeTypeAdd = "U";
        instAdd = "Lui ";
        aluOpAdd = 1;
        branchAdd = 0;
        memReadAdd = 0;
        writeBackSelAdd = 0;
        memWriteAdd = 0;
        aluSrcAdd = 1;
        regWriteAdd = 1;
        signExtAdd = 1;
        pcSelAdd = 0;
        branchConditionAdd = 0;
        branchSelAdd = 0;
        break;

    case 23:                                                    // Auipc instruction
        uTypeAdd = true;
        opcodeTypeAdd = "U";
        instAdd = "Auipc ";
        aluOpAdd = 1;
        branchAdd = 0;
        memReadAdd = 0;
        writeBackSelAdd = 1;
        memWriteAdd = 0;
        aluSrcAdd = 1;
        regWriteAdd = 1;
        signExtAdd = 0;
        pcSelAdd = 1;
        branchConditionAdd = 0;
        branchSelAdd = 0;
        break;

        /* Start of Control Opcode Section */

    case 99:                                                    // branchAdd instructions    
        switch (func3ToDecimal) {
        case 0:                            // Beq instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Beq ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 1:                            // Bne instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Bne ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 4:                            // Blt instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Blt ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 5:                            // Bge instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Bge ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 6:                            // Bltu instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Bltu ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        case 7:                            // Bgeu instruction
            bTypeAdd = true;
            opcodeTypeAdd = "B";
            instAdd = "Bgeu ";
            aluOpAdd = 1;
            branchAdd = 1;
            memReadAdd = 0;
            writeBackSelAdd = 0;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 0;
            signExtAdd = 0;
            pcSelAdd = 2;
            branchConditionAdd = 1;
            branchSelAdd = 1;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }
        break;

    case 111:                                                   // Jal instruction 
        jTypeAdd = true;
        opcodeTypeAdd = "J";
        instAdd = "Jal ";
        aluOpAdd = 1;
        branchAdd = 0;
        memReadAdd = 0;
        writeBackSelAdd = 1;
        memWriteAdd = 0;
        aluSrcAdd = 1;
        regWriteAdd = 1;
        signExtAdd = 0;
        pcSelAdd = 2;
        branchConditionAdd = 0;
        branchSelAdd = 0;
        break;

    case 103:
        switch (func3ToDecimal) {
        case 0:                                             // Jalr instruction
            iTypeAdd = true;
            iTypeCateAdd = 2;
            opcodeTypeAdd = "I";
            instAdd = "Jalr ";
            aluOpAdd = 1;
            branchAdd = 0;
            memReadAdd = 0;
            writeBackSelAdd = 1;
            memWriteAdd = 0;
            aluSrcAdd = 1;
            regWriteAdd = 1;
            signExtAdd = 0;
            pcSelAdd = 1;
            branchConditionAdd = 0;
            branchSelAdd = 0;
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

        assignRegisterFieldsAdd();     // Call function to decode the remaining parts of machine code input
        printEventDecodeAdd();               // Prints the output of instruction and corresponding control signals

    }

    rTypeAdd = false;              // Resets all instruction types 
    iTypeAdd = false;
    sTypeAdd = false;
    bTypeAdd = false;
    uTypeAdd = false;
    jTypeAdd = false;

    iTypeCateAdd = 0;              // Resets all values 
    rdToDecimalAdd = 0;
    rs1ToDecimalAdd = 0;
    rs2ToDecimalAdd = 0;
    immToDecimalAdd = 0;

    validInstruction = true;
}

// Function that determines which instruction is to be performed from machine code (modified for RV32I and RV32F Base Instruction Sets) (CPU 1)
void determineOperationSub() {

    int opcode[7] = {};
    int func3[3] = {};
    int func7[7] = {};
    int func3ToDecimal = 0;
    int func7ToDecimal = 0;
    int opcodeToDecimal = 0;
    bool validInstruction = true;

    opcode[0] = inputArraySub[31];             // Decodes opcode from input array
    opcode[1] = inputArraySub[30];
    opcode[2] = inputArraySub[29];
    opcode[3] = inputArraySub[28];
    opcode[4] = inputArraySub[27];
    opcode[5] = inputArraySub[26];
    opcode[6] = inputArraySub[25];

    func3[0] = inputArraySub[19];              // Decodes func3 from input array
    func3[1] = inputArraySub[18];
    func3[2] = inputArraySub[17];

    func7[0] = inputArraySub[6];               // Decodes func7 from input array
    func7[1] = inputArraySub[5];
    func7[2] = inputArraySub[4];
    func7[3] = inputArraySub[3];
    func7[4] = inputArraySub[2];
    func7[5] = inputArraySub[1];
    func7[6] = inputArraySub[0];

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
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lb ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 1:             // Lh instruction
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lh ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 2:             // Lw instruction 
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lw ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;


        case 4:             // Lbu instruction
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lbu ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 5:             // Lhu instruction
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Lhu ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 35:                                                        // Store instructions     
        switch (func3ToDecimal) {

        case 0:             // Sb instruction 
            sTypeSub = true;
            opcodeTypeAdd = "S";
            instSub = "Sb ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 1;
            writeBackSelSub = 2;
            memWriteSub = 1;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 1:             // Sh instruction
            sTypeSub = true;
            opcodeTypeAdd = "S";
            instSub = "Sh ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 1;
            writeBackSelSub = 2;
            memWriteSub = 1;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 2:             // Sw instruction 
            sTypeSub = true;
            opcodeTypeAdd = "S";
            instSub = "Sw ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 1;
            writeBackSelSub = 2;
            memWriteSub = 1;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 7:                                                         // Float load instructions    
        switch (func3ToDecimal) {

        case 2:             // Flw instruction 
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Flw ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 39:                                                        // Float store instructions   
        switch (func3ToDecimal) {

        case 2:             // Fsw instruction 
            sTypeSub = true;
            opcodeTypeAdd = "S";
            instSub = "Fsw ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 1;
            writeBackSelSub = 2;
            memWriteSub = 1;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 1;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
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

            case 120:                       // FMV.W.X instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "FMV.W.X ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 1;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 0:                       // FADD.S instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "FADD.S ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 1;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 4:                       // FSUB.S instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "FSUB.S ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 1;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
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
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Add ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 32:                    // Sub instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Sub ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 1:
            switch (func7ToDecimal) {

            case 0:                     // Sll instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Sll ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 2:
            switch (func7ToDecimal) {

            case 0:                     // Slt instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Slt ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 3:
            switch (func7ToDecimal) {

            case 0:                     // Sltu instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Sltu ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 4:
            switch (func7ToDecimal) {

            case 0:                     // Xor instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Xor ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 5:
            switch (func7ToDecimal) {

            case 0:                     // Srl instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Srl ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 32:                    // Sra instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Sra ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 6:
            switch (func7ToDecimal) {

            case 0:                     // Or instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Or ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 7:
            switch (func7ToDecimal) {

            case 0:                     // And instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "And ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 0;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
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
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Addi ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 1:
            switch (func7ToDecimal) {

            case 0:                     // Slli instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Slli ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 2:                             // Slti instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Slti ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 3:                             // Sltiu instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Sltiu ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 4:                             // Xori instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Xori ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 5:
            switch (func7ToDecimal) {

            case 0:                         // Srli instruction 
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Srli ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            case 32:                        // Srai instruction
                rTypeSub = true;
                opcodeTypeAdd = "R";
                instSub = "Srai ";
                aluOpSub = 1;
                branchSub = 0;
                memReadSub = 0;
                writeBackSelSub = 0;
                memWriteSub = 0;
                aluSrcSub = 1;
                regWriteSub = 1;
                signExtSub = 0;
                pcSelSub = 0;
                branchConditionSub = 0;
                branchSelSub = 0;
                break;

            default:
                cout << "Error has occured in func7" << endl;
                break;
            }

            break;

        case 6:                             // Ori instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Ori ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        case 7:                             // Andi instruction
            iTypeSub = true;
            iTypeCateSub = 1;
            opcodeTypeAdd = "I";
            instSub = "Andi ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 0;
            branchConditionSub = 0;
            branchSelSub = 0;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }

        break;

    case 55:                                                    // Lui instruction  
        uTypeSub = true;
        opcodeTypeAdd = "U";
        instSub = "Lui ";
        aluOpSub = 1;
        branchSub = 0;
        memReadSub = 0;
        writeBackSelSub = 0;
        memWriteSub = 0;
        aluSrcSub = 1;
        regWriteSub = 1;
        signExtSub = 1;
        pcSelSub = 0;
        branchConditionSub = 0;
        branchSelSub = 0;
        break;

    case 23:                                                    // Auipc instruction
        uTypeSub = true;
        opcodeTypeAdd = "U";
        instSub = "Auipc ";
        aluOpSub = 1;
        branchSub = 0;
        memReadSub = 0;
        writeBackSelSub = 1;
        memWriteSub = 0;
        aluSrcSub = 1;
        regWriteSub = 1;
        signExtSub = 0;
        pcSelSub = 1;
        branchConditionSub = 0;
        branchSelSub = 0;
        break;

        /* Start of Control Opcode Section */

    case 99:                                                    // branchSub instructions    
        switch (func3ToDecimal) {
        case 0:                            // Beq instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Beq ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 1:                            // Bne instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Bne ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 4:                            // Blt instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Blt ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 5:                            // Bge instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Bge ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 6:                            // Bltu instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Bltu ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        case 7:                            // Bgeu instruction
            bTypeSub = true;
            opcodeTypeAdd = "B";
            instSub = "Bgeu ";
            aluOpSub = 1;
            branchSub = 1;
            memReadSub = 0;
            writeBackSelSub = 0;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 0;
            signExtSub = 0;
            pcSelSub = 2;
            branchConditionSub = 1;
            branchSelSub = 1;
            break;

        default:
            cout << "Error has occured in func3" << endl;
            break;
        }
        break;

    case 111:                                                   // Jal instruction 
        jTypeSub = true;
        opcodeTypeAdd = "J";
        instSub = "Jal ";
        aluOpSub = 1;
        branchSub = 0;
        memReadSub = 0;
        writeBackSelSub = 1;
        memWriteSub = 0;
        aluSrcSub = 1;
        regWriteSub = 1;
        signExtSub = 0;
        pcSelSub = 2;
        branchConditionSub = 0;
        branchSelSub = 0;
        break;

    case 103:
        switch (func3ToDecimal) {
        case 0:                                             // Jalr instruction
            iTypeSub = true;
            iTypeCateSub = 2;
            opcodeTypeAdd = "I";
            instSub = "Jalr ";
            aluOpSub = 1;
            branchSub = 0;
            memReadSub = 0;
            writeBackSelSub = 1;
            memWriteSub = 0;
            aluSrcSub = 1;
            regWriteSub = 1;
            signExtSub = 0;
            pcSelSub = 1;
            branchConditionSub = 0;
            branchSelSub = 0;
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

        assignRegisterFieldsSub();     // Call function to decode the remaining parts of machine code input
        printEventDecodeSub();               // Prints the output of instruction and corresponding control signals

    }

    rTypeSub = false;              // Resets all instruction types 
    iTypeSub = false;
    sTypeSub = false;
    bTypeSub = false;
    uTypeSub = false;
    jTypeSub = false;

    iTypeCateSub = 0;              // Resets all values 
    rdToDecimalSub = 0;
    rs1ToDecimalSub = 0;
    rs2ToDecimalSub = 0;
    immToDecimalSub = 0;

    validInstruction = true;
}

// Function that creates each instruction and writes in into an instruction file to be decoded (CPU 0)
void create32bitInstructionAdd(int instructionInLoop) {
    int addOrSub = 0;

    switch (instructionInLoop) {
    case 0:

        if (fmod(A.mainArray[arrayIndexAdd], 1) == 0) {          // Load integer instruction Array A

            decToBinaryNormal(offsetAdd, addOrSub);                                    // Outputs the immediate value offset
            myFileAdd << "00001" << "000" << "01010" << "0000011\n";

            floatPresent = 0;
        }
        else {                                  // Load float instruction Array A

            decToBinaryNormal(offsetAdd, addOrSub);                                    // Outputs the immediate value offset
            myFileAdd << "00001" << "010" << "01010" << "0000111\n";

            floatPresent = 1;
        }

        break;

    case 1:

        if (fmod(B.mainArray[arrayIndexAdd], 1) == 0) {          // Load integer instruction Array B

            decToBinaryNormal(offsetAdd, addOrSub);                                    // Outputs the immediate value offset
            myFileAdd << "00010" << "000" << "01011" << "0000011\n";
            floatPresent = 0;
        }
        else {                                  // Load float instruction Array B

            decToBinaryNormal(offsetAdd, addOrSub);                                    // Outputs the immediate value offset
            myFileAdd << "00010" << "010" << "01011" << "0000111\n";
            floatPresent = 1;
        }


        break;

    case 2:

        if (floatPresent == 0) {                // Add instruction 

            myFileAdd << "0000000" << "01011" << "01010" << "000" << "01101" << "0110011\n";

        }
        else {                                   // Float Add instruction

            myFileAdd << "0000000" << "01011" << "01010" << "000" << "01101" << "1010011\n";

        }

        break;

    case 3:

        if (floatPresent == 0) {                // Store instruction 

            decToBinarySplit(offsetAdd, floatPresent, addOrSub);
            myFileAdd << "0100011\n";
        }
        else {                                   // Float Store instruction

            decToBinarySplit(offsetAdd, floatPresent, addOrSub);
            myFileAdd << "0100111\n";
        }

        break;

    case 4:

        myFileAdd << "000000000100" << "00001" << "000" << "00001" << "0010011\n";         // Addi instruction Array A
        break;

    case 5:

        myFileAdd << "000000000100" << "00010" << "000" << "00010" << "0010011\n";         // Addi instruction Array B
        break;

    case 6:

        myFileAdd << "000000000100" << "00011" << "000" << "00011" << "0010011\n";         // Addi instruction Array C
        break;

    case 7:

        floatPresent = 2;
        decToBinarySplit(offsetAdd, floatPresent, addOrSub);                     // BNE instruction 
        myFileAdd << "1100011\n";

        offsetAdd += 4;
        arrayIndexAdd += 1;
        runOnceAdd = 0;
        runTwiceAdd = 0;
        break;

    default:
        break;

    }


}

// Function that creates each instruction and writes in into an instruction file to be decoded (CPU 1)
void create32bitInstructionSub(int instructionInLoop) {

    int addOrSub = 1;

    switch (instructionInLoop) {
    case 0:

        if (fmod(A.mainArray[arrayIndexSub], 1) == 0) {          // Load integer instruction Array A

            decToBinaryNormal(offsetSub, addOrSub);                                    // Outputs the immediate value offset
            myFileSub << "00001" << "000" << "01111" << "0000011\n";

            floatPresent = 0;
        }
        else {                                  // Load float instruction Array A

            decToBinaryNormal(offsetSub, addOrSub);                                    // Outputs the immediate value offset
            myFileSub << "00001" << "010" << "01111" << "0000111\n";

            floatPresent = 1;
        }

        break;

    case 1:

        if (fmod(B.mainArray[arrayIndexSub], 1) == 0) {          // Load integer instruction Array B

            decToBinaryNormal(offsetSub, addOrSub);                                    // Outputs the immediate value offset
            myFileSub << "00010" << "000" << "10000" << "0000011\n";
            floatPresent = 0;
        }
        else {                                  // Load float instruction Array B

            decToBinaryNormal(offsetSub, addOrSub);                                    // Outputs the immediate value offset
            myFileSub << "00010" << "010" << "10000" << "0000111\n";
            floatPresent = 1;
        }


        break;

    case 2:

        if (floatPresent == 0) {                // Sub instruction 

            myFileSub << "0100000" << "10000" << "01111" << "000" << "01110" << "0110011\n";

        }
        else {                                   // Float Sub instruction

            myFileSub << "0000100" << "10000" << "01111" << "000" << "01110" << "1010011\n";

        }

        break;

    case 3:

        if (floatPresent == 0) {                // Store instruction 

            decToBinarySplit(offsetSub, floatPresent, addOrSub);
            myFileSub << "0100011\n";
        }
        else {                                   // Float Store instruction

            decToBinarySplit(offsetSub, floatPresent, addOrSub);
            myFileSub << "0100111\n";
        }

        break;

    case 4:

        myFileSub << "000000000100" << "00001" << "000" << "00001" << "0010011\n";         // Addi instruction Array A
        break;

    case 5:

        myFileSub << "000000000100" << "00010" << "000" << "00010" << "0010011\n";         // Addi instruction Array B
        break;

    case 6:

        myFileSub << "000000000100" << "00011" << "000" << "00011" << "0010011\n";         // Addi instruction Array C
        break;

    case 7:

        floatPresent = 2;
        decToBinarySplit(offsetSub, floatPresent, addOrSub);                     // BNE instruction 
        myFileSub << "1100011\n";

        offsetSub += 4;
        arrayIndexSub += 1;
        runOnceSub = 0;
        runTwiceSub = 0;
        break;

    default:
        break;

    }


}

// Function that convert decimal to binary for normal immediate values in instruction
void decToBinaryNormal(int n, int addOrSub)
{
    if (addOrSub == 0) {        // Add case

        // Size of an integer is assumed to be 32 bits
        for (int i = 11; i >= 0; i--) {
            int k = n >> i;
            if (k & 1)
                myFileAdd << "1";
            else
                myFileAdd << "0";
        }
    }
    else {          // Sub case

        // Size of an integer is assumed to be 32 bits
        for (int i = 11; i >= 0; i--) {
            int k = n >> i;
            if (k & 1)
                myFileSub << "1";
            else
                myFileSub << "0";
        }
    }

}

// Function that converts decimal to binary for split immediate values in instruction
void decToBinarySplit(int n, int floatOrInt, int addOrSub) {

    if (addOrSub == 0) {            // Add case                   

        // Size of an integer is assumed to be 32 bits
        for (int i = 11; i >= 5; i--) {
            int k = n >> i;
            if (k & 1)
                myFileAdd << "1";
            else
                myFileAdd << "0";
        }

        if (floatOrInt == 0) {      // Integer instruction 
            myFileAdd << "01101" << "00011" << "000";
        }
        else if (floatOrInt == 1) {                      // Float instruction 
            myFileAdd << "01101" << "00011" << "010";
        }
        else {
            myFileAdd << "01000" << "00111" << "001";
        }

        for (int i = 4; i >= 0; i--) {
            int k = n >> i;
            if (k & 1)
                myFileAdd << "1";
            else
                myFileAdd << "0";
        }

    }
    else {      // Sub case

        // Size of an integer is assumed to be 32 bits
        for (int i = 11; i >= 5; i--) {
            int k = n >> i;
            if (k & 1)
                myFileSub << "1";
            else
                myFileSub << "0";
        }

        if (floatOrInt == 0) {      // Integer instruction 
            myFileSub << "01110" << "00100" << "000";
        }
        else if (floatOrInt == 1) {                      // Float instruction 
            myFileSub << "01110" << "00100" << "010";
        }
        else {
            myFileSub << "01000" << "00111" << "001";
        }

        for (int i = 4; i >= 0; i--) {
            int k = n >> i;
            if (k & 1)
                myFileSub << "1";
            else
                myFileSub << "0";
        }
    }



}

// Function that simulates the fetch stage of the pipeline
void fetchStageAdd(int loadPrevStage) {

    if (loadPrevStage == 3 || loadNewStageAdd == 3 || loadNewStage == 3) {

        if (loadNewStageAdd == 3) {

            cout << endl << "--------------------------------------------------------------" << endl;           // Reprint old data (don't update)
            cout << endl << "CPU 0 Fetch Stage:" << endl;
            cout << "Instruction: " << lineFetchAdd << endl;
            cout << "Clock Cycle: " << clockcycleAdd << endl;

        }
        else if (loadNewStageAdd == 1) {
            myFileAdd.open("ECGR 5181 Assignment 4 Instruction List Add.txt", ios::out, ios::trunc);       // Opens file for instructions in write mode


            if (!myFileAdd.is_open()) {

                cout << "Error opening file" << endl;

            }
            else {



                create32bitInstructionAdd(count32InstructionAdd);       // Creates the instructions and puts them in file
                count32InstructionAdd++;

                if (count32InstructionAdd > 7) {           // Resets loop of instructions
                    count32InstructionAdd = 0;
                }

                myFileAdd.close();


                myFileAdd.open("ECGR 5181 Assignment 4 Instruction List Add.txt", ios::in);

                getline(myFileAdd, lineFetchAdd);

                cout << endl << "--------------------------------------------------------------" << endl;       // Output instruction fetched
                cout << endl << "CPU 0 Fetch Stage:" << endl;
                cout << "Instruction: " << lineFetchAdd << endl;
                cout << "Clock Cycle: " << clockcycleAdd << endl;

                lineDecodeAdd = lineFetchAdd;

            }


            myFileAdd.close();

            cacheIOperationCPU0();

            clockcycleAdd += 3;
        }

    }
    else if (loadPrevStage == 1) {                                                                   // In case of normal operation
        myFileAdd.open("ECGR 5181 Assignment 4 Instruction List Add.txt", ios::out, ios::trunc);       // Opens file for instructions in write mode


        if (!myFileAdd.is_open()) {

            cout << "Error opening file" << endl;

        }
        else {



            create32bitInstructionAdd(count32InstructionAdd);       // Creates the instructions and puts them in file
            count32InstructionAdd++;

            if (count32InstructionAdd > 7) {           // Resets loop of instructions
                count32InstructionAdd = 0;
            }

            myFileAdd.close();


            myFileAdd.open("ECGR 5181 Assignment 4 Instruction List Add.txt", ios::in);

            getline(myFileAdd, lineFetchAdd);

            cout << endl << "--------------------------------------------------------------" << endl;       // Output instruction fetched
            cout << endl << "CPU 0 Fetch Stage:" << endl;
            cout << "Instruction: " << lineFetchAdd << endl;
            cout << "Clock Cycle: " << clockcycleAdd << endl;

            lineDecodeAdd = lineFetchAdd;

        }


        myFileAdd.close();

        cacheIOperationCPU0();

        //clockcycleAdd += 3;         // Only use in case of 2 processer and cache
    }
    else if (loadPrevStage == 2) {                                                                          // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;           // Reprint old data (don't update)
        cout << endl << "CPU 0 Fetch Stage:" << endl;
        cout << "Instruction: " << lineFetchAdd << endl;
        cout << "Clock Cycle: " << clockcycleAdd << endl;

    }
    else {
        cout << endl << "CPU 0 Fetch Stage:" << endl;                                                             // In case of no operation
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycleAdd << endl;

    }
}

// Function that simulates the fetch stage of the pipeline
void fetchStageSub(int loadPrevStage) {

    if (loadPrevStage == 3 || loadNewStageSub == 3 || loadNewStage == 3) {

        if (loadNewStageSub == 3) {
            cout << endl << "--------------------------------------------------------------" << endl;           // Reprint old data (don't update)
            cout << endl << "CPU 1 Fetch Stage:" << endl;
            cout << "Instruction: " << lineFetchSub << endl;
            cout << "Clock Cycle: " << clockcycleSub << endl;
        }
        else if (loadNewStageSub == 1) {
            myFileSub.open("ECGR 5181 Assignment 4 Instruction List Sub.txt", ios::out, ios::trunc);       // Opens file for instructions in write mode


            if (!myFileSub.is_open()) {

                cout << "Error opening file" << endl;

            }
            else {

                create32bitInstructionSub(count32InstructionSub);       // Creates the instructions and puts them in file
                count32InstructionSub++;

                if (count32InstructionSub > 7) {           // Resets loop of instructions
                    count32InstructionSub = 0;
                }

                myFileSub.close();

                myFileSub.open("ECGR 5181 Assignment 4 Instruction List Sub.txt", ios::in);

                getline(myFileSub, lineFetchSub);

                cout << endl << "--------------------------------------------------------------" << endl;       // Output instruction fetched
                cout << endl << "CPU 1 Fetch Stage:" << endl;
                cout << "Instruction: " << lineFetchSub << endl;
                cout << "Clock Cycle: " << clockcycleSub << endl;

            }

            lineDecodeSub = lineFetchSub;

            myFileSub.close();

            cacheIOperationCPU1();

            //clockcycleSub += 3;
        }

    }
    else if (loadPrevStage == 1) {                                                                   // In case of normal operation
        myFileSub.open("ECGR 5181 Assignment 4 Instruction List Sub.txt", ios::out, ios::trunc);       // Opens file for instructions in write mode


        if (!myFileSub.is_open()) {

            cout << "Error opening file" << endl;

        }
        else {

            create32bitInstructionSub(count32InstructionSub);       // Creates the instructions and puts them in file
            count32InstructionSub++;

            if (count32InstructionSub > 7) {           // Resets loop of instructions
                count32InstructionSub = 0;
            }

            myFileSub.close();

            myFileSub.open("ECGR 5181 Assignment 4 Instruction List Sub.txt", ios::in);

            getline(myFileSub, lineFetchSub);

            cout << endl << "--------------------------------------------------------------" << endl;       // Output instruction fetched
            cout << endl << "CPU 1 Fetch Stage:" << endl;
            cout << "Instruction: " << lineFetchSub << endl;
            cout << "Clock Cycle: " << clockcycleSub << endl;

        }

        lineDecodeSub = lineFetchSub;

        myFileSub.close();

        cacheIOperationCPU1();

        clockcycleSub += 3;         // Only use in case of 2 processer and cache
    }
    else if (loadPrevStage == 2) {                                                                          // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;           // Reprint old data (don't update)
        cout << endl << "CPU 1 Fetch Stage:" << endl;
        cout << "Instruction: " << lineFetchSub << endl;
        cout << "Clock Cycle: " << clockcycleSub << endl;

    }
    else {

        cout << endl << "CPU 1 Fetch Stage:" << endl;                                                             // In case of no operation
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycleSub << endl;
    }
}

// Function that simulates the decode stage of the pipeline
void decodeStageAdd(int loadPrevStage) {

    if (loadPrevStage == 3 || loadNewStageAdd == 3 || loadNewStage == 3) {

        if (loadNewStageAdd == 3) {

            cout << endl << "--------------------------------------------------------------" << endl;       // Reprint old data (don't update)
            cout << endl << "CPU 0 Decode Stage:" << endl;
            cout << "Clock Cycle: " << clockcycleAdd << endl << endl;
            determineOperationAdd();           // Decodes and determines the instruction

        }
        else if (loadNewStageAdd == 1) {

            myFileAdd.open("ECGR 5181 Assignment 4 Instruction List Add.txt", ios::in);       // Opens file for instructions in read mode

            while (getline(myFileAdd, lineDecodeAdd)) {     // CPU 0

                for (int i = 0; i < 32; i++) {
                    tempArray[i] = (int)lineDecodeAdd[i];

                    if (tempArray[i] == 48) {               // If '0' in ACSII set 0 integer
                        inputArrayAdd[i] = 0;
                    }
                    else if (tempArray[i] == 49) {        // If '1' in ACSII set 1 integer
                        inputArrayAdd[i] = 1;
                    }
                }
                cout << endl << "--------------------------------------------------------------" << endl;
                cout << endl << "CPU 0 Decode Stage:" << endl;
                cout << "Clock Cycle: " << clockcycleAdd << endl << endl;
                determineOperationAdd();           // Decodes and determines the instruction
            }

            myFileAdd.close();

        }

    }
    else if (loadPrevStage == 1) {                                                       // In case of normal operation
        myFileAdd.open("ECGR 5181 Assignment 4 Instruction List Add.txt", ios::in);       // Opens file for instructions in read mode

        while (getline(myFileAdd, lineDecodeAdd)) {     // CPU 0

            for (int i = 0; i < 32; i++) {
                tempArray[i] = (int)lineDecodeAdd[i];

                if (tempArray[i] == 48) {               // If '0' in ACSII set 0 integer
                    inputArrayAdd[i] = 0;
                }
                else if (tempArray[i] == 49) {        // If '1' in ACSII set 1 integer
                    inputArrayAdd[i] = 1;
                }
            }
            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 0 Decode Stage:" << endl;
            cout << "Clock Cycle: " << clockcycleAdd << endl << endl;
            determineOperationAdd();           // Decodes and determines the instruction
        }

        myFileAdd.close();
    }
    else if (loadPrevStage == 2) {                                                                  // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;       // Reprint old data (don't update)
        cout << endl << "CPU 0 Decode Stage:" << endl;
        cout << "Clock Cycle: " << clockcycleAdd << endl << endl;
        determineOperationAdd();           // Decodes and determines the instruction

    }
    else {                                                                                          // In case of no operation
        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 0 Decode Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycleAdd << endl;

    }
}

// Function that simulates the decode stage of the pipeline
void decodeStageSub(int loadPrevStage) {

    if (loadPrevStage == 3 || loadNewStageSub == 3 || loadNewStage == 3) {

        if (loadNewStageSub == 3) {

            cout << endl << "--------------------------------------------------------------" << endl;       // Reprint old data (don't update)
            cout << endl << "CPU 1 Decode Stage:" << endl;
            cout << "Clock Cycle: " << clockcycleSub << endl << endl;
            determineOperationSub();            // Decodes and determines the instruction

        }
        else if (loadNewStageSub == 1) {

            myFileSub.open("ECGR 5181 Assignment 4 Instruction List Sub.txt", ios::in);

            while (getline(myFileSub, lineDecodeSub)) {     // CPU 1

                for (int i = 0; i < 32; i++) {
                    tempArray[i] = (int)lineDecodeSub[i];

                    if (tempArray[i] == 48) {               // If '0' in ACSII set 0 integer
                        inputArraySub[i] = 0;
                    }
                    else if (tempArray[i] == 49) {        // If '1' in ACSII set 1 integer
                        inputArraySub[i] = 1;
                    }
                }
                cout << endl << "--------------------------------------------------------------" << endl;
                cout << endl << "CPU 1 Decode Stage:" << endl;
                cout << "Clock Cycle: " << clockcycleSub << endl << endl;
                determineOperationSub();           // Decodes and determines the instruction
            }

            myFileSub.close();

        }

    }
    else if (loadPrevStage == 1) {                                                       // In case of normal operation
        myFileSub.open("ECGR 5181 Assignment 4 Instruction List Sub.txt", ios::in);

        while (getline(myFileSub, lineDecodeSub)) {     // CPU 1

            for (int i = 0; i < 32; i++) {
                tempArray[i] = (int)lineDecodeSub[i];

                if (tempArray[i] == 48) {               // If '0' in ACSII set 0 integer
                    inputArraySub[i] = 0;
                }
                else if (tempArray[i] == 49) {        // If '1' in ACSII set 1 integer
                    inputArraySub[i] = 1;
                }
            }
            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 1 Decode Stage:" << endl;
            cout << "Clock Cycle: " << clockcycleSub << endl << endl;
            determineOperationSub();           // Decodes and determines the instruction
        }

        myFileSub.close();
    }
    else if (loadPrevStage == 2) {                                                                  // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;       // Reprint old data (don't update)
        cout << endl << "CPU 1 Decode Stage:" << endl;
        cout << "Clock Cycle: " << clockcycleSub << endl << endl;
        determineOperationSub();            // Decodes and determines the instruction

    }
    else {                                                                                          // In case of no operation

        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 1 Decode Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycleSub << endl;
    }
}

// Function that simulates the execute stage of the pipeline
void executeStageAdd(int loadPrevStage) {

    if (loadPrevStage == 3 || loadNewStageAdd == 3 || loadNewStage == 3) {


        if (loadNewStageAdd == 1) {

            if (instExecuteAdd == "") {
                cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
                cout << endl << "CPU 0 Execute Stage: " << endl;

                cout << "Instruction: No operation" << endl;
                cout << "Clock Cycle: " << clockcycleAdd << endl;
            }
            else {
                printEventExecuteAdd();
            }

            if (instExecuteAdd == "Lb " || instExecuteAdd == "Flw ") {                                        // Perform cache operation

                if (cacheDCount0 > 0) {
                    cacheDMode0 = 1;
                    cacheDCount0 = 0;
                }
                else {
                    cacheDMode0 = 0;
                    cacheDCount0++;
                }

                cacheDOperationCPU0();

            }

            if (instExecuteAdd == "Add " || instExecuteAdd == "FADD.S ") {                                        // Perform operation
                C.mainArray[arrayIndexAdd] = A.mainArray[arrayIndexAdd] + B.mainArray[arrayIndexAdd];
            }
        }
        else if ((count32InstructionAdd == 3 && floatPresent == 0) || (count32InstructionAdd == 4 && floatPresent == 0) || (count32InstructionAdd == 4 && floatPresent == 1) || (count32InstructionAdd == 5 && floatPresent == 1)) {

            //if (instExecuteAdd == "Lb " || instExecuteAdd == "Flw ") {                                        // Perform cache operation

            //    if (cacheDCount0 > 0) {
            //        cacheDMode = 1;
            //        cacheDCount0 = 0;
            //    }
            //    else {
            //        cacheDMode = 0;
            //    }

            //    cacheDOperationCPU0();
            //    cacheDCount0++;

            //}

            cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
            cout << endl << "CPU 0 Execute Stage: " << endl;

            cout << "Instruction: Stall" << endl;
            cout << "Clock Cycle: " << clockcycleAdd << endl;

            stallOccured0 = 1;
            instructionAddCount++;
        }
        else if (loadNewStageAdd == 3) {

            //if (instExecuteAdd == "Lb " || instExecuteAdd == "Flw ") {                                        // Perform cache operation

            //    if (cacheDCount0 > 0) {
            //        cacheDMode = 1;
            //        cacheDCount0 = 0;
            //    }
            //    else {
            //        cacheDMode = 0;
            //    }

            //    cacheDOperationCPU0();
            //    cacheDCount0++;

            //}

            cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
            cout << endl << "CPU 0 Execute Stage: " << endl;

            cout << "Pending Data Request Operation" << endl;
            cout << "Clock Cycle: " << clockcycleAdd << endl;

            stallOccured0 = 1;
            instructionAddCount++;
        }


    }
    else if (loadPrevStage == 1 || loadNewStageAdd == 1) {                                                                           // In case of normal operation

        if (count32InstructionAdd == 4 && floatPresent == 0) {

            cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
            cout << endl << "CPU 0 Execute Stage: " << endl;

            cout << "Instruction: Stall" << endl;
            cout << "Clock Cycle: " << clockcycleAdd << endl;

            loadNewStageAdd = 3;
        }
        else {
            printEventExecuteAdd();
        }




        if (instExecuteAdd == "Add " || instExecuteAdd == "FADD.S ") {                                        // Perform operation
            C.mainArray[arrayIndexAdd] = A.mainArray[arrayIndexAdd] + B.mainArray[arrayIndexAdd];
        }

    }
    else if (loadPrevStage == 2 || loadNewStageAdd == 2) {                                                                      // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
        cout << endl << "CPU 0 Execute Stage: " << endl;

        cout << "Instruction: Stall" << endl;
        cout << "Clock Cycle: " << clockcycleAdd << endl;



        instructionAddCount++;

    }
    else {
        cout << endl << "--------------------------------------------------------------" << endl;      // In case of no operation
        cout << endl << "CPU 0 Execute Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycleAdd << endl;
    }
}

// Function that simulates the execute stage of the pipeline
void executeStageSub(int loadPrevStage) {

    if (loadPrevStage == 3 || loadNewStageSub == 3 || loadNewStage == 3) {

        if (loadNewStageSub == 1) {



            if (instExecuteSub == "") {
                cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
                cout << endl << "CPU 0 Execute Stage: " << endl;

                cout << "Instruction: No operation" << endl;
                cout << "Clock Cycle: " << clockcycleSub << endl;
            }
            else {
                printEventExecuteSub();
            }

            if (instExecuteSub == "Lb " || instExecuteSub == "Flw ") {                                        // Perform cache operation

                if (cacheDCount1 > 0) {
                    cacheDMode1 = 1;
                    cacheDCount1 = 0;
                }
                else {
                    cacheDMode1 = 0;
                    cacheDCount1++;
                }

                cacheDOperationCPU1();

            }

            if (instExecuteSub == "Sub " || instExecuteSub == "FSUB.S ") {                                        // Perform operation
                D.mainArray[arrayIndexAdd] = A.mainArray[arrayIndexSub] - B.mainArray[arrayIndexSub];
            }
        }
        else if (((count32InstructionSub == 3 && floatPresent == 0) || (count32InstructionSub == 4 && floatPresent == 0) || (count32InstructionSub == 4 && floatPresent == 1) || (count32InstructionSub == 5 && floatPresent == 1))) {

            //if (instExecuteSub == "Lb " || instExecuteSub == "Flw ") {                                        // Perform cache operation

            //    if (cacheDCount1 > 0) {
            //        cacheDMode = 1;
            //        cacheDCount1 = 0;
            //    }
            //    else {
            //        cacheDMode = 0;
            //    }

            //    cacheDOperationCPU1();
            //    cacheDCount1++;

            //}

            cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
            cout << endl << "CPU 1 Execute Stage: " << endl;

            cout << "Instruction: Stall" << endl;
            cout << "Clock Cycle: " << clockcycleSub << endl;

            stallOccured1 = 1;
            instructionSubCount++;

        }
        else if (loadNewStageSub == 3) {

            //if (instExecuteSub == "Lb " || instExecuteSub == "Flw ") {                                        // Perform cache operation

            //    if (cacheDCount1 > 0) {
            //        cacheDMode = 1;
            //        cacheDCount1 = 0;
            //    }
            //    else {
            //        cacheDMode = 0;
            //    }

            //    cacheDOperationCPU1();
            //    cacheDCount1++;

            //}

            cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
            cout << endl << "CPU 1 Execute Stage: " << endl;

            cout << "Pending Data Request Operation" << endl;
            cout << "Clock Cycle: " << clockcycleSub << endl;

            stallOccured1 = 1;
            instructionSubCount++;

        }




    }
    else if (loadPrevStage == 1 || loadNewStageSub == 1) {                                                                           // In case of normal operation

        printEventExecuteSub();


        if (instExecuteSub == "Sub " || instExecuteSub == "FSUB.S ") {                                        // Perform operation
            D.mainArray[arrayIndexAdd] = A.mainArray[arrayIndexSub] - B.mainArray[arrayIndexSub];
        }

    }
    else if (loadPrevStage == 2 || loadNewStageSub == 2) {                                                                      // In case of stall


        cout << endl << "--------------------------------------------------------------" << endl;       // Output stall
        cout << endl << "CPU 1 Execute Stage: " << endl;

        cout << "Instruction: Stall" << endl;
        cout << "Clock Cycle: " << clockcycleSub << endl;




        instructionSubCount++;

    }
    else {

        cout << endl << "--------------------------------------------------------------" << endl;      // In case of no operation
        cout << endl << "CPU 1 Execute Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycleSub << endl;
    }
}

// Function that simulates the writeback stage of the pipeline
void writebackStageAdd(int loadPrevStage) {

    if (loadPrevStage == 3 || loadNewStageAdd == 3 || loadNewStage == 3) {

        if (loadNewStageAdd == 3) {

            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 0 Writeback Stage:" << endl;
            cout << "Instruction: No Operation" << endl;
            cout << "Clock Cycle: " << clockcycleAdd << endl;

        }
        else if (loadNewStageAdd == 1) {

            if (instExecuteAdd == "Addi " && count32InstructionAdd == 6) {                                                               // If store executed, output value and at its saved address


                cout << endl << "--------------------------------------------------------------" << endl;
                cout << endl << "CPU 0 Writeback Stage: " << endl;
                printf("Value: %f is saved at memory address 0x%p\n", C.mainArray[arrayIndexAdd], C.mainArrayAddress[arrayIndexAdd]);
                cout << "Clock Cycle: " << clockcycleAdd << endl;

            }
            else {
                cout << endl << "--------------------------------------------------------------" << endl;
                cout << endl << "CPU 0 Writeback Stage:" << endl;
                cout << "Instruction: No Operation" << endl;
                cout << "Clock Cycle: " << clockcycleAdd << endl;
            }

        }

    }
    else if (loadPrevStage == 1) {                                                                                               // In case of normal operation 

        if (instExecuteAdd == "Addi " && count32InstructionAdd == 6) {                                                               // If store executed, output value and at its saved address


            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 0 Writeback Stage: " << endl;
            printf("Value: %f is saved at memory address 0x%p\n", C.mainArray[arrayIndexAdd], C.mainArrayAddress[arrayIndexAdd]);
            cout << "Clock Cycle: " << clockcycleAdd << endl;

        }
        else {
            // Else no operation
            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 0 Writeback Stage:" << endl;
            cout << "Instruction: No Operation" << endl;
            cout << "Clock Cycle: " << clockcycleAdd << endl;
        }


    }
    else if (loadPrevStage == 2) {                                                                                              // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 0 Writeback Stage: " << endl;
        cout << "Clock Cycle: " << clockcycleAdd << endl;

    }
    else {                                                                                                                      // In case of no operation
        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 0 Writeback Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycleAdd << endl;
    }
}

// Function that simulates the writeback stage of the pipeline
void writebackStageSub(int loadPrevStage) {

    if (loadPrevStage == 3 || loadNewStageSub == 3 || loadNewStage == 3) {

        if (loadNewStageSub == 3) {

            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 1 Writeback Stage:" << endl;
            cout << "Instruction: No Operation" << endl;
            cout << "Clock Cycle: " << clockcycleSub << endl;

        }
        else if (loadNewStageSub == 1) {

            if (instExecuteSub == "Addi " && count32InstructionSub == 6) {                                                               // If store executed, output value and at its saved address

                cout << endl << "--------------------------------------------------------------" << endl;
                cout << endl << "CPU 1 Writeback Stage: " << endl;
                printf("Value: %f is saved at memory address 0x%p\n", D.mainArray[arrayIndexSub], D.mainArrayAddress[arrayIndexSub]);
                cout << "Clock Cycle: " << clockcycleSub << endl;

            }
            else {
                cout << endl << "--------------------------------------------------------------" << endl;
                cout << endl << "CPU 1 Writeback Stage:" << endl;
                cout << "Instruction: No Operation" << endl;
                cout << "Clock Cycle: " << clockcycleSub << endl;
            }

        }

    }
    else if (loadPrevStage == 1) {                                                                                               // In case of normal operation 


        if (instExecuteSub == "Addi " && count32InstructionSub == 6) {                                                               // If store executed, output value and at its saved address

            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 1 Writeback Stage: " << endl;
            printf("Value: %f is saved at memory address 0x%p\n", D.mainArray[arrayIndexSub], D.mainArrayAddress[arrayIndexSub]);
            cout << "Clock Cycle: " << clockcycleSub << endl;

        }
        else {                                                                                                                  // Else no operation
            cout << endl << "--------------------------------------------------------------" << endl;
            cout << endl << "CPU 1 Writeback Stage:" << endl;
            cout << "Instruction: No Operation" << endl;
            cout << "Clock Cycle: " << clockcycleSub << endl;
        }



    }
    else if (loadPrevStage == 2) {                                                                                              // In case of stall

        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 1 Writeback Stage: " << endl;
        cout << "Clock Cycle: " << clockcycleSub << endl;
    }
    else {                                                                                                                      // In case of no operation

        cout << endl << "--------------------------------------------------------------" << endl;
        cout << endl << "CPU 1 Writeback Stage:" << endl;
        cout << "Instruction: No Operation" << endl;
        cout << "Clock Cycle: " << clockcycleSub << endl;
    }
}

void memBus(int cpu0DataRequest, int cpu1DataRequest) {

    if (((cpu0DataRequest == 0 && cpu1DataRequest == 1) || (cpu0DataRequest == 0 && cpu1DataRequest == 0)) && timesFetchOccured >= 1) {
        loadNewStage = 3;
        loadNewStageAdd = 1;
        loadNewStageSub = 1;
    }
    else if (cpu0DataRequest == 1 && cpu1DataRequest == 0) {
        loadNewStage = 3;
        loadNewStageAdd = 1;
        loadNewStageSub = 1;
    }
    else if (cpu0DataRequest == 1 && cpu1DataRequest == 1) {         // Both CPUs request data

        if (cpuSelect == 0) {


            loadNewStage = 3;
            loadNewStageAdd = 1;
            loadNewStageSub = 3;



            switchToCPU ^= 1;
            cpuSelect ^= 1;
        }
        else if (cpuSelect == 1 && (instExecuteAdd != "Fsw " && instExecuteSub != "Fsw ")) {



            loadNewStage = 3;
            loadNewStageAdd = 3;
            loadNewStageSub = 1;

            switchToCPU = 0;
            cpuSelect ^= 1;
        }

    }


    if (loadNewStage == 0 && timesFetchOccured >= 1) {
        loadNewStage = 3;
        loadNewStageAdd = 1;
        loadNewStageSub = 1;
    }

    if (instExecuteAdd == "Add " && runOnceAdd == 0) {
        loadNewStageAdd = 3;
        runOnceAdd++;
    }

    if (instExecuteAdd == "FADD.S " && runOnceAdd == 0) {
        loadNewStageAdd = 3;
        runOnceAdd++;
    }

    if (instExecuteAdd == "Fsw " && runTwiceAdd <= 1) {
        loadNewStageAdd = 3;
        runTwiceAdd++;
    }

    if (instExecuteSub == "Sub " && runOnceSub == 0) {
        loadNewStageSub = 3;
        runOnceSub++;
    }

    if (instExecuteSub == "FSUB.S " && runTwiceSub <= 0) {
        loadNewStageSub = 3;
        runTwiceSub++;
    }

    if (instExecuteSub == "Fsw " && runTwiceSub <= 2) {
        loadNewStageSub = 3;
        runTwiceSub++;
    }

}

void cacheSetup() {

    cacheICPU0.capacitySize = 256;			// Cache I CPU 0 details
    cacheICPU0.cacheLineSize = 32;
    cacheICPU0.cacheAssociativity = 2;

    cacheICPU0.numberOfOffsetBits = log2(cacheICPU0.cacheLineSize);
    cacheICPU0.numberOfSets = cacheICPU0.capacitySize / cacheICPU0.cacheLineSize;
    cacheICPU0.numberOfIndexBitsPerSet = log2(cacheICPU0.numberOfSets);

    cacheICPU0.numberOfTagBits = cacheICPU0.numberOfBitsInAddress - cacheICPU0.numberOfOffsetBits - cacheICPU0.numberOfIndexBitsPerSet;
    cacheICPU0.numberOfTotalBlocks = cacheICPU0.numberOfSets * cacheICPU0.cacheAssociativity;



    cacheICPU1.capacitySize = 256;			// Cache I CPU 1 details
    cacheICPU1.cacheLineSize = 32;
    cacheICPU1.cacheAssociativity = 2;

    cacheICPU1.numberOfOffsetBits = log2(cacheICPU1.cacheLineSize);
    cacheICPU1.numberOfSets = cacheICPU1.capacitySize / cacheICPU1.cacheLineSize;
    cacheICPU1.numberOfIndexBitsPerSet = log2(cacheICPU1.numberOfSets);

    cacheICPU1.numberOfTagBits = cacheICPU1.numberOfBitsInAddress - cacheICPU1.numberOfOffsetBits - cacheICPU1.numberOfIndexBitsPerSet;
    cacheICPU1.numberOfTotalBlocks = cacheICPU1.numberOfSets * cacheICPU1.cacheAssociativity;



    cacheDCPU0.capacitySize = 256;			// Cache D CPU 0 details
    cacheDCPU0.cacheLineSize = 32;
    cacheDCPU0.cacheAssociativity = 2;

    cacheDCPU0.numberOfOffsetBits = log2(cacheDCPU0.cacheLineSize);
    cacheDCPU0.numberOfSets = cacheDCPU0.capacitySize / cacheDCPU0.cacheLineSize;
    cacheDCPU0.numberOfIndexBitsPerSet = log2(cacheDCPU0.numberOfSets);

    cacheDCPU0.numberOfTagBits = cacheDCPU0.numberOfBitsInAddress - cacheDCPU0.numberOfOffsetBits - cacheDCPU0.numberOfIndexBitsPerSet;
    cacheDCPU0.numberOfTotalBlocks = cacheDCPU0.numberOfSets * cacheDCPU0.cacheAssociativity;



    cacheDCPU1.capacitySize = 256;			// Cache D CPU 1 details
    cacheDCPU1.cacheLineSize = 32;
    cacheDCPU1.cacheAssociativity = 2;

    cacheDCPU1.numberOfOffsetBits = log2(cacheDCPU1.cacheLineSize);
    cacheDCPU1.numberOfSets = cacheDCPU1.capacitySize / cacheDCPU1.cacheLineSize;
    cacheDCPU1.numberOfIndexBitsPerSet = log2(cacheDCPU1.numberOfSets);

    cacheDCPU1.numberOfTagBits = cacheDCPU1.numberOfBitsInAddress - cacheDCPU1.numberOfOffsetBits - cacheDCPU1.numberOfIndexBitsPerSet;
    cacheDCPU1.numberOfTotalBlocks = cacheDCPU1.numberOfSets * cacheDCPU1.cacheAssociativity;

}

// LRU cache function
void readFromMemoryCPU0() {							// Reads memory address location

    decimalAddressValue = 0;
    int decimalPosition = 7;

    int bitPosition = 0;

    for (int j = 0; j < 1; j++) {

        for (int i = 0; i < 8; i++) {								// Determine tag bit value

            if (hexStringCache[i] == 'A' || hexStringCache[i] == 'a') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 10 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'B' || hexStringCache[i] == 'b') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 11 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'C' || hexStringCache[i] == 'c') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 12 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'D' || hexStringCache[i] == 'd') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 13 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'E' || hexStringCache[i] == 'e') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 14 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'F' || hexStringCache[i] == 'f') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 15 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '0') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 0 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '1') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 1 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '2') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 2 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '3') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 3 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '4') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 4 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '5') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 5 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '6') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 6 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '7') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 7 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '8') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 8 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '9') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 9 * pow(16, decimalPosition);
            }

            bitPosition += 4;
            decimalPosition--;

        }
        ;
        bitPosition = 0;

    }

}

// LRU cache function
void readFromMemoryCPU1() {							// Reads memory address location

    decimalAddressValue = 0;
    int decimalPosition = 7;


    int bitPosition = 0;

    for (int j = 0; j < 1; j++) {


        for (int i = 0; i < 8; i++) {								// Determine tag bit value

            if (hexStringCache[i] == 'A' || hexStringCache[i] == 'a') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 10 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'B' || hexStringCache[i] == 'b') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 11 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'C' || hexStringCache[i] == 'c') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 12 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'D' || hexStringCache[i] == 'd') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 13 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'E' || hexStringCache[i] == 'e') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 14 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == 'F' || hexStringCache[i] == 'f') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 15 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '0') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 0 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '1') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 1 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '2') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 2 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '3') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 3 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '4') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 4 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '5') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 5 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '6') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 6 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '7') {

                binaryArray[bitPosition] = 0;
                binaryArray[bitPosition + 1] = 1;
                binaryArray[bitPosition + 2] = 1;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 7 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '8') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 0;

                decimalAddressValue += 8 * pow(16, decimalPosition);
            }
            else if (hexStringCache[i] == '9') {

                binaryArray[bitPosition] = 1;
                binaryArray[bitPosition + 1] = 0;
                binaryArray[bitPosition + 2] = 0;
                binaryArray[bitPosition + 3] = 1;

                decimalAddressValue += 9 * pow(16, decimalPosition);
            }

            bitPosition += 4;
            decimalPosition--;

        }
        ;
        bitPosition = 0;

    }

}

// LRU cache function
void readBinaryValue(int numSets) {

    int bitPositionTag = 0;
    int bitPositionSet = 0;

    tagBitFromBinary = 0;
    indexSetBitFromBinary = 0;

    if (numSets == 8) {

        bitPositionTag = 23;
        bitPositionSet = 2;

        for (int i = 0; i < 24; i++) {		// Calculates tag bits

            if (binaryArray[i] == 0) {

                tagBitFromBinary += 0 * pow(2, bitPositionTag);

            }
            else if (binaryArray[i] == 1) {

                tagBitFromBinary += 1 * pow(2, bitPositionTag);

            }

            bitPositionTag--;

        }

        for (int i = 24; i < 27; i++) {

            if (binaryArray[i] == 0) {

                indexSetBitFromBinary += 0 * pow(2, bitPositionSet);

            }
            else if (binaryArray[i] == 1) {

                indexSetBitFromBinary += 1 * pow(2, bitPositionSet);

            }

            bitPositionSet--;

        }

    }
    else if (numSets == 16) {

        bitPositionTag = 23;
        bitPositionSet = 3;

        for (int i = 0; i < 24; i++) {		// Calculates tag bits

            if (binaryArray[i] == 0) {

                tagBitFromBinary += 0 * pow(2, bitPositionTag);

            }
            else if (binaryArray[i] == 1) {

                tagBitFromBinary += 1 * pow(2, bitPositionTag);

            }

            bitPositionTag--;

        }

        for (int i = 24; i < 28; i++) {

            if (binaryArray[i] == 0) {

                indexSetBitFromBinary += 0 * pow(2, bitPositionSet);

            }
            else if (binaryArray[i] == 1) {

                indexSetBitFromBinary += 1 * pow(2, bitPositionSet);

            }

            bitPositionSet--;

        }

    }
    else if (numSets == 4) {

        bitPositionTag = 22;
        bitPositionSet = 1;

        for (int i = 0; i < 23; i++) {		// Calculates tag bits

            if (binaryArray[i] == 0) {

                tagBitFromBinary += 0 * pow(2, bitPositionTag);

            }
            else if (binaryArray[i] == 1) {

                tagBitFromBinary += 1 * pow(2, bitPositionTag);

            }

            bitPositionTag--;

        }

        for (int i = 23; i < 25; i++) {

            if (binaryArray[i] == 0) {

                indexSetBitFromBinary += 0 * pow(2, bitPositionSet);

            }
            else if (binaryArray[i] == 1) {

                indexSetBitFromBinary += 1 * pow(2, bitPositionSet);

            }

            bitPositionSet--;

        }

    }
    else if (numSets == 2) {

        bitPositionTag = 23;
        bitPositionSet = 0;

        for (int i = 0; i < 24; i++) {		// Calculates tag bits

            if (binaryArray[i] == 0) {

                tagBitFromBinary += 0 * pow(2, bitPositionTag);

            }
            else if (binaryArray[i] == 1) {

                tagBitFromBinary += 1 * pow(2, bitPositionTag);

            }

            bitPositionTag--;

        }

        for (int i = 24; i < 25; i++) {

            if (binaryArray[i] == 0) {

                indexSetBitFromBinary += 0 * pow(2, bitPositionSet);

            }
            else if (binaryArray[i] == 1) {

                indexSetBitFromBinary += 1 * pow(2, bitPositionSet);

            }

            bitPositionSet--;

        }

    }



}

void cacheDOperationCPU0() {

    int temp = 0;

    if (cacheDMode0 == 0) {
        temp = addressACache0;
    }
    else if (cacheDMode0 == 1) {
        temp = addressBCache0;
    }

    int bitPositionCache = 7;

    for (int i = 0; i < 8; i++) {

        hexStringCache[i] = '0';

    }

    do {

        if (temp % 16 == 10) {

            hexStringCache[bitPositionCache] = 'A';

        }
        else if (temp % 16 == 11) {

            hexStringCache[bitPositionCache] = 'B';

        }
        else if (temp % 16 == 12) {

            hexStringCache[bitPositionCache] = 'C';

        }
        else if (temp % 16 == 13) {

            hexStringCache[bitPositionCache] = 'D';

        }
        else if (temp % 16 == 14) {

            hexStringCache[bitPositionCache] = 'E';

        }
        else if (temp % 16 == 15) {

            hexStringCache[bitPositionCache] = 'F';

        }
        else if (temp % 16 == 0) {

            hexStringCache[bitPositionCache] = '0';

        }
        else if (temp % 16 == 1) {

            hexStringCache[bitPositionCache] = '1';

        }
        else if (temp % 16 == 2) {

            hexStringCache[bitPositionCache] = '2';

        }
        else if (temp % 16 == 3) {

            hexStringCache[bitPositionCache] = '3';

        }
        else if (temp % 16 == 4) {

            hexStringCache[bitPositionCache] = '4';

        }
        else if (temp % 16 == 5) {

            hexStringCache[bitPositionCache] = '5';

        }
        else if (temp % 16 == 6) {

            hexStringCache[bitPositionCache] = '6';

        }
        else if (temp % 16 == 7) {

            hexStringCache[bitPositionCache] = '7';

        }
        else if (temp % 16 == 8) {

            hexStringCache[bitPositionCache] = '8';

        }
        else if (temp % 16 == 9) {

            hexStringCache[bitPositionCache] = '9';

        }

        temp = temp / 16;

        bitPositionCache--;

    } while (temp / 16 != 0);

    if (temp % 16 == 10) {

        hexStringCache[bitPositionCache] = 'A';

    }
    else if (temp % 16 == 11) {

        hexStringCache[bitPositionCache] = 'B';

    }
    else if (temp % 16 == 12) {

        hexStringCache[bitPositionCache] = 'C';

    }
    else if (temp % 16 == 13) {

        hexStringCache[bitPositionCache] = 'D';

    }
    else if (temp % 16 == 14) {

        hexStringCache[bitPositionCache] = 'E';

    }
    else if (temp % 16 == 15) {

        hexStringCache[bitPositionCache] = 'F';

    }
    else if (temp % 16 == 0) {

        hexStringCache[bitPositionCache] = '0';

    }
    else if (temp % 16 == 1) {

        hexStringCache[bitPositionCache] = '1';

    }
    else if (temp % 16 == 2) {

        hexStringCache[bitPositionCache] = '2';

    }
    else if (temp % 16 == 3) {

        hexStringCache[bitPositionCache] = '3';

    }
    else if (temp % 16 == 4) {

        hexStringCache[bitPositionCache] = '4';

    }
    else if (temp % 16 == 5) {

        hexStringCache[bitPositionCache] = '5';

    }
    else if (temp % 16 == 6) {

        hexStringCache[bitPositionCache] = '6';

    }
    else if (temp % 16 == 7) {

        hexStringCache[bitPositionCache] = '7';

    }
    else if (temp % 16 == 8) {

        hexStringCache[bitPositionCache] = '8';

    }
    else if (temp % 16 == 9) {

        hexStringCache[bitPositionCache] = '9';

    }

    readFromMemoryCPU0();

    readBinaryValue(cacheDCPU0.numberOfSets);

    if (cacheDMode0 == 0) {      // Performs cache for load A

        if (cacheDCPU0PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary || cacheDCPU0PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {				// Determines hit or misses for cache 1

            cacheDCPU0.numberOfHits++;

            LRUOptimizeCacheHitDCPU0();

            hitOrMissCacheD0 = 1;



        }
        else {


            cacheDCPU0.numberOfMisses++;

            LRUOptimizeCacheMissDCPU0();

            hitOrMissCacheD0 = 0;

        }

        addressACache0 += 4;

        //cacheDCPU0PositionArray[indexSetBitFromBinary] = A.mainArray[iterationACache0];		// Assigns tag at location  



        iterationACache0++;

    }
    else if (cacheDMode0 == 1) {                // Performs cache for load B

        if (cacheDCPU0PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary || cacheDCPU0PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {				// Determines hit or misses for cache 1

            cacheDCPU0.numberOfHits++;

            LRUOptimizeCacheHitDCPU0();

            hitOrMissCacheD0 = 1;

        }
        else {


            cacheDCPU0.numberOfMisses++;

            LRUOptimizeCacheMissDCPU0();

            hitOrMissCacheD0 = 0;

        }

        addressBCache0 += 4;

        //cacheDCPU0PositionArray[indexSetBitFromBinary] = B.mainArray[iterationBCache0];		// Assigns tag at location   


        iterationBCache0++;

    }
    cacheDCPU0.totalAccesses++;

}

void cacheDOperationCPU1() {

    int temp = 0;

    if (cacheDMode1 == 0) {
        temp = addressACache1;
    }
    else if (cacheDMode1 == 1) {
        temp = addressBCache1;
    }

    int bitPositionCache = 7;

    for (int i = 0; i < 8; i++) {

        hexStringCache[i] = '0';

    }

    do {

        if (temp % 16 == 10) {

            hexStringCache[bitPositionCache] = 'A';

        }
        else if (temp % 16 == 11) {

            hexStringCache[bitPositionCache] = 'B';

        }
        else if (temp % 16 == 12) {

            hexStringCache[bitPositionCache] = 'C';

        }
        else if (temp % 16 == 13) {

            hexStringCache[bitPositionCache] = 'D';

        }
        else if (temp % 16 == 14) {

            hexStringCache[bitPositionCache] = 'E';

        }
        else if (temp % 16 == 15) {

            hexStringCache[bitPositionCache] = 'F';

        }
        else if (temp % 16 == 0) {

            hexStringCache[bitPositionCache] = '0';

        }
        else if (temp % 16 == 1) {

            hexStringCache[bitPositionCache] = '1';

        }
        else if (temp % 16 == 2) {

            hexStringCache[bitPositionCache] = '2';

        }
        else if (temp % 16 == 3) {

            hexStringCache[bitPositionCache] = '3';

        }
        else if (temp % 16 == 4) {

            hexStringCache[bitPositionCache] = '4';

        }
        else if (temp % 16 == 5) {

            hexStringCache[bitPositionCache] = '5';

        }
        else if (temp % 16 == 6) {

            hexStringCache[bitPositionCache] = '6';

        }
        else if (temp % 16 == 7) {

            hexStringCache[bitPositionCache] = '7';

        }
        else if (temp % 16 == 8) {

            hexStringCache[bitPositionCache] = '8';

        }
        else if (temp % 16 == 9) {

            hexStringCache[bitPositionCache] = '9';

        }

        temp = temp / 16;

        bitPositionCache--;

    } while (temp / 16 != 0);

    if (temp % 16 == 10) {

        hexStringCache[bitPositionCache] = 'A';

    }
    else if (temp % 16 == 11) {

        hexStringCache[bitPositionCache] = 'B';

    }
    else if (temp % 16 == 12) {

        hexStringCache[bitPositionCache] = 'C';

    }
    else if (temp % 16 == 13) {

        hexStringCache[bitPositionCache] = 'D';

    }
    else if (temp % 16 == 14) {

        hexStringCache[bitPositionCache] = 'E';

    }
    else if (temp % 16 == 15) {

        hexStringCache[bitPositionCache] = 'F';

    }
    else if (temp % 16 == 0) {

        hexStringCache[bitPositionCache] = '0';

    }
    else if (temp % 16 == 1) {

        hexStringCache[bitPositionCache] = '1';

    }
    else if (temp % 16 == 2) {

        hexStringCache[bitPositionCache] = '2';

    }
    else if (temp % 16 == 3) {

        hexStringCache[bitPositionCache] = '3';

    }
    else if (temp % 16 == 4) {

        hexStringCache[bitPositionCache] = '4';

    }
    else if (temp % 16 == 5) {

        hexStringCache[bitPositionCache] = '5';

    }
    else if (temp % 16 == 6) {

        hexStringCache[bitPositionCache] = '6';

    }
    else if (temp % 16 == 7) {

        hexStringCache[bitPositionCache] = '7';

    }
    else if (temp % 16 == 8) {

        hexStringCache[bitPositionCache] = '8';

    }
    else if (temp % 16 == 9) {

        hexStringCache[bitPositionCache] = '9';

    }


    readFromMemoryCPU0();

    readBinaryValue(cacheDCPU1.numberOfSets);

    if (cacheDMode1 == 0) {      // Performs cache for load A

        if (cacheDCPU1PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary || (cacheDCPU1PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary)) {				// Determines hit or misses for cache 1

            cacheDCPU1.numberOfHits++;

            LRUOptimizeCacheHitDCPU1();

            hitOrMissCacheD1 = 1;

        }
        else {


            cacheDCPU1.numberOfMisses++;

            LRUOptimizeCacheMissDCPU1();

            hitOrMissCacheD1 = 0;

        }

        addressACache1 += 4;

        // cacheDCPU1PositionArray[indexSetBitFromBinary] = A.mainArray[iterationACache1];		// Assigns tag at location   



        iterationACache1++;

    }
    else if (cacheDMode1 == 1) {                // Performs cache for load B

        if (cacheDCPU1PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary || cacheDCPU1PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {				// Determines hit or misses for cache 1

            cacheDCPU1.numberOfHits++;

            LRUOptimizeCacheHitDCPU1();

            hitOrMissCacheD1 = 1;

        }
        else {


            cacheDCPU1.numberOfMisses++;

            LRUOptimizeCacheMissDCPU1();


            hitOrMissCacheD1 = 0;

        }

        addressBCache1 += 4;

        //cacheDCPU1PositionArray[indexSetBitFromBinary] = B.mainArray[iterationBCache1];		// Assigns tag at location  


        iterationBCache1++;

    }

    cacheDCPU1.totalAccesses++;

}

void cacheIOperationCPU0() {


    for (int i = 0; i < 32; i++) {
        tempArray[i] = (int)lineDecodeAdd[i];

        if (tempArray[i] == 48) {               // If '0' in ACSII set 0 integer
            binaryArray[i] = 0;
        }
        else if (tempArray[i] == 49) {        // If '1' in ACSII set 1 integer
            binaryArray[i] = 1;
        }
    }

    readBinaryValue(cacheICPU0.numberOfSets);

    if (cacheICPU0PositionArray[indexSetBitFromBinary][0] == count32InstructionAdd || cacheICPU0PositionArray[indexSetBitFromBinary][1] == count32InstructionAdd) {				// Determines hit or misses for cache 1

        cacheICPU0.numberOfHits++;

        LRUOptimizeCacheHitICPU0();

        hitOrMissCacheI0 = 1;

    }
    else {


        cacheICPU0.numberOfMisses++;

        LRUOptimizeCacheMissICPU0();

        hitOrMissCacheI0 = 0;

    }

    //cacheICPU0PositionArray[indexSetBitFromBinary] = tagBitFromBinary;		// Assigns tag at location   


    cacheICPU0.totalAccesses++;

}

void cacheIOperationCPU1() {


    for (int i = 0; i < 32; i++) {
        tempArray[i] = (int)lineDecodeSub[i];

        if (tempArray[i] == 48) {               // If '0' in ACSII set 0 integer
            binaryArray[i] = 0;
        }
        else if (tempArray[i] == 49) {        // If '1' in ACSII set 1 integer
            binaryArray[i] = 1;
        }
    }

    readBinaryValue(cacheICPU1.numberOfSets);

    if (cacheICPU1PositionArray[indexSetBitFromBinary][0] == count32InstructionSub || cacheICPU1PositionArray[indexSetBitFromBinary][1] == count32InstructionSub) {				// Determines hit or misses for cache 1

        cacheICPU1.numberOfHits++;

        LRUOptimizeCacheHitICPU1();

        hitOrMissCacheI1 = 1;

    }
    else {


        cacheICPU1.numberOfMisses++;

        LRUOptimizeCacheMissICPU1();

        hitOrMissCacheI1 = 0;

    }

    //cacheICPU1PositionArray[indexSetBitFromBinary] = tagBitFromBinary;		// Assigns tag at location   

    cacheICPU1.totalAccesses++;

}

void LRUOptimizeCacheMissICPU0() {

    int temp;

    if (indexSetBitFromBinary == 0 && cacheICPU0PositionArray[0][0] == -1) {

        cacheICPU0PositionArray[0][0] = count32InstructionAdd;		// Assigns tag at location

        if (indexSetBitFromBinary == 0) {

            lastElementUsed1ICPU0Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 0 && cacheICPU0PositionArray[0][1] == -1) {

        cacheICPU0PositionArray[0][1] = count32InstructionAdd;		// Assigns tag at location

        if (indexSetBitFromBinary == 0) {

            lastElementUsed1ICPU0Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 1 && cacheICPU0PositionArray[1][0] == -1) {

        cacheICPU0PositionArray[1][0] = count32InstructionAdd;		// Assigns tag at location

        if (indexSetBitFromBinary == 1) {

            lastElementUsed2ICPU0Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 1 && cacheICPU0PositionArray[1][1] == -1) {

        cacheICPU0PositionArray[1][1] = count32InstructionAdd;		// Assigns tag at location

        if (indexSetBitFromBinary == 1) {

            lastElementUsed2ICPU0Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 2 && cacheICPU0PositionArray[2][0] == -1) {

        cacheICPU0PositionArray[2][0] = count32InstructionAdd;		// Assigns tag at location

        if (indexSetBitFromBinary == 2) {

            lastElementUsed3ICPU0Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 2 && cacheICPU0PositionArray[2][1] == -1) {

        cacheICPU0PositionArray[2][1] = count32InstructionAdd;		// Assigns tag at location

        if (indexSetBitFromBinary == 2) {

            lastElementUsed3ICPU0Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 3 && cacheICPU0PositionArray[3][0] == -1) {

        cacheICPU0PositionArray[3][0] = count32InstructionAdd;		// Assigns tag at location

        if (indexSetBitFromBinary == 3) {

            lastElementUsed4ICPU0Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 3 && cacheICPU0PositionArray[3][1] == -1) {

        cacheICPU0PositionArray[3][1] = count32InstructionAdd;		// Assigns tag at location

        if (indexSetBitFromBinary == 3) {

            lastElementUsed4ICPU0Optimize.push_back(1);

        }

    }
    else {

        if (indexSetBitFromBinary == 0) {

            temp = lastElementUsed1ICPU0Optimize.back();

            lastElementUsed1ICPU0Optimize.pop_back();

            if (temp == 0) {

                cacheICPU0PositionArray[0][temp] = count32InstructionAdd;
                lastElementUsed1ICPU0Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheICPU0PositionArray[0][temp] = count32InstructionAdd;
                lastElementUsed1ICPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 1) {

            temp = lastElementUsed2ICPU0Optimize.back();

            lastElementUsed2ICPU0Optimize.pop_back();

            if (temp == 0) {

                cacheICPU0PositionArray[1][temp] = count32InstructionAdd;
                lastElementUsed2ICPU0Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheICPU0PositionArray[1][temp] = count32InstructionAdd;
                lastElementUsed2ICPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 2) {

            temp = lastElementUsed3ICPU0Optimize.back();

            lastElementUsed3ICPU0Optimize.pop_back();

            if (temp == 0) {

                cacheICPU0PositionArray[2][temp] = count32InstructionAdd;
                lastElementUsed3ICPU0Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheICPU0PositionArray[2][temp] = count32InstructionAdd;
                lastElementUsed3ICPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 3) {

            temp = lastElementUsed4ICPU0Optimize.back();

            lastElementUsed4ICPU0Optimize.pop_back();

            if (temp == 0) {

                cacheICPU0PositionArray[3][temp] = count32InstructionAdd;
                lastElementUsed4ICPU0Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheICPU0PositionArray[3][temp] = count32InstructionAdd;
                lastElementUsed4ICPU0Optimize.push_back(1);

            }

        }

    }

}

void LRUOptimizeCacheHitICPU0() {

    if (indexSetBitFromBinary == 0) {

        if (cacheICPU0PositionArray[indexSetBitFromBinary][0] == count32InstructionAdd) {

            for (int i = 0; i < lastElementUsed1ICPU0Optimize.size(); i++) {

                if (lastElementUsed1ICPU0Optimize.at(i) == 0) {
                    lastElementUsed1ICPU0Optimize.erase(lastElementUsed1ICPU0Optimize.begin() + i);
                }

            }

            lastElementUsed1ICPU0Optimize.push_back(0);

        }
        else if (cacheICPU0PositionArray[indexSetBitFromBinary][1] == count32InstructionAdd) {

            for (int i = 0; i < lastElementUsed1ICPU0Optimize.size(); i++) {

                if (lastElementUsed1ICPU0Optimize.at(i) == 1) {
                    lastElementUsed1ICPU0Optimize.erase(lastElementUsed1ICPU0Optimize.begin() + i);
                }

            }

            lastElementUsed1ICPU0Optimize.push_back(1);

        }
        else if (indexSetBitFromBinary == 1) {

            if (cacheICPU0PositionArray[indexSetBitFromBinary][0] == count32InstructionAdd) {

                for (int i = 0; i < lastElementUsed2ICPU0Optimize.size(); i++) {

                    if (lastElementUsed2ICPU0Optimize.at(i) == 0) {
                        lastElementUsed2ICPU0Optimize.erase(lastElementUsed2ICPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed2ICPU0Optimize.push_back(0);

            }
            else if (cacheICPU0PositionArray[indexSetBitFromBinary][1] == count32InstructionAdd) {

                for (int i = 0; i < lastElementUsed2ICPU0Optimize.size(); i++) {

                    if (lastElementUsed2ICPU0Optimize.at(i) == 1) {
                        lastElementUsed2ICPU0Optimize.erase(lastElementUsed2ICPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed2ICPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 2) {

            if (cacheICPU0PositionArray[indexSetBitFromBinary][0] == count32InstructionAdd) {

                for (int i = 0; i < lastElementUsed3ICPU0Optimize.size(); i++) {

                    if (lastElementUsed3ICPU0Optimize.at(i) == 0) {
                        lastElementUsed3ICPU0Optimize.erase(lastElementUsed3ICPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed3ICPU0Optimize.push_back(0);

            }
            else if (cacheICPU0PositionArray[indexSetBitFromBinary][1] == count32InstructionAdd) {

                for (int i = 0; i < lastElementUsed3ICPU0Optimize.size(); i++) {

                    if (lastElementUsed3ICPU0Optimize.at(i) == 1) {
                        lastElementUsed3ICPU0Optimize.erase(lastElementUsed3ICPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed3ICPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 3) {

            if (cacheICPU0PositionArray[indexSetBitFromBinary][0] == count32InstructionAdd) {

                for (int i = 0; i < lastElementUsed4ICPU0Optimize.size(); i++) {

                    if (lastElementUsed4ICPU0Optimize.at(i) == 0) {
                        lastElementUsed4ICPU0Optimize.erase(lastElementUsed4ICPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed4ICPU0Optimize.push_back(0);

            }
            else if (cacheICPU0PositionArray[indexSetBitFromBinary][1] == count32InstructionAdd) {

                for (int i = 0; i < lastElementUsed4ICPU0Optimize.size(); i++) {

                    if (lastElementUsed4ICPU0Optimize.at(i) == 1) {
                        lastElementUsed4ICPU0Optimize.erase(lastElementUsed4ICPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed4ICPU0Optimize.push_back(1);

            }

        }

    }

}

void LRUOptimizeCacheMissDCPU0() {

    int temp;

    if (indexSetBitFromBinary == 0 && cacheDCPU0PositionArray[0][0] == -1) {

        cacheDCPU0PositionArray[0][0] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 0) {

            lastElementUsed1DCPU0Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 0 && cacheDCPU0PositionArray[0][1] == -1) {

        cacheDCPU0PositionArray[0][1] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 0) {

            lastElementUsed1DCPU0Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 1 && cacheDCPU0PositionArray[1][0] == -1) {

        cacheDCPU0PositionArray[1][0] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 1) {

            lastElementUsed2DCPU0Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 1 && cacheDCPU0PositionArray[1][1] == -1) {

        cacheDCPU0PositionArray[1][1] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 1) {

            lastElementUsed2DCPU0Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 2 && cacheDCPU0PositionArray[2][0] == -1) {

        cacheDCPU0PositionArray[2][0] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 2) {

            lastElementUsed3DCPU0Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 2 && cacheDCPU0PositionArray[2][1] == -1) {

        cacheDCPU0PositionArray[2][1] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 2) {

            lastElementUsed3DCPU0Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 3 && cacheDCPU0PositionArray[3][0] == -1) {

        cacheDCPU0PositionArray[3][0] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 3) {

            lastElementUsed4DCPU0Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 3 && cacheDCPU0PositionArray[3][1] == -1) {

        cacheDCPU0PositionArray[3][1] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 3) {

            lastElementUsed4DCPU0Optimize.push_back(1);

        }

    }
    else {

        if (indexSetBitFromBinary == 0) {

            temp = lastElementUsed1DCPU0Optimize.back();

            lastElementUsed1DCPU0Optimize.pop_back();

            if (temp == 0) {

                cacheDCPU0PositionArray[0][temp] = tagBitFromBinary;
                lastElementUsed1DCPU0Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheDCPU0PositionArray[0][temp] = tagBitFromBinary;
                lastElementUsed1DCPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 1) {

            temp = lastElementUsed2DCPU0Optimize.back();

            lastElementUsed2DCPU0Optimize.pop_back();

            if (temp == 0) {

                cacheDCPU0PositionArray[1][temp] = tagBitFromBinary;
                lastElementUsed2DCPU0Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheDCPU0PositionArray[1][temp] = tagBitFromBinary;
                lastElementUsed2DCPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 2) {

            temp = lastElementUsed3DCPU0Optimize.back();

            lastElementUsed3DCPU0Optimize.pop_back();

            if (temp == 0) {

                cacheDCPU0PositionArray[2][temp] = tagBitFromBinary;
                lastElementUsed3DCPU0Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheDCPU0PositionArray[2][temp] = tagBitFromBinary;
                lastElementUsed3DCPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 3) {

            temp = lastElementUsed4DCPU0Optimize.back();

            lastElementUsed4DCPU0Optimize.pop_back();

            if (temp == 0) {

                cacheDCPU0PositionArray[3][temp] = tagBitFromBinary;
                lastElementUsed4DCPU0Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheDCPU0PositionArray[3][temp] = tagBitFromBinary;
                lastElementUsed4DCPU0Optimize.push_back(1);

            }

        }

    }

}

void LRUOptimizeCacheHitDCPU0() {

    if (indexSetBitFromBinary == 0) {

        if (cacheDCPU0PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

            for (int i = 0; i < lastElementUsed1DCPU0Optimize.size(); i++) {

                if (lastElementUsed1DCPU0Optimize.at(i) == 0) {
                    lastElementUsed1DCPU0Optimize.erase(lastElementUsed1DCPU0Optimize.begin() + i);
                }

            }

            lastElementUsed1DCPU0Optimize.push_back(0);

        }
        else if (cacheDCPU0PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

            for (int i = 0; i < lastElementUsed1DCPU0Optimize.size(); i++) {

                if (lastElementUsed1DCPU0Optimize.at(i) == 1) {
                    lastElementUsed1DCPU0Optimize.erase(lastElementUsed1DCPU0Optimize.begin() + i);
                }

            }

            lastElementUsed1DCPU0Optimize.push_back(1);

        }
        else if (indexSetBitFromBinary == 1) {

            if (cacheDCPU0PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed2DCPU0Optimize.size(); i++) {

                    if (lastElementUsed2DCPU0Optimize.at(i) == 0) {
                        lastElementUsed2DCPU0Optimize.erase(lastElementUsed2DCPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed2DCPU0Optimize.push_back(0);

            }
            else if (cacheDCPU0PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed2DCPU0Optimize.size(); i++) {

                    if (lastElementUsed2DCPU0Optimize.at(i) == 1) {
                        lastElementUsed2DCPU0Optimize.erase(lastElementUsed2DCPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed2DCPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 2) {

            if (cacheDCPU0PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed3DCPU0Optimize.size(); i++) {

                    if (lastElementUsed3DCPU0Optimize.at(i) == 0) {
                        lastElementUsed3DCPU0Optimize.erase(lastElementUsed3DCPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed3DCPU0Optimize.push_back(0);

            }
            else if (cacheDCPU0PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed3DCPU0Optimize.size(); i++) {

                    if (lastElementUsed3DCPU0Optimize.at(i) == 1) {
                        lastElementUsed3DCPU0Optimize.erase(lastElementUsed3DCPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed3DCPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 3) {

            if (cacheDCPU0PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed4DCPU0Optimize.size(); i++) {

                    if (lastElementUsed4DCPU0Optimize.at(i) == 0) {
                        lastElementUsed4DCPU0Optimize.erase(lastElementUsed4DCPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed4DCPU0Optimize.push_back(0);

            }
            else if (cacheDCPU0PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed4DCPU0Optimize.size(); i++) {

                    if (lastElementUsed4DCPU0Optimize.at(i) == 1) {
                        lastElementUsed4DCPU0Optimize.erase(lastElementUsed4DCPU0Optimize.begin() + i);
                    }

                }

                lastElementUsed4DCPU0Optimize.push_back(1);

            }

        }

    }

}

void LRUOptimizeCacheMissICPU1() {

    int temp;

    if (indexSetBitFromBinary == 0 && cacheICPU1PositionArray[0][0] == -1) {

        cacheICPU1PositionArray[0][0] = count32InstructionSub;		// Assigns tag at location

        if (indexSetBitFromBinary == 0) {

            lastElementUsed1ICPU1Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 0 && cacheICPU1PositionArray[0][1] == -1) {

        cacheICPU1PositionArray[0][1] = count32InstructionSub;		// Assigns tag at location

        if (indexSetBitFromBinary == 0) {

            lastElementUsed1ICPU1Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 1 && cacheICPU1PositionArray[1][0] == -1) {

        cacheICPU1PositionArray[1][0] = count32InstructionSub;		// Assigns tag at location

        if (indexSetBitFromBinary == 1) {

            lastElementUsed2ICPU1Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 1 && cacheICPU1PositionArray[1][1] == -1) {

        cacheICPU1PositionArray[1][1] = count32InstructionSub;		// Assigns tag at location

        if (indexSetBitFromBinary == 1) {

            lastElementUsed2ICPU1Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 2 && cacheICPU1PositionArray[2][0] == -1) {

        cacheICPU1PositionArray[2][0] = count32InstructionSub;		// Assigns tag at location

        if (indexSetBitFromBinary == 2) {

            lastElementUsed3ICPU1Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 2 && cacheICPU1PositionArray[2][1] == -1) {

        cacheICPU1PositionArray[2][1] = count32InstructionSub;		// Assigns tag at location

        if (indexSetBitFromBinary == 2) {

            lastElementUsed3ICPU1Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 3 && cacheICPU1PositionArray[3][0] == -1) {

        cacheICPU1PositionArray[3][0] = count32InstructionSub;		// Assigns tag at location

        if (indexSetBitFromBinary == 3) {

            lastElementUsed4ICPU1Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 3 && cacheICPU1PositionArray[3][1] == -1) {

        cacheICPU1PositionArray[3][1] = count32InstructionSub;		// Assigns tag at location

        if (indexSetBitFromBinary == 3) {

            lastElementUsed4ICPU1Optimize.push_back(1);

        }

    }
    else {

        if (indexSetBitFromBinary == 0) {

            temp = lastElementUsed1ICPU1Optimize.back();

            lastElementUsed1ICPU1Optimize.pop_back();

            if (temp == 0) {

                cacheICPU1PositionArray[0][temp] = count32InstructionSub;
                lastElementUsed1ICPU1Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheICPU1PositionArray[0][temp] = count32InstructionSub;
                lastElementUsed1ICPU1Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 1) {

            temp = lastElementUsed2ICPU1Optimize.back();

            lastElementUsed2ICPU1Optimize.pop_back();

            if (temp == 0) {

                cacheICPU1PositionArray[1][temp] = count32InstructionSub;
                lastElementUsed2ICPU1Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheICPU1PositionArray[1][temp] = count32InstructionSub;
                lastElementUsed2ICPU1Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 2) {

            temp = lastElementUsed3ICPU1Optimize.back();

            lastElementUsed3ICPU1Optimize.pop_back();

            if (temp == 0) {

                cacheICPU1PositionArray[2][temp] = count32InstructionSub;
                lastElementUsed3ICPU1Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheICPU1PositionArray[2][temp] = count32InstructionSub;
                lastElementUsed3ICPU1Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 3) {

            temp = lastElementUsed4ICPU1Optimize.back();

            lastElementUsed4ICPU1Optimize.pop_back();

            if (temp == 0) {

                cacheICPU1PositionArray[3][temp] = count32InstructionSub;
                lastElementUsed4ICPU1Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheICPU1PositionArray[3][temp] = count32InstructionSub;
                lastElementUsed4ICPU1Optimize.push_back(1);

            }

        }

    }

}

void LRUOptimizeCacheHitICPU1() {

    if (indexSetBitFromBinary == 0) {

        if (cacheICPU1PositionArray[indexSetBitFromBinary][0] == count32InstructionSub) {

            for (int i = 0; i < lastElementUsed1ICPU1Optimize.size(); i++) {

                if (lastElementUsed1ICPU1Optimize.at(i) == 0) {
                    lastElementUsed1ICPU1Optimize.erase(lastElementUsed1ICPU1Optimize.begin() + i);
                }

            }

            lastElementUsed1ICPU1Optimize.push_back(0);

        }
        else if (cacheICPU1PositionArray[indexSetBitFromBinary][1] == count32InstructionSub) {

            for (int i = 0; i < lastElementUsed1ICPU1Optimize.size(); i++) {

                if (lastElementUsed1ICPU1Optimize.at(i) == 1) {
                    lastElementUsed1ICPU1Optimize.erase(lastElementUsed1ICPU1Optimize.begin() + i);
                }

            }

            lastElementUsed1ICPU1Optimize.push_back(1);

        }
        else if (indexSetBitFromBinary == 1) {

            if (cacheICPU1PositionArray[indexSetBitFromBinary][0] == count32InstructionSub) {

                for (int i = 0; i < lastElementUsed2ICPU1Optimize.size(); i++) {

                    if (lastElementUsed2ICPU1Optimize.at(i) == 0) {
                        lastElementUsed2ICPU1Optimize.erase(lastElementUsed2ICPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed2ICPU1Optimize.push_back(0);

            }
            else if (cacheICPU1PositionArray[indexSetBitFromBinary][1] == count32InstructionSub) {

                for (int i = 0; i < lastElementUsed2ICPU1Optimize.size(); i++) {

                    if (lastElementUsed2ICPU1Optimize.at(i) == 1) {
                        lastElementUsed2ICPU1Optimize.erase(lastElementUsed2ICPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed2ICPU1Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 2) {

            if (cacheICPU1PositionArray[indexSetBitFromBinary][0] == count32InstructionSub) {

                for (int i = 0; i < lastElementUsed3ICPU1Optimize.size(); i++) {

                    if (lastElementUsed3ICPU1Optimize.at(i) == 0) {
                        lastElementUsed3ICPU1Optimize.erase(lastElementUsed3ICPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed3ICPU1Optimize.push_back(0);

            }
            else if (cacheICPU1PositionArray[indexSetBitFromBinary][1] == count32InstructionSub) {

                for (int i = 0; i < lastElementUsed3ICPU1Optimize.size(); i++) {

                    if (lastElementUsed3ICPU1Optimize.at(i) == 1) {
                        lastElementUsed3ICPU1Optimize.erase(lastElementUsed3ICPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed3ICPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 3) {

            if (cacheICPU1PositionArray[indexSetBitFromBinary][0] == count32InstructionSub) {

                for (int i = 0; i < lastElementUsed4ICPU1Optimize.size(); i++) {

                    if (lastElementUsed4ICPU1Optimize.at(i) == 0) {
                        lastElementUsed4ICPU1Optimize.erase(lastElementUsed4ICPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed4ICPU1Optimize.push_back(0);

            }
            else if (cacheICPU1PositionArray[indexSetBitFromBinary][1] == count32InstructionSub) {

                for (int i = 0; i < lastElementUsed4ICPU1Optimize.size(); i++) {

                    if (lastElementUsed4ICPU1Optimize.at(i) == 1) {
                        lastElementUsed4ICPU1Optimize.erase(lastElementUsed4ICPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed4ICPU1Optimize.push_back(1);

            }

        }

    }

}

void LRUOptimizeCacheMissDCPU1() {

    int temp;

    if (indexSetBitFromBinary == 0 && cacheDCPU1PositionArray[0][0] == -1) {

        cacheDCPU1PositionArray[0][0] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 0) {

            lastElementUsed1ICPU1Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 0 && cacheDCPU1PositionArray[0][1] == -1) {

        cacheDCPU1PositionArray[0][1] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 0) {

            lastElementUsed1DCPU1Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 1 && cacheDCPU1PositionArray[1][0] == -1) {

        cacheDCPU1PositionArray[1][0] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 1) {

            lastElementUsed2DCPU1Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 1 && cacheDCPU1PositionArray[1][1] == -1) {

        cacheDCPU1PositionArray[1][1] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 1) {

            lastElementUsed2DCPU1Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 2 && cacheDCPU1PositionArray[2][0] == -1) {

        cacheDCPU1PositionArray[2][0] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 2) {

            lastElementUsed3DCPU1Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 2 && cacheDCPU1PositionArray[2][1] == -1) {

        cacheDCPU1PositionArray[2][1] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 2) {

            lastElementUsed3DCPU1Optimize.push_back(1);

        }

    }
    else if (indexSetBitFromBinary == 3 && cacheDCPU1PositionArray[3][0] == -1) {

        cacheDCPU1PositionArray[3][0] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 3) {

            lastElementUsed4DCPU1Optimize.push_back(0);

        }

    }
    else if (indexSetBitFromBinary == 3 && cacheDCPU1PositionArray[3][1] == -1) {

        cacheDCPU1PositionArray[3][1] = tagBitFromBinary;		// Assigns tag at location

        if (indexSetBitFromBinary == 3) {

            lastElementUsed4DCPU1Optimize.push_back(1);

        }

    }
    else {

        if (indexSetBitFromBinary == 0) {

            temp = lastElementUsed1DCPU1Optimize.back();

            lastElementUsed1DCPU1Optimize.pop_back();

            if (temp == 0) {

                cacheDCPU1PositionArray[0][temp] = tagBitFromBinary;
                lastElementUsed1DCPU1Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheDCPU1PositionArray[0][temp] = tagBitFromBinary;
                lastElementUsed1DCPU1Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 1) {

            temp = lastElementUsed2DCPU1Optimize.back();

            lastElementUsed2DCPU1Optimize.pop_back();

            if (temp == 0) {

                cacheDCPU1PositionArray[1][temp] = tagBitFromBinary;
                lastElementUsed2DCPU1Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheDCPU1PositionArray[1][temp] = tagBitFromBinary;
                lastElementUsed2DCPU1Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 2) {

            temp = lastElementUsed3DCPU1Optimize.back();

            lastElementUsed3DCPU1Optimize.pop_back();

            if (temp == 0) {

                cacheDCPU1PositionArray[2][temp] = tagBitFromBinary;
                lastElementUsed3DCPU1Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheDCPU1PositionArray[2][temp] = tagBitFromBinary;
                lastElementUsed3DCPU1Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 3) {

            temp = lastElementUsed4DCPU1Optimize.back();

            lastElementUsed4DCPU1Optimize.pop_back();

            if (temp == 0) {

                cacheDCPU1PositionArray[3][temp] = tagBitFromBinary;
                lastElementUsed4DCPU1Optimize.push_back(0);

            }
            else if (temp == 1) {

                cacheDCPU1PositionArray[3][temp] = tagBitFromBinary;
                lastElementUsed4DCPU1Optimize.push_back(1);

            }

        }

    }

}

void LRUOptimizeCacheHitDCPU1() {

    if (indexSetBitFromBinary == 0) {

        if (cacheDCPU1PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

            for (int i = 0; i < lastElementUsed1DCPU1Optimize.size(); i++) {

                if (lastElementUsed1DCPU1Optimize.at(i) == 0) {
                    lastElementUsed1DCPU1Optimize.erase(lastElementUsed1DCPU1Optimize.begin() + i);
                }

            }

            lastElementUsed1DCPU1Optimize.push_back(0);

        }
        else if (cacheDCPU1PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

            for (int i = 0; i < lastElementUsed1DCPU1Optimize.size(); i++) {

                if (lastElementUsed1DCPU1Optimize.at(i) == 1) {
                    lastElementUsed1DCPU1Optimize.erase(lastElementUsed1DCPU1Optimize.begin() + i);
                }

            }

            lastElementUsed1DCPU1Optimize.push_back(1);

        }
        else if (indexSetBitFromBinary == 1) {

            if (cacheDCPU1PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed2DCPU1Optimize.size(); i++) {

                    if (lastElementUsed2DCPU1Optimize.at(i) == 0) {
                        lastElementUsed2DCPU1Optimize.erase(lastElementUsed2DCPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed2DCPU1Optimize.push_back(0);

            }
            else if (cacheDCPU1PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed2DCPU1Optimize.size(); i++) {

                    if (lastElementUsed2DCPU1Optimize.at(i) == 1) {
                        lastElementUsed2DCPU1Optimize.erase(lastElementUsed2DCPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed2DCPU1Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 2) {

            if (cacheDCPU1PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed3DCPU1Optimize.size(); i++) {

                    if (lastElementUsed3DCPU1Optimize.at(i) == 0) {
                        lastElementUsed3DCPU1Optimize.erase(lastElementUsed3DCPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed3DCPU1Optimize.push_back(0);

            }
            else if (cacheDCPU1PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed3DCPU1Optimize.size(); i++) {

                    if (lastElementUsed3DCPU1Optimize.at(i) == 1) {
                        lastElementUsed3DCPU1Optimize.erase(lastElementUsed3DCPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed3ICPU0Optimize.push_back(1);

            }

        }
        else if (indexSetBitFromBinary == 3) {

            if (cacheDCPU1PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed4DCPU1Optimize.size(); i++) {

                    if (lastElementUsed4DCPU1Optimize.at(i) == 0) {
                        lastElementUsed4DCPU1Optimize.erase(lastElementUsed4DCPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed4DCPU1Optimize.push_back(0);

            }
            else if (cacheDCPU1PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

                for (int i = 0; i < lastElementUsed4DCPU1Optimize.size(); i++) {

                    if (lastElementUsed4DCPU1Optimize.at(i) == 1) {
                        lastElementUsed4DCPU1Optimize.erase(lastElementUsed4DCPU1Optimize.begin() + i);
                    }

                }

                lastElementUsed4DCPU1Optimize.push_back(1);

            }

        }

    }

}
