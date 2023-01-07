//***********************************************************************
// ECGR 5181 - Assignment 2 - 4-Stage Pipelined CPU		(C++ Verision)	*
//																		*
// This program will utilize custom structures/vectors to simulate how	*
// a computer processor would pipeline a looped event in assembly		*
// instructions via C code.												*
// 																		*															
// Kendall Britton, 2022-09-26											* 
//***********************************************************************

// List of libraries that are required to run the program
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

enum instruction { Load, Add, Store, Addi, BNE, Halt };		// Translated the instructions into a numbered identifier

#define UPPERLOOPITERATER  100

int memoryArray[1000];

// The data structure to create/process an event
typedef struct Pipeline {
	int x1 = 20;
	int x2 = 0;
	int x3;
	int instance = 20; 
	int clockCycleNumber = 1;
	int branchTF;
	int preload = 0;
	int stall;

	// Handles instructions in the Fetch Stage
	void Fetch(int instruction) {

		switch (instruction) {
			case Load:
				cout << "Fetch Stage: { Instruction: Load, ld x1 = x[" << instance << "], Data x1 = " << x1 << ", Clock Cycle Number : " << clockCycleNumber << " }" << endl;
				break;
			case Add: 
				cout << "Fetch Stage: { Instruction: Add, add.d x3 = x1 + x2, Data x3 = " << x3 << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
				break;
			case instruction::Store:
				cout << "Fetch Stage: { Instruction: Store, sd x[" << instance << "] = x3, Data x[" << instance << "] = " << x1 << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
				break;
			case Addi:
				cout << "Fetch Stage: { Instruction: Addi, addi x[" << instance << "] = x[" << instance << "] - 4, Data x[" << instance << "] = x[" << instance - 4 << "], Clock Cycle Number: " << clockCycleNumber << " }" << endl;
				break;
			case BNE:
				branchTF = 0;
				preload = 1;
				cout << "Fetch Stage: { Instruction: BNE, bne x1 - x2 = 0 ? Halt : Loop, Data = " << branchTF << ", Clock Cycle Number: " << clockCycleNumber << "}" << endl;
				break;
			default:
				cout << "Fetch Stage: { No Operation }"  << endl;
				break;
		}

	}

	// Handles instructions in the Decode Stage
	void Decode(int instruction) {

		switch (instruction) {
		case Load:
			cout << "Decode Stage: { Instruction: Load, ld x1 = x[" << instance << "], Data x1 = " << x1 << ", Clock Cycle Number : " << clockCycleNumber << " }" << endl;
			break;
		case Add:
			cout << "Decode Stage: { Instruction: Add, add.d x3 = x1 + x2, Data x3 = " << x3 << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		case instruction::Store:
			cout << "Decode Stage: { Instruction: Store, sd x[" << instance << "] = x3, Data x[" << instance << "] = " << x1 << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		case Addi:
			cout << "Decode Stage: { Instruction: Addi, addi x[" << instance << "] = x[" << instance << "] - 4, Data x[" << instance << "] = x[" << instance - 4 << "], Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		case BNE:
			cout << "Decode Stage: { Instruction: BNE, bne x1 - x2 = 0 ? Halt : Loop, Data = " << branchTF << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		default:
			cout << "Decode Stage: { No Operation }" << endl;
			break;
		}


	}
	
	// Handles instructions in the Execute stage
	void Execute(int instruction) {

		switch (instruction) {
		case Load:
			cout << "Execute Stage: { Instruction: Load, ld x1 = x[" << instance << "], Data x1 = " << x1 << ", Clock Cycle Number : " << clockCycleNumber << " }" << endl;
			stall = 1;
			break;
		case Add:
			x3 = x1 + x2;
			cout << "Execute Stage: { Instruction: Add, add.d x3 = x1 + x2, Data x3 = " << x3 << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			stall = 2;
			break;
		case instruction::Store:
			cout << "Execute Stage: { Instruction: Store, sd x[" << instance << "] = x3, Data x[" << instance << "] = " << x1 << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		case Addi:
			cout << "Execute Stage: { Instruction: Addi, addi x[" << instance << "] = x[" << instance << "] - 4, Data x[" << instance << "] = x[" << instance - 4 << "], Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		case BNE:
			branchTF = 1;
			cout << "Execute Stage: { Instruction: BNE, bne x1 - x2 = 0 ? Halt : Loop, Data = " << branchTF << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		default:
			cout << "Execute Stage: { No Operation }" << endl;
			break;
		}

	}

	// Handles instructions in the Store/Write Back stage
	void Store(int instruction) {

		switch (instruction) {
		case Load:
			cout << "Store Stage: { Instruction: Load, ld x1 = x[" << instance << "], Data x1 = " << x1 << ", Clock Cycle Number : " << clockCycleNumber << " }" << endl;
			break;
		case Add:
			cout << "Store Stage: { Instruction: Add, add.d x3 = x1 + x2, Data x3 = " << x3 << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		case instruction::Store:
			memoryArray[instance] = x3;
			cout << "Store Stage: { Instruction: Store, sd x[" << instance << "] = x3, Data x[" << instance << "] = " << x1 << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		case Addi:
			cout << "Store Stage: { Instruction: Addi, addi x[" << instance << "] = x[" << instance << "] - 4, Data x[" << instance << "] = x[" << instance - 4 << "], Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		case BNE:
			cout << "Store Stage: { Instruction: BNE, bne x1 - x2 = 0 ? Halt : Loop, Data = " << branchTF << ", Clock Cycle Number: " << clockCycleNumber << " }" << endl;
			break;
		default:
			cout << "Store Stage: { No Operation }" << endl;
			break;
		}

	}

	Pipeline() {

	}

};

