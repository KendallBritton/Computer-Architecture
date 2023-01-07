//***********************************************************************
// ECGR 5181 - Assignment 6 (Part 2) - MESI Cache Coherence Simulation	*		
// (C++ Verision)														*
//																		*
// This program will simulate the two state diagrams of the MESI Cache	*
// Coherence protocol by use of a predetermined set of cache requests.	*
// The predetermined set of cache requests will show each transition	*
// within each state diagram.											*				
// 																		*															
// Kendall Britton, Jaron Price, Sreekanksha Gangadhar		2022-12-05	* 
//***********************************************************************

// Assumptions:
// We are assuming that each write instruction is equivalent to a store instruction
// We are assuming that each read instruction is equivalent to a load instruction
// We are assuming that each cache contains 3 cache lines for simplifaction
// For simulation to work, each cache line request must all be in consecutive order
// 4 Processors each with a Level-1 64 KB cache
// All connected to a shared memory (1 MB) using a system memory bus
// We assume that only one processor will send a request automatically winning access (predetermined)

/*		Write Hit Definition: The cache data is dirty and the cache data needs to be updated (written) but data is already there
*		Write Miss Definition: The cache data is dirty and the cahce data needs to be updated (written) but data is not already there
*		Read Hit Defintion: The cache goes to read data from cache, and data is in cache
*		Read Miss Definition: The cache goes to read data from cache, and data is not in cache
*/

// List of libraries that are required to run the program
#include <iostream>
#include <map>
using namespace std;

// List of the various possible states of each cache
enum class MESICacheState {

	Invalid,
	Modified,
	Exculsive,
	Shared

};

enum source {Memory, P1, P2, P3, P4, processStore};		// Mapping to deteremine where data is obtained from

// Class to hold the properties of each MESI Cache
class MESICache {

	public:
		MESICache();

		MESICacheState currentState;		// Keeps track of current state in MESI state diagram
		MESICacheState previousState;		// Keeps track of previous state in MESI state diagram

	private:

};

// Arrays that will transition the state diagram by sending new request
int readOrWriteRequest[17] = { 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1 };
int cacheLineArray[17] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 3, 3 };
int processorRequestSel[17] = {1, 2, 3, 4, 4, 4, 4, 1, 1, 1, 2, 3, 4, 4, 4, 3, 1};

// Variables necessary to run the code
int iter = 0;							// Iterator that loops through all arrays
int runOnce = 0;						// Variable to reset cache states for new cache line
int memReadFlag = 0;					// Sets memory bus transaction flag
int invalidateFlag = 0;					// Sets invalidate bus transaction flag
int readWithIntToModifyFlag = 0;		// Sets RWITM bus transaction flag
int supplier = -1;						// Keeps track of supplier source
int clockCycle = 0;						// Keeps clock cycle count
int requestNumber = 0;					// Number to keep track of request number

// Call to the functions that will lead to operation of code
void stateTransition();
void printOutput();
void supplierDetermine();
void clockCycleCount();
void busArbiter();

MESICache cache1, cache2, cache3, cache4;		// Allocates the 4 different caches 

// Main Function
int main() {


	printf("Instruction (Inst., $ Line, Processor):    Processor States:    Bus Request:    Data Supplier:    Clock Cycle:\n");
	printf("\n                                               1 2 3 4\n");

	for (iter = 0; iter < 17; iter++) {						// Loops through all the array requests

		//busArbiter();										// Uncomment to make random (not predetermined)

		cache1.previousState = cache1.currentState;			// Updates previous states
		cache2.previousState = cache2.currentState;
		cache3.previousState = cache3.currentState;
		cache4.previousState = cache4.currentState;

		supplierDetermine();								// Determines the supplier of data
		stateTransition();									// Processes the request
		clockCycleCount();									// Calculates the updated clock cycle
		printOutput();										// Prints the total output

	}

	return 0;
}

// Function that updates the state transition
void stateTransition() {

	switch(cacheLineArray[iter]){

		case 1:		// Cache line 1

			if (runOnce == 0) {
				cache1.currentState = MESICacheState::Invalid;
				cache2.currentState = MESICacheState::Invalid;
				cache3.currentState = MESICacheState::Invalid;
				cache4.currentState = MESICacheState::Invalid;
				runOnce++;
			}

			switch (processorRequestSel[iter]) {		// Processor Select

				case 1:		// Processor 1

					switch (readOrWriteRequest[iter]) {

						case 0:								// Read/Load Instruction

							if (cache1.currentState == MESICacheState::Invalid && (cache2.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid)) {

								cache1.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
								memReadFlag = 1;

							}
							else if (cache1.currentState == MESICacheState::Invalid) {

								cache1.currentState = MESICacheState::Shared;				// Local access (Mem Read)
								memReadFlag = 1;

								switch (cache2.currentState) {								// Remote access
									case MESICacheState::Shared:
									case MESICacheState::Exculsive:
									case MESICacheState::Modified:							// Copyback
										cache2.currentState = MESICacheState::Shared;
										break;

									default:
										break;
								}

								switch (cache3.currentState) {								// Remote access
									case MESICacheState::Shared:
									case MESICacheState::Exculsive:
									case MESICacheState::Modified:							// Copy back
										cache3.currentState = MESICacheState::Shared;
										break;

									default:
										break;
								}

								switch (cache4.currentState) {								// Remote access
									case MESICacheState::Shared:
									case MESICacheState::Exculsive:
									case MESICacheState::Modified:							// Copy back
										cache4.currentState = MESICacheState::Shared;
										break;

									default:
										break;
								}

							}
							else if (cache1.currentState == MESICacheState::Shared) {
							
								cache1.currentState = MESICacheState::Shared;				// Local access

							}
							else if (cache1.currentState == MESICacheState::Modified) {

								cache1.currentState = MESICacheState::Modified;				// Local access

							}
							else if (cache1.currentState == MESICacheState::Exculsive) {

								cache1.currentState = MESICacheState::Exculsive;				// Local access

							}

							break;

						case 1:								// Write/Store Instruction

							if (cache1.currentState == MESICacheState::Invalid) {

								cache1.currentState = MESICacheState::Modified;				// Local access (RWITM)
								readWithIntToModifyFlag = 1;

								switch (cache2.currentState) {								// Remote access
									case MESICacheState::Modified:							// Copy back
									case MESICacheState::Exculsive:
										cache2.currentState = MESICacheState::Invalid;
										break;

									default:
										break;
								}

								switch (cache3.currentState) {								// Remote access
									case MESICacheState::Modified:							// Copy back
									case MESICacheState::Exculsive:
										cache3.currentState = MESICacheState::Invalid;
										break;

									default:
										break;
								}

								switch (cache4.currentState) {								// Remote access
									case MESICacheState::Modified:							// Copy back
									case MESICacheState::Exculsive:
										cache4.currentState = MESICacheState::Invalid;
										break;

									default:
										break;
								}

							}
							else if (cache1.currentState == MESICacheState::Shared) {

								cache1.currentState = MESICacheState::Modified;				// Local access (Invalidate)
								invalidateFlag = 1;

								switch (cache2.currentState) {								// Remote access
									case MESICacheState::Shared:							
										cache2.currentState = MESICacheState::Invalid;
										break;

									default:
										break;
								}


								switch (cache3.currentState) {								// Remote access
									case MESICacheState::Shared:
										cache3.currentState = MESICacheState::Invalid;
										break;

									default:
										break;
								}


								switch (cache4.currentState) {								// Remote access
									case MESICacheState::Shared:
										cache4.currentState = MESICacheState::Invalid;
										break;

									default:
										break;
								}


							}
							else if (cache1.currentState == MESICacheState::Modified) {

								cache1.currentState = MESICacheState::Modified;				// Local access

							}
							else if (cache1.currentState == MESICacheState::Exculsive) {

								cache1.currentState = MESICacheState::Modified;				// Local access

							}

							break;

						default:
							break;
					}

					break;

				case 2:		// Processor 2

					switch (readOrWriteRequest[iter]) {

						case 0:								// Read/Load Instruction

							if (cache2.currentState == MESICacheState::Invalid && (cache1.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid)) {

								cache2.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
								memReadFlag = 1;

							}
							else if (cache2.currentState == MESICacheState::Invalid) {

								cache2.currentState = MESICacheState::Shared;				// Local access (Mem Read)
								memReadFlag = 1;

								switch (cache1.currentState) {								// Remote access
								case MESICacheState::Shared:
								case MESICacheState::Exculsive:
								case MESICacheState::Modified:							// Copyback
									cache1.currentState = MESICacheState::Shared;
									break;

								default:
									break;
								}

								switch (cache3.currentState) {								// Remote access
								case MESICacheState::Shared:
								case MESICacheState::Exculsive:
								case MESICacheState::Modified:							// Copy back
									cache3.currentState = MESICacheState::Shared;
									break;

								default:
									break;
								}

								switch (cache4.currentState) {								// Remote access
								case MESICacheState::Shared:
								case MESICacheState::Exculsive:
								case MESICacheState::Modified:							// Copy back
									cache4.currentState = MESICacheState::Shared;
									break;

								default:
									break;
								}

							}
							else if (cache2.currentState == MESICacheState::Shared) {

								cache2.currentState = MESICacheState::Shared;				// Local access

							}
							else if (cache2.currentState == MESICacheState::Modified) {

								cache2.currentState = MESICacheState::Modified;				// Local access

							}
							else if (cache2.currentState == MESICacheState::Exculsive) {

								cache2.currentState = MESICacheState::Exculsive;				// Local access

							}

							break;

						case 1:								// Write/Store Instruction

							if (cache2.currentState == MESICacheState::Invalid) {

								cache2.currentState = MESICacheState::Modified;				// Local access (RWITM)
								readWithIntToModifyFlag = 1;

								switch (cache1.currentState) {								// Remote access
								case MESICacheState::Modified:							// Copy back
								case MESICacheState::Exculsive:
									cache1.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}

								switch (cache3.currentState) {								// Remote access
								case MESICacheState::Modified:							// Copy back
								case MESICacheState::Exculsive:
									cache3.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}

								switch (cache4.currentState) {								// Remote access
								case MESICacheState::Modified:							// Copy back
								case MESICacheState::Exculsive:
									cache4.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}

							}
							else if (cache2.currentState == MESICacheState::Shared) {

								cache2.currentState = MESICacheState::Modified;				// Local access (Invalidate)
								invalidateFlag = 1;

								switch (cache1.currentState) {								// Remote access
								case MESICacheState::Shared:
									cache1.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}


								switch (cache3.currentState) {								// Remote access
								case MESICacheState::Shared:
									cache3.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}


								switch (cache4.currentState) {								// Remote access
								case MESICacheState::Shared:
									cache4.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}


							}
							else if (cache2.currentState == MESICacheState::Modified) {

								cache2.currentState = MESICacheState::Modified;				// Local access

							}
							else if (cache2.currentState == MESICacheState::Exculsive) {

								cache2.currentState = MESICacheState::Modified;				// Local access

							}

							break;

						default:
							break;
					}

					break;

				case 3:		// Processor 3

					switch (readOrWriteRequest[iter]) {

						case 0:								// Read/Load Instruction

							if (cache3.currentState == MESICacheState::Invalid && (cache2.currentState == MESICacheState::Invalid && cache1.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid)) {

								cache3.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
								memReadFlag = 1;

							}
							else if (cache3.currentState == MESICacheState::Invalid) {

								cache3.currentState = MESICacheState::Shared;				// Local access (Mem Read)
								memReadFlag = 1;

								switch (cache2.currentState) {								// Remote access
								case MESICacheState::Shared:
								case MESICacheState::Exculsive:
								case MESICacheState::Modified:							// Copyback
									cache2.currentState = MESICacheState::Shared;
									break;

								default:
									break;
								}

								switch (cache1.currentState) {								// Remote access
								case MESICacheState::Shared:
								case MESICacheState::Exculsive:
								case MESICacheState::Modified:							// Copy back
									cache1.currentState = MESICacheState::Shared;
									break;

								default:
									break;
								}

								switch (cache4.currentState) {								// Remote access
								case MESICacheState::Shared:
								case MESICacheState::Exculsive:
								case MESICacheState::Modified:							// Copy back
									cache4.currentState = MESICacheState::Shared;
									break;

								default:
									break;
								}

							}
							else if (cache3.currentState == MESICacheState::Shared) {

								cache3.currentState = MESICacheState::Shared;				// Local access

							}
							else if (cache3.currentState == MESICacheState::Modified) {

								cache3.currentState = MESICacheState::Modified;				// Local access

							}
							else if (cache3.currentState == MESICacheState::Exculsive) {

								cache3.currentState = MESICacheState::Exculsive;				// Local access

							}

							break;

						case 1:								// Write/Store Instruction

							if (cache3.currentState == MESICacheState::Invalid) {

								cache3.currentState = MESICacheState::Modified;				// Local access (RWITM)
								readWithIntToModifyFlag = 1;

								switch (cache2.currentState) {								// Remote access
								case MESICacheState::Modified:							// Copy back
								case MESICacheState::Exculsive:
									cache2.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}

								switch (cache1.currentState) {								// Remote access
								case MESICacheState::Modified:							// Copy back
								case MESICacheState::Exculsive:
									cache1.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}

								switch (cache4.currentState) {								// Remote access
								case MESICacheState::Modified:							// Copy back
								case MESICacheState::Exculsive:
									cache4.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}

							}
							else if (cache3.currentState == MESICacheState::Shared) {

								cache3.currentState = MESICacheState::Modified;				// Local access (Invalidate)
								invalidateFlag = 1;

								switch (cache2.currentState) {								// Remote access
								case MESICacheState::Shared:
									cache2.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}


								switch (cache1.currentState) {								// Remote access
								case MESICacheState::Shared:
									cache1.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}


								switch (cache4.currentState) {								// Remote access
								case MESICacheState::Shared:
									cache4.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}


							}
							else if (cache3.currentState == MESICacheState::Modified) {

								cache3.currentState = MESICacheState::Modified;				// Local access

							}
							else if (cache3.currentState == MESICacheState::Exculsive) {

								cache3.currentState = MESICacheState::Modified;				// Local access

							}

							break;

						default:
							break;
					}

					break;

				case 4:		// Processor 4

					switch (readOrWriteRequest[iter]) {

						case 0:								// Read/Load Instruction

							if (cache4.currentState == MESICacheState::Invalid && (cache2.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache1.currentState == MESICacheState::Invalid)) {

								cache4.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
								memReadFlag = 1;

							}
							else if (cache4.currentState == MESICacheState::Invalid) {

								cache4.currentState = MESICacheState::Shared;				// Local access (Mem Read)
								memReadFlag = 1;

								switch (cache2.currentState) {								// Remote access
								case MESICacheState::Shared:
								case MESICacheState::Exculsive:
								case MESICacheState::Modified:							// Copyback
									cache2.currentState = MESICacheState::Shared;
									break;

								default:
									break;
								}

								switch (cache3.currentState) {								// Remote access
								case MESICacheState::Shared:
								case MESICacheState::Exculsive:
								case MESICacheState::Modified:							// Copy back
									cache3.currentState = MESICacheState::Shared;
									break;

								default:
									break;
								}

								switch (cache1.currentState) {								// Remote access
								case MESICacheState::Shared:
								case MESICacheState::Exculsive:
								case MESICacheState::Modified:							// Copy back
									cache1.currentState = MESICacheState::Shared;
									break;

								default:
									break;
								}

							}
							else if (cache4.currentState == MESICacheState::Shared) {

								cache4.currentState = MESICacheState::Shared;				// Local access

							}
							else if (cache4.currentState == MESICacheState::Modified) {

								cache4.currentState = MESICacheState::Modified;				// Local access

							}
							else if (cache4.currentState == MESICacheState::Exculsive) {

								cache4.currentState = MESICacheState::Exculsive;				// Local access

							}

							break;

						case 1:								// Write/Store Instruction

							if (cache4.currentState == MESICacheState::Invalid) {

								cache4.currentState = MESICacheState::Modified;				// Local access (RWITM)
								readWithIntToModifyFlag = 1;

								switch (cache2.currentState) {								// Remote access
								case MESICacheState::Modified:							// Copy back
								case MESICacheState::Exculsive:
									cache2.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}

								switch (cache3.currentState) {								// Remote access
								case MESICacheState::Modified:							// Copy back
								case MESICacheState::Exculsive:
									cache3.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}

								switch (cache1.currentState) {								// Remote access
								case MESICacheState::Modified:							// Copy back
								case MESICacheState::Exculsive:
									cache1.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}

							}
							else if (cache4.currentState == MESICacheState::Shared) {

								cache4.currentState = MESICacheState::Modified;				// Local access (Invalidate)
								invalidateFlag = 1;

								switch (cache2.currentState) {								// Remote access
								case MESICacheState::Shared:
									cache2.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}


								switch (cache3.currentState) {								// Remote access
								case MESICacheState::Shared:
									cache3.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}


								switch (cache1.currentState) {								// Remote access
								case MESICacheState::Shared:
									cache1.currentState = MESICacheState::Invalid;
									break;

								default:
									break;
								}


							}
							else if (cache4.currentState == MESICacheState::Modified) {

								cache4.currentState = MESICacheState::Modified;				// Local access

							}
							else if (cache4.currentState == MESICacheState::Exculsive) {

								cache4.currentState = MESICacheState::Modified;				// Local access

							}

							break;

						default:
							break;
					}

					break;


				default:
					break;
			}

			break;

		case 2:		// Cache line 2

			if (runOnce == 1) {
				cache1.currentState = MESICacheState::Invalid;
				cache2.currentState = MESICacheState::Invalid;
				cache3.currentState = MESICacheState::Invalid;
				cache4.currentState = MESICacheState::Invalid;
				runOnce++;
			}

			switch (processorRequestSel[iter]) {		// Processor Select

				case 1:		// Processor 1

					switch (readOrWriteRequest[iter]) {

					case 0:								// Read/Load Instruction

						if (cache1.currentState == MESICacheState::Invalid && (cache2.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid)) {

							cache1.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
							memReadFlag = 1;

						}
						else if (cache1.currentState == MESICacheState::Invalid) {

							cache1.currentState = MESICacheState::Shared;				// Local access (Mem Read)
							memReadFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copyback
								cache2.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache3.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache4.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

						}
						else if (cache1.currentState == MESICacheState::Shared) {

							cache1.currentState = MESICacheState::Shared;				// Local access

						}
						else if (cache1.currentState == MESICacheState::Modified) {

							cache1.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache1.currentState == MESICacheState::Exculsive) {

							cache1.currentState = MESICacheState::Exculsive;				// Local access

						}

						break;

					case 1:								// Write/Store Instruction

						if (cache1.currentState == MESICacheState::Invalid) {

							cache1.currentState = MESICacheState::Modified;				// Local access (RWITM)
							readWithIntToModifyFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

						}
						else if (cache1.currentState == MESICacheState::Shared) {

							cache1.currentState = MESICacheState::Modified;				// Local access (Invalidate)
							invalidateFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


						}
						else if (cache1.currentState == MESICacheState::Modified) {

							cache1.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache1.currentState == MESICacheState::Exculsive) {

							cache1.currentState = MESICacheState::Modified;				// Local access

						}

						break;

					default:
						break;
					}

					break;

				case 2:		// Processor 2

					switch (readOrWriteRequest[iter]) {

					case 0:								// Read/Load Instruction

						if (cache2.currentState == MESICacheState::Invalid && (cache1.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid)) {

							cache2.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
							memReadFlag = 1;

						}
						else if (cache2.currentState == MESICacheState::Invalid) {

							cache2.currentState = MESICacheState::Shared;				// Local access (Mem Read)
							memReadFlag = 1;

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copyback
								cache1.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache3.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache4.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

						}
						else if (cache2.currentState == MESICacheState::Shared) {

							cache2.currentState = MESICacheState::Shared;				// Local access

						}
						else if (cache2.currentState == MESICacheState::Modified) {

							cache2.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache2.currentState == MESICacheState::Exculsive) {

							cache2.currentState = MESICacheState::Exculsive;				// Local access

						}

						break;

					case 1:								// Write/Store Instruction

						if (cache2.currentState == MESICacheState::Invalid) {

							cache2.currentState = MESICacheState::Modified;				// Local access (RWITM)
							readWithIntToModifyFlag = 1;

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

						}
						else if (cache2.currentState == MESICacheState::Shared) {

							cache2.currentState = MESICacheState::Modified;				// Local access (Invalidate)
							invalidateFlag = 1;

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


						}
						else if (cache2.currentState == MESICacheState::Modified) {

							cache2.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache2.currentState == MESICacheState::Exculsive) {

							cache2.currentState = MESICacheState::Modified;				// Local access

						}

						break;

					default:
						break;
					}

					break;

				case 3:		// Processor 3

					switch (readOrWriteRequest[iter]) {

					case 0:								// Read/Load Instruction

						if (cache3.currentState == MESICacheState::Invalid && (cache2.currentState == MESICacheState::Invalid && cache1.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid)) {

							cache3.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
							memReadFlag = 1;

						}
						else if (cache3.currentState == MESICacheState::Invalid) {

							cache3.currentState = MESICacheState::Shared;				// Local access (Mem Read)
							memReadFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copyback
								cache2.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache1.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache4.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

						}
						else if (cache3.currentState == MESICacheState::Shared) {

							cache3.currentState = MESICacheState::Shared;				// Local access

						}
						else if (cache3.currentState == MESICacheState::Modified) {

							cache3.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache3.currentState == MESICacheState::Exculsive) {

							cache3.currentState = MESICacheState::Exculsive;				// Local access

						}

						break;

					case 1:								// Write/Store Instruction

						if (cache3.currentState == MESICacheState::Invalid) {

							cache3.currentState = MESICacheState::Modified;				// Local access (RWITM)
							readWithIntToModifyFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

						}
						else if (cache3.currentState == MESICacheState::Shared) {

							cache3.currentState = MESICacheState::Modified;				// Local access (Invalidate)
							invalidateFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


						}
						else if (cache3.currentState == MESICacheState::Modified) {

							cache3.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache3.currentState == MESICacheState::Exculsive) {

							cache3.currentState = MESICacheState::Modified;				// Local access

						}

						break;

					default:
						break;
					}

					break;

				case 4:		// Processor 4

					switch (readOrWriteRequest[iter]) {

					case 0:								// Read/Load Instruction

						if (cache4.currentState == MESICacheState::Invalid && (cache2.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache1.currentState == MESICacheState::Invalid)) {

							cache4.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
							memReadFlag = 1;

						}
						else if (cache4.currentState == MESICacheState::Invalid) {

							cache4.currentState = MESICacheState::Shared;				// Local access (Mem Read)
							memReadFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copyback
								cache2.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache3.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache1.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

						}
						else if (cache4.currentState == MESICacheState::Shared) {

							cache4.currentState = MESICacheState::Shared;				// Local access

						}
						else if (cache4.currentState == MESICacheState::Modified) {

							cache4.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache4.currentState == MESICacheState::Exculsive) {

							cache4.currentState = MESICacheState::Exculsive;				// Local access

						}

						break;

					case 1:								// Write/Store Instruction

						if (cache4.currentState == MESICacheState::Invalid) {

							cache4.currentState = MESICacheState::Modified;				// Local access (RWITM)
							readWithIntToModifyFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

						}
						else if (cache4.currentState == MESICacheState::Shared) {

							cache4.currentState = MESICacheState::Modified;				// Local access (Invalidate)
							invalidateFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


						}
						else if (cache4.currentState == MESICacheState::Modified) {

							cache4.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache4.currentState == MESICacheState::Exculsive) {

							cache4.currentState = MESICacheState::Modified;				// Local access

						}

						break;

					default:
						break;
					}

					break;


				default:
					break;
			}

			break;

		case 3:		// Cache line 3

			if (runOnce == 2) {
				cache1.currentState = MESICacheState::Invalid;
				cache2.currentState = MESICacheState::Invalid;
				cache3.currentState = MESICacheState::Invalid;
				cache4.currentState = MESICacheState::Invalid;
				runOnce++;
			}

			switch (processorRequestSel[iter]) {		// Processor Select

				case 1:		// Processor 1

					switch (readOrWriteRequest[iter]) {

					case 0:								// Read/Load Instruction

						if (cache1.currentState == MESICacheState::Invalid && (cache2.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid)) {

							cache1.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
							memReadFlag = 1;

						}
						else if (cache1.currentState == MESICacheState::Invalid) {

							cache1.currentState = MESICacheState::Shared;				// Local access (Mem Read)
							memReadFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copyback
								cache2.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache3.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache4.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

						}
						else if (cache1.currentState == MESICacheState::Shared) {

							cache1.currentState = MESICacheState::Shared;				// Local access

						}
						else if (cache1.currentState == MESICacheState::Modified) {

							cache1.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache1.currentState == MESICacheState::Exculsive) {

							cache1.currentState = MESICacheState::Exculsive;				// Local access

						}

						break;

					case 1:								// Write/Store Instruction

						if (cache1.currentState == MESICacheState::Invalid) {

							cache1.currentState = MESICacheState::Modified;				// Local access (RWITM)
							readWithIntToModifyFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

						}
						else if (cache1.currentState == MESICacheState::Shared) {

							cache1.currentState = MESICacheState::Modified;				// Local access (Invalidate)
							invalidateFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


						}
						else if (cache1.currentState == MESICacheState::Modified) {

							cache1.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache1.currentState == MESICacheState::Exculsive) {

							cache1.currentState = MESICacheState::Modified;				// Local access

						}

						break;

					default:
						break;
					}

					break;

				case 2:		// Processor 2

					switch (readOrWriteRequest[iter]) {

					case 0:								// Read/Load Instruction

						if (cache2.currentState == MESICacheState::Invalid && (cache1.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid)) {

							cache2.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
							memReadFlag = 1;

						}
						else if (cache2.currentState == MESICacheState::Invalid) {

							cache2.currentState = MESICacheState::Shared;				// Local access (Mem Read)
							memReadFlag = 1;

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copyback
								cache1.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache3.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache4.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

						}
						else if (cache2.currentState == MESICacheState::Shared) {

							cache2.currentState = MESICacheState::Shared;				// Local access

						}
						else if (cache2.currentState == MESICacheState::Modified) {

							cache2.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache2.currentState == MESICacheState::Exculsive) {

							cache2.currentState = MESICacheState::Exculsive;				// Local access

						}

						break;

					case 1:								// Write/Store Instruction

						if (cache2.currentState == MESICacheState::Invalid) {

							cache2.currentState = MESICacheState::Modified;				// Local access (RWITM)
							readWithIntToModifyFlag = 1;

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

						}
						else if (cache2.currentState == MESICacheState::Shared) {

							cache2.currentState = MESICacheState::Modified;				// Local access (Invalidate)
							invalidateFlag = 1;

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


						}
						else if (cache2.currentState == MESICacheState::Modified) {

							cache2.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache2.currentState == MESICacheState::Exculsive) {

							cache2.currentState = MESICacheState::Modified;				// Local access

						}

						break;

					default:
						break;
					}

					break;

				case 3:		// Processor 3

					switch (readOrWriteRequest[iter]) {

					case 0:								// Read/Load Instruction

						if (cache3.currentState == MESICacheState::Invalid && (cache2.currentState == MESICacheState::Invalid && cache1.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid)) {

							cache3.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
							memReadFlag = 1;

						}
						else if (cache3.currentState == MESICacheState::Invalid) {

							cache3.currentState = MESICacheState::Shared;				// Local access (Mem Read)
							memReadFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copyback
								cache2.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache1.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache4.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

						}
						else if (cache3.currentState == MESICacheState::Shared) {

							cache3.currentState = MESICacheState::Shared;				// Local access

						}
						else if (cache3.currentState == MESICacheState::Modified) {

							cache3.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache3.currentState == MESICacheState::Exculsive) {

							cache3.currentState = MESICacheState::Exculsive;				// Local access

						}

						break;

					case 1:								// Write/Store Instruction

						if (cache3.currentState == MESICacheState::Invalid) {

							cache3.currentState = MESICacheState::Modified;				// Local access (RWITM)
							readWithIntToModifyFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

						}
						else if (cache3.currentState == MESICacheState::Shared) {

							cache3.currentState = MESICacheState::Modified;				// Local access (Invalidate)
							invalidateFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache4.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache4.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


						}
						else if (cache3.currentState == MESICacheState::Modified) {

							cache3.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache3.currentState == MESICacheState::Exculsive) {

							cache3.currentState = MESICacheState::Modified;				// Local access

						}

						break;

					default:
						break;
					}

					break;

				case 4:		// Processor 4

					switch (readOrWriteRequest[iter]) {

					case 0:								// Read/Load Instruction

						if (cache4.currentState == MESICacheState::Invalid && (cache2.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache1.currentState == MESICacheState::Invalid)) {

							cache4.currentState = MESICacheState::Exculsive;				// Local access (Mem Read)
							memReadFlag = 1;

						}
						else if (cache4.currentState == MESICacheState::Invalid) {

							cache4.currentState = MESICacheState::Shared;				// Local access (Mem Read)
							memReadFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copyback
								cache2.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache3.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
							case MESICacheState::Exculsive:
							case MESICacheState::Modified:							// Copy back
								cache1.currentState = MESICacheState::Shared;
								break;

							default:
								break;
							}

						}
						else if (cache4.currentState == MESICacheState::Shared) {

							cache4.currentState = MESICacheState::Shared;				// Local access

						}
						else if (cache4.currentState == MESICacheState::Modified) {

							cache4.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache4.currentState == MESICacheState::Exculsive) {

							cache4.currentState = MESICacheState::Exculsive;				// Local access

						}

						break;

					case 1:								// Write/Store Instruction

						if (cache4.currentState == MESICacheState::Invalid) {

							cache4.currentState = MESICacheState::Modified;				// Local access (RWITM)
							readWithIntToModifyFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Modified:							// Copy back
							case MESICacheState::Exculsive:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}

						}
						else if (cache4.currentState == MESICacheState::Shared) {

							cache4.currentState = MESICacheState::Modified;				// Local access (Invalidate)
							invalidateFlag = 1;

							switch (cache2.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache2.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache3.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache3.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


							switch (cache1.currentState) {								// Remote access
							case MESICacheState::Shared:
								cache1.currentState = MESICacheState::Invalid;
								break;

							default:
								break;
							}


						}
						else if (cache4.currentState == MESICacheState::Modified) {

							cache4.currentState = MESICacheState::Modified;				// Local access

						}
						else if (cache4.currentState == MESICacheState::Exculsive) {

							cache4.currentState = MESICacheState::Modified;				// Local access

						}

						break;

					default:
						break;
					}

					break;


				default:
					break;
			}

			break;

		default:
			break;

	}
	
}