void switchStage();		// Call to the function that will perform pipelining

// List of variable used to perform simulation
vector<int> eventList = { Load, Add, Store, Addi, BNE, Halt};
int currentEventInQueue = 0;
int timesProcessedLoad = 0;
int timesProcessedAdd = 0;
int timesProcessedStore = 0;
int timesProcessedAddi = 0;
int timesProcessedBNE = 0;
int needStall = 0;
Pipeline fourStagePipeline;
int i;

// Main Function
int main() {

	for (i = UPPERLOOPITERATER; i >= 0; i = i - 4) {	// Loop runs 1000 times *Note* Doesn't do all 1000 indexes
														// Increase UPPERLOOPITERATER value to achieve all 1000
		switchStage();									// Decrements by 4
		cout << endl;

	}
 
	return 0;
}

// Function that performs CPU pipelining
void switchStage() {

	needStall = fourStagePipeline.stall;		// Checks if stall was needed

	if (needStall == 1) {					// Output depending on one stall
		fourStagePipeline.Store(Load);
		fourStagePipeline.Execute(Halt);
		fourStagePipeline.Decode(Add);
		fourStagePipeline.Fetch(Store);
		timesProcessedLoad++;
		needStall = 0;
		fourStagePipeline.stall = 0;
		fourStagePipeline.clockCycleNumber++;
		cout << endl;
	}

	if (needStall == 2) {					// Output depending on two stalls

		fourStagePipeline.Store(Add);
		fourStagePipeline.Execute(Halt);
		fourStagePipeline.Execute(Halt);
		fourStagePipeline.Decode(Store);
		fourStagePipeline.Fetch(Addi);
		timesProcessedAdd++;
		fourStagePipeline.clockCycleNumber++;

		cout << endl;
		fourStagePipeline.Execute(Halt);
		fourStagePipeline.Decode(Store);
		fourStagePipeline.Fetch(Addi);
		needStall = 0;
		fourStagePipeline.stall = 0;

		cout << endl;
		fourStagePipeline.clockCycleNumber++;
	}

	if ((eventList.at(Load) == currentEventInQueue || timesProcessedLoad > 0) && needStall == 0) {		// Processing Load Instructions across various stages

		if (timesProcessedLoad == 0) {

			fourStagePipeline.Fetch(Load);
			timesProcessedLoad++;

		} else if (timesProcessedLoad == 1) {

			fourStagePipeline.Decode(Load);
			timesProcessedLoad++;

		} else if (timesProcessedLoad == 2) {

			fourStagePipeline.Execute(Load);
			timesProcessedLoad++;

		} else if (timesProcessedLoad == 3) {

			fourStagePipeline.Store(Load);
			timesProcessedLoad = 0;

		}
		else if (timesProcessedLoad == 4) {

			timesProcessedLoad = 0;

		}
		
	}

	if ((eventList.at(Add) == currentEventInQueue || timesProcessedAdd > 0) && needStall == 0) {		// Processing Add Instructions across various stages

		if (timesProcessedAdd == 0) {

			fourStagePipeline.Fetch(Add);
			timesProcessedAdd++;

		} else if (timesProcessedAdd == 1) {

			fourStagePipeline.Decode(Add);
			timesProcessedAdd++;

		} else if (timesProcessedAdd == 2) {

			fourStagePipeline.Execute(Add);
			timesProcessedAdd++;

		} else if (timesProcessedAdd == 3) {

			fourStagePipeline.Store(Add);
			timesProcessedAdd = 0;
		}
		else if (timesProcessedAdd == 4) {

			timesProcessedAdd = 0;

		}

	}

	if ((eventList.at(Store) == currentEventInQueue || timesProcessedStore > 0) && needStall == 0) {		// Processing Store/Write Back Instructions across various stages
		
		if (timesProcessedStore == 0) {

			fourStagePipeline.Fetch(Store);
			timesProcessedStore++;

		} else if (timesProcessedStore == 1) {

			fourStagePipeline.Decode(Store);
			timesProcessedStore++;

		} else if (timesProcessedStore == 2) {

			fourStagePipeline.Execute(Store);
			timesProcessedStore++;

		} else if (timesProcessedStore == 3) {

			fourStagePipeline.Store(Store);
			timesProcessedStore = 0;
		}

	}

	if ((eventList.at(Addi) == currentEventInQueue || timesProcessedAddi > 0) && needStall == 0) {		// Processing Addi Instructions across various stages
		
		if (timesProcessedAddi == 0) {

			fourStagePipeline.Fetch(Addi);
			timesProcessedAddi++;

		} else if (timesProcessedAddi == 1) {

			fourStagePipeline.Decode(Addi);
			timesProcessedAddi++;

		} else if (timesProcessedAddi == 2) {

			fourStagePipeline.Execute(Addi);
			timesProcessedAddi++;

		} else if (timesProcessedAddi == 3 ) {

			fourStagePipeline.Store(Addi);
			timesProcessedAddi = 0;
		}

	}

	if ((eventList.at(BNE) == currentEventInQueue || timesProcessedBNE > 0) && needStall == 0) {		// Processing BNE Instructions across various stages
		
		if (timesProcessedBNE == 0) {

			fourStagePipeline.Fetch(BNE);
			timesProcessedBNE++;

			if (fourStagePipeline.preload == 1) {		// Preload indicator predicts loop will continue
				fourStagePipeline.instance -= 4;		// Decrements by 4
				fourStagePipeline.preload = 0;
			}

		} else if (timesProcessedBNE == 1) {

			fourStagePipeline.Decode(BNE);
			timesProcessedBNE++;

		} else if (timesProcessedBNE == 2) {

			fourStagePipeline.Execute(BNE);
			timesProcessedBNE++;

		} else if (timesProcessedBNE == 3) {

			fourStagePipeline.Store(BNE);
			timesProcessedBNE = 0;
		}

	}


	if (i == 0) {											// Pipeline will halt if it can't go any further
		currentEventInQueue = Halt;
		cout << endl << "Pipeline has halted!" << endl;
	}
	else {
		
		if (currentEventInQueue < 4) {			// Loops the instruction queue
			currentEventInQueue++;
		}
		else {
			currentEventInQueue = 0;
		}

		fourStagePipeline.clockCycleNumber++;		// Updates the clock cycle number

	}

}