// Function to initialize class variable
MESICache::MESICache() {

	currentState = MESICacheState::Invalid;

}

// Function to print the proper output format to terminal
void printOutput() {

	int fixPrint = 0;		// Variable to fix print spacing

	if (readOrWriteRequest[iter] == 0) {			// Prints Load/Store of Instruction
		printf("    Load, ");
	}
	else if (readOrWriteRequest[iter] == 1) {
		printf("   Store, ");
	}

	if (cacheLineArray[iter] == 1) {				// Prints selected cache line of array
		printf("Cache Line 1, ");
	}
	else if (cacheLineArray[iter] == 2) {
		printf("Cache Line 2, ");
	}
	else if (cacheLineArray[iter] == 3) {
		printf("Cache Line 3, ");
	}

	if (processorRequestSel[iter] == 1) {			// Prints selected processor
		printf("Processor 1");
	}
	else if (processorRequestSel[iter] == 2) {
		printf("Processor 2");
	}
	else if (processorRequestSel[iter] == 3) {
		printf("Processor 3");
	}
	else if (processorRequestSel[iter] == 4) {
		printf("Processor 4");
	}

	if (cache1.currentState == MESICacheState::Invalid) {			// Print cache 1 state

		printf("            I");

	} else if (cache1.currentState == MESICacheState::Shared) {

		printf("            S");

	}
	else if (cache1.currentState == MESICacheState::Modified) {

		printf("            M");

	}
	else if (cache1.currentState == MESICacheState::Exculsive) {

		printf("            E");

	}


	if (cache2.currentState == MESICacheState::Invalid) {			// Print cache 2 state

		printf(" I");

	}
	else if (cache2.currentState == MESICacheState::Shared) {

		printf(" S");

	}
	else if (cache2.currentState == MESICacheState::Modified) {

		printf(" M");

	}
	else if (cache2.currentState == MESICacheState::Exculsive) {

		printf(" E");

	}

	if (cache3.currentState == MESICacheState::Invalid) {			// Print cache 3 state

		printf(" I");

	}
	else if (cache3.currentState == MESICacheState::Shared) {

		printf(" S");

	}
	else if (cache3.currentState == MESICacheState::Modified) {

		printf(" M");

	}
	else if (cache3.currentState == MESICacheState::Exculsive) {

		printf(" E");

	}

	if (cache4.currentState == MESICacheState::Invalid) {			// Print cache 4 state

		printf(" I");

	}
	else if (cache4.currentState == MESICacheState::Shared) {

		printf(" S");

	}
	else if (cache4.currentState == MESICacheState::Modified) {

		printf(" M");

	}
	else if (cache4.currentState == MESICacheState::Exculsive) {

		printf(" E");

	}

	if (readOrWriteRequest[iter] == 0) {				// Print load or store request

		printf("           Bus Load");

	}
	else if (readOrWriteRequest[iter] == 1) {

		printf("          Bus Store");

	}

	switch (supplier) {									// Prints the supplier source

		case 0:
			fixPrint = 0;
			printf("          Memory");
			break;

		case 1:
			fixPrint = 1;
			printf("            P1");
			break;

		case 2:
			fixPrint = 1;
			printf("            P2");
			break;

		case 3:
			fixPrint = 1;
			printf("            P3");
			break;

		case 4:
			fixPrint = 1;
			printf("            P4");
			break;

		case 5:
			fixPrint = 0;
			printf("        Proc. Store");
			break;
	}

	if (fixPrint == 0) {							// Prints the current clock cycle
		printf("\t\t%d", clockCycle);
	}
	else if (fixPrint == 1) {
		printf("\t\t\t%d", clockCycle);
	}
	

	printf("\n");

}

// Function to determine the supplier source of the data
void supplierDetermine() {

	int temp = -1;

	if (cacheLineArray[iter] != cacheLineArray[iter - 1]) {		// If its a new cache line, reset states
		cache1.currentState = MESICacheState::Invalid;
		cache2.currentState = MESICacheState::Invalid;
		cache3.currentState = MESICacheState::Invalid;
		cache4.currentState = MESICacheState::Invalid;
	}

	if (readOrWriteRequest[iter] == 0) {		// Parameters for load requests

		if (cache1.currentState == MESICacheState::Invalid && cache2.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid) {

			supplier = source::Memory;

		}
		else if (cache1.currentState != MESICacheState::Invalid && cache2.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid) {

			supplier = source::P1;

		}
		else if (cache1.currentState == MESICacheState::Invalid && cache2.currentState != MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid) {

			supplier = source::P2;

		}
		else if (cache1.currentState == MESICacheState::Invalid && cache2.currentState == MESICacheState::Invalid && cache3.currentState != MESICacheState::Invalid && cache4.currentState == MESICacheState::Invalid) {

			supplier = source::P3;

		}
		else if (cache1.currentState == MESICacheState::Invalid && cache2.currentState == MESICacheState::Invalid && cache3.currentState == MESICacheState::Invalid && cache4.currentState != MESICacheState::Invalid) {

			supplier = source::P4;

		}
		else if (readOrWriteRequest[iter - 1] == 1){
			
			temp = processorRequestSel[iter - 1];	// Retrieves previous processor processed
			
			switch (temp) {
				case 1:
					supplier = source::P1;
					break;

				case 2:
					supplier = source::P2;
					break;

				case 3:
					supplier = source::P3;
					break;

				case 4:
					supplier = source::P4;
					break;

				default:
					break;
			}
			
		}

	}
	else if (readOrWriteRequest[iter] == 1) {		// Parameters for store request

		supplier = source::processStore;

	}

}

// Function to determine the updated clock cycle amount
void clockCycleCount(){

	if (supplier == source::Memory) {

		clockCycle += 5;

	}
	else if (readOrWriteRequest[iter] == 0 && (cache1.currentState == MESICacheState::Modified || cache2.currentState == MESICacheState::Modified || cache3.currentState == MESICacheState::Modified || cache4.currentState == MESICacheState::Modified)) {

		clockCycle += 1;

	}
	else if (readOrWriteRequest[iter] == 1 && (cache1.currentState == MESICacheState::Modified || cache2.currentState == MESICacheState::Modified || cache3.currentState == MESICacheState::Modified || cache4.currentState == MESICacheState::Modified)) {

		clockCycle += 4;

	}
	else if (readOrWriteRequest[iter] == 0 && (cache1.currentState == MESICacheState::Exculsive || cache2.currentState == MESICacheState::Exculsive || cache3.currentState == MESICacheState::Exculsive || cache4.currentState == MESICacheState::Exculsive)) {

		clockCycle += 1;

	}
	else if (readOrWriteRequest[iter] == 0 && (cache1.currentState == MESICacheState::Shared) && supplier == source::P1 && processorRequestSel[iter] == 1) {

		clockCycle += 1;

	}
	else if (readOrWriteRequest[iter] == 0 && (cache1.currentState == MESICacheState::Shared) && supplier != source::P1 && processorRequestSel[iter] == 1) {

		if (cache1.previousState == MESICacheState::Shared) {

			clockCycle += 1;

		}
		else if (cache1.previousState != MESICacheState::Shared) {

			clockCycle += 2;

		}

	}
	else if (readOrWriteRequest[iter] == 0 && (cache2.currentState == MESICacheState::Shared) && supplier == source::P2 && processorRequestSel[iter] == 2) {

		clockCycle += 1;

	}
	else if (readOrWriteRequest[iter] == 0 && (cache2.currentState == MESICacheState::Shared) && supplier != source::P2 && processorRequestSel[iter] == 2) {

		if (cache2.previousState == MESICacheState::Shared) {

			clockCycle += 1;

		}
		else if (cache2.previousState != MESICacheState::Shared) {

			clockCycle += 2;

		}

	}
	else if (readOrWriteRequest[iter] == 0 && (cache3.currentState == MESICacheState::Shared) && supplier == source::P3 && processorRequestSel[iter] == 3) {

		clockCycle += 1;

	}
	else if (readOrWriteRequest[iter] == 0 && (cache3.currentState == MESICacheState::Shared) && supplier != source::P3 && processorRequestSel[iter] == 3) {

		if (cache3.previousState == MESICacheState::Shared) {

			clockCycle += 1;

		}
		else if (cache3.previousState != MESICacheState::Shared) {

			clockCycle += 2;

		}

	}
	else if (readOrWriteRequest[iter] == 0 && (cache4.currentState == MESICacheState::Shared) && supplier == source::P4 && processorRequestSel[iter] == 4) {

		clockCycle += 1;

	}
	else if (readOrWriteRequest[iter] == 0 && (cache4.currentState == MESICacheState::Shared) && supplier != source::P4 && processorRequestSel[iter] == 4) {

		if (cache4.previousState == MESICacheState::Shared) {

			clockCycle += 1;

		}
		else if (cache4.previousState != MESICacheState::Shared) {

			clockCycle += 2;

		}

	}

}

// Function to determine which processor wins access request (if not predetermined)
void busArbiter() {

	readOrWriteRequest[requestNumber] = rand() % 2;				// Determines whether load or store request

	if (requestNumber <= 11) {									// Determines cache line to check (3 total line sizes) 

		cacheLineArray[requestNumber] =  1;			 

	}
	else if (requestNumber >= 12 && requestNumber <= 14) {

		cacheLineArray[requestNumber] = 2;

	}
	else if (requestNumber > 14) {

		cacheLineArray[requestNumber] = 3;

	}

	processorRequestSel[requestNumber] = (rand() % 4) + 1;		// Determines which processor made request and is granted access

	requestNumber++;

}