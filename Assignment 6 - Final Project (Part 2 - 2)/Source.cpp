//***********************************************************************
// ECGR 5181 - Assignment 6 (Part 2 - 2) - Directory Based Cache		*		
// Cache Coherence Simulation (C++ Verision)							*
//																		*
// This program will simulate the two state diagrams of the Directory 	*
// Based Cache Coherence protocol by use of a predetermined set of		*
// cache requests. The predetermined set of cache requests will show 	*
// each transition within each state diagram.							*				
// 																		*															
// Kendall Britton, Jaron Price, Sreekanksha Gangadhar		2022-12-05	* 
//***********************************************************************

/* Assumptions:
*  4 processors each with a level-1 64 KB cache
*  Each node contains a 256 KB local memory bank
*  Each node contains a local directory of 1024 Kb size **(256 KB x 1024 bytes x 4 memory banks) = 1 Mb = 1024 Kb
*  Each node has access to each node through point to point network (vector)
*  Each node will make a request on the variable X
*  We assume that only one processor will send a request automatically winning acess (predetermined)
*  We are not simulating cache lines for simplification of simulation, the focus is on state diagram transition
*/

/*		Write Hit Definition: The cache data is dirty and the cache data needs to be updated (written) but data is already there
*		Write Miss Definition: The cache data is dirty and the cahce data needs to be updated (written) but data is not already there
*		Read Hit Defintion: The cache goes to read data from cache, and data is in cache
*		Read Miss Definition: The cache goes to read data from cache, and data is not in cache
*/


// List of libraries that are required to run the program
#include <iostream>
#include <vector>
#include <map>
using namespace std;

// List of various possible states of each cache
enum class DirectoryBasedCacheState {

	Invalid,
	Modified,
	Shared,

};

// List of various possible states of each directory
enum class DirectoryBasedDirectoryState {

	Shared,
	Uncached,
	Exclusive

};

// Class to hold the properties of each Directory Based Cache Node
class DirectoryBasedCacheNode {

public:
	DirectoryBasedCacheNode();

	int memoryBank[256];			// Memory bank in KB
	int directoryBank[1024];		// Directory bank in KB ** (256 KB x 1024 bytes x 4 memory banks) = 1 Mb = 1024 Kb

	DirectoryBasedCacheState currentStateCache;		// Keeps track of current state in MESI state diagram
	DirectoryBasedCacheState previousStateCache;		// Keeps track of previous state in MESI state diagram

	DirectoryBasedDirectoryState currentStateDirectory;		// Keeps track of current state in MESI state diagram
	DirectoryBasedDirectoryState previousStateDirectory;		// Keeps track of previous state in MESI state diagram

	int cpuReadMiss = -1;			// Variable to determine whether requests were a hit or a miss
	int cpuReadHit = -1;
	int cpuWriteMiss = -1;
	int cpuWriteHit = -1;

private:

	int nodeID = -1;		// Identification number of the node

};

// Variables necessary to run the code
int iter = 0;							// Iterator that loops through all arrays
vector<int> pointToPointNetwork;		// Point to point network of nodes 
int nodeNumber = 1;						// Starting number for first node entered in network
int invalidateFlag = 0;					// Sets invalidate flag
int dataWriteBackFlag = 0;				// Sets data write back flag
int fetchFlag = 0;						// Sets the fetch flag
int dataValueReply = 0;					// Sets the data value reply flag
int supplier = -1;						// Keeps track of supplier source
int clockCycle = 0;						// Keeps clock cycle count

DirectoryBasedCacheNode node1, node2, node3, node4;			// Allocates the 4 different nodes

enum source { Memory, N1, N2, N3, N4, processStore };		// Mapping to deteremine where data is obtained from


// Arrays that will transition the state diagram by sending new request
int hitOrMissOnRequest[12] = {0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0};
int readOrWriteRequest[12] = { 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1};
int processorRequestSel[12] = { 1, 1, 2, 2, 2, 2, 2, 4, 4, 4, 3, 3 };

// Call to the functions that will lead to operation of code
void stateTransitionCache();
void stateTransitionDirectory();
void printOutput();
void supplierDetermine();
void clockCycleCount();

// Main Function
int main() {

	printf("Instruction (Inst., Processor):  Cache Hit/Miss:  Processor States:  Directory States:    Data Supplier:    Clock Cycle:\n");
	printf("\n                                                      1 2 3 4            U U U U\n");


	for (iter = 0; iter < 12; iter++) {									// Loops through all the array requests
			

		node1.previousStateCache = node1.currentStateCache;				// Updates previous states
		node1.previousStateDirectory = node1.currentStateDirectory;

		node2.previousStateCache = node2.currentStateCache;
		node2.previousStateDirectory = node2.currentStateDirectory;

		node3.previousStateCache = node3.currentStateCache;
		node3.previousStateDirectory = node3.currentStateDirectory;

		node4.previousStateCache = node4.currentStateCache;
		node4.previousStateDirectory = node4.currentStateDirectory;

		// Assigns each hit or miss on request
		switch (readOrWriteRequest[iter]) {	
			case 0:
				switch (processorRequestSel[iter]) {

					case 1:

						if (hitOrMissOnRequest[iter] == 0) {

							node1.cpuReadMiss = hitOrMissOnRequest[iter];
							node1.cpuReadHit = 0;

						}
						else if (hitOrMissOnRequest[iter] == 1) {

							node1.cpuReadHit = hitOrMissOnRequest[iter];
							node1.cpuReadMiss = 0;
						}

						break;

					case 2:

						if (hitOrMissOnRequest[iter] == 0) {

							node2.cpuReadMiss = hitOrMissOnRequest[iter];
							node2.cpuReadHit = -1;

						}
						else if (hitOrMissOnRequest[iter] == 1) {

							node2.cpuReadHit = hitOrMissOnRequest[iter];
							node2.cpuReadMiss = -1;

						}

						break;

					case 3:

						if (hitOrMissOnRequest[iter] == 0) {

							node3.cpuReadMiss = hitOrMissOnRequest[iter];
							node3.cpuReadHit = -1;

						}
						else if (hitOrMissOnRequest[iter] == 1) {

							node3.cpuReadHit = hitOrMissOnRequest[iter];
							node3.cpuReadMiss = -1;

						}

						break;

					case 4:

						if (hitOrMissOnRequest[iter] == 0) {

							node4.cpuReadMiss = hitOrMissOnRequest[iter];
							node4.cpuReadHit = -1;

						}
						else if (hitOrMissOnRequest[iter] == 1) {

							node4.cpuReadHit = hitOrMissOnRequest[iter];
							node4.cpuReadMiss = -1;

						}

						break;

					default:
						break;


				}
				break;

			case 1:

				switch (processorRequestSel[iter]) {

				case 1:

					if (hitOrMissOnRequest[iter] == 0) {

						node1.cpuWriteMiss = hitOrMissOnRequest[iter];
						node1.cpuWriteHit = -1;

					}
					else if (hitOrMissOnRequest[iter] == 1) {

						node1.cpuWriteHit = hitOrMissOnRequest[iter];
						node1.cpuWriteMiss = -1;
					}

					break;

				case 2:

					if (hitOrMissOnRequest[iter] == 0) {

						node2.cpuWriteMiss = hitOrMissOnRequest[iter];
						node2.cpuWriteHit = -1;

					}
					else if (hitOrMissOnRequest[iter] == 1) {

						node2.cpuWriteHit = hitOrMissOnRequest[iter];
						node2.cpuWriteMiss = -1;

					}

					break;

				case 3:

					if (hitOrMissOnRequest[iter] == 0) {

						node3.cpuWriteMiss = hitOrMissOnRequest[iter];
						node3.cpuWriteHit = -1;

					}
					else if (hitOrMissOnRequest[iter] == 1) {

						node3.cpuWriteHit = hitOrMissOnRequest[iter];
						node3.cpuWriteMiss = -1;

					}

					break;

				case 4:

					if (hitOrMissOnRequest[iter] == 0) {

						node4.cpuWriteMiss = hitOrMissOnRequest[iter];
						node4.cpuWriteHit = -1;

					}
					else if (hitOrMissOnRequest[iter] == 1) {

						node4.cpuWriteHit = hitOrMissOnRequest[iter];
						node4.cpuWriteMiss = -1;

					}

					break;

				default:
					break;


				}

			default:
				break;
		}		

		supplierDetermine();			// Determines the supplier of data
		clockCycleCount();				// Calculates the updated clock cycle
		stateTransitionCache();			// Processes the request
		printOutput();					// Prints the total output

		invalidateFlag = 0;				// Resets the flags
		dataWriteBackFlag = 0;
		fetchFlag = 0;
		dataValueReply = 0;

	}


	return 0;
}

// Function to initialize class variable
DirectoryBasedCacheNode::DirectoryBasedCacheNode() {
			
	currentStateCache = DirectoryBasedCacheState::Invalid;					// Initializes cache state as invalid
	currentStateDirectory = DirectoryBasedDirectoryState::Uncached;			// Initializes directory state as uncached

	nodeID = nodeNumber;						// Assigns node number to current node established
	nodeNumber++;								// Increments node number for next node

	pointToPointNetwork.push_back(nodeID);		// Adds the node to the network

}

// Function that updates the state transition for caches
void stateTransitionCache() {

	switch (processorRequestSel[iter]) {				// Processor Select

		case 1:			// Processor 1

			switch (readOrWriteRequest[iter]) {			// Read/Write Instruction Select

				case 0:							// Read/Load Instruction

					if (node1.currentStateCache == DirectoryBasedCacheState::Invalid) {

						node1.currentStateCache = DirectoryBasedCacheState::Shared;

						stateTransitionDirectory();

					}
					else if (node1.currentStateCache == DirectoryBasedCacheState::Shared) {

						node1.currentStateCache = DirectoryBasedCacheState::Shared;

						stateTransitionDirectory();

					}
					else if (node1.currentStateCache == DirectoryBasedCacheState::Modified) {

						switch (node1.cpuReadHit) {
							case 0:
								break;

							case 1:
								node1.currentStateCache = DirectoryBasedCacheState::Modified;
								break;

							default:
								break;
						}

						switch (node1.cpuReadMiss) {
							case 0:
								node1.currentStateCache = DirectoryBasedCacheState::Shared;
								dataWriteBackFlag = 1;
								node2.currentStateCache = DirectoryBasedCacheState::Invalid;
								node3.currentStateCache = DirectoryBasedCacheState::Invalid;
								node4.currentStateCache = DirectoryBasedCacheState::Invalid;
								fetchFlag = 1;
								break;

							case 1:
								break;

							default:
								break;
						}

						stateTransitionDirectory();

					}

					break;

				case 1:							// Write/Store Instruction

					if (node1.currentStateCache == DirectoryBasedCacheState::Invalid) {

						node1.currentStateCache = DirectoryBasedCacheState::Modified;

						stateTransitionDirectory();


					}
					else if (node1.currentStateCache == DirectoryBasedCacheState::Shared) {

						node1.currentStateCache = DirectoryBasedCacheState::Modified;

						node2.currentStateCache = DirectoryBasedCacheState::Invalid;
						node3.currentStateCache = DirectoryBasedCacheState::Invalid;
						node4.currentStateCache = DirectoryBasedCacheState::Invalid;

						switch (node1.cpuWriteHit) {			

							case 0:
								break;

							case 1:
								invalidateFlag = 1;				// Sets invalidate flag
								node2.currentStateCache = DirectoryBasedCacheState::Invalid;
								node3.currentStateCache = DirectoryBasedCacheState::Invalid;
								node4.currentStateCache = DirectoryBasedCacheState::Invalid;
								break;

							default:
								break;

						}

						stateTransitionDirectory();

					}
					else if (node1.currentStateCache == DirectoryBasedCacheState::Modified) {

						switch (node1.cpuWriteHit) {
							case 0:
								break;

							case 1:
								node1.currentStateCache = DirectoryBasedCacheState::Modified;
								break;

							default:
								break;
						}

						switch (node1.cpuWriteMiss) {
							case 0:
								break;

							case 1:
								node1.currentStateCache = DirectoryBasedCacheState::Modified;
								dataWriteBackFlag = 1;
								node2.currentStateCache = DirectoryBasedCacheState::Invalid;
								node3.currentStateCache = DirectoryBasedCacheState::Invalid;
								node4.currentStateCache = DirectoryBasedCacheState::Invalid;
								break;

							default:
								break;
						}

						stateTransitionDirectory();

					}

					break;

				default:
					break;
			}

			break;

		case 2:			// Processor 2

			switch (readOrWriteRequest[iter]) {			// Read/Write Instruction Select

			case 0:							// Read/Load Instruction

				if (node2.currentStateCache == DirectoryBasedCacheState::Invalid) {

					node2.currentStateCache = DirectoryBasedCacheState::Shared;

					stateTransitionDirectory();

				}
				else if (node2.currentStateCache == DirectoryBasedCacheState::Shared) {

					node2.currentStateCache = DirectoryBasedCacheState::Shared;

					stateTransitionDirectory();

				}
				else if (node2.currentStateCache == DirectoryBasedCacheState::Modified) {

					switch (node2.cpuReadHit) {
					case 0:
						break;

					case 1:
						node2.currentStateCache = DirectoryBasedCacheState::Modified;
						break;

					default:
						break;
					}

					switch (node2.cpuReadMiss) {
					case 0:
						node2.currentStateCache = DirectoryBasedCacheState::Shared;
						dataWriteBackFlag = 1;
						node1.currentStateCache = DirectoryBasedCacheState::Invalid;
						node3.currentStateCache = DirectoryBasedCacheState::Invalid;
						node4.currentStateCache = DirectoryBasedCacheState::Invalid;
						fetchFlag = 1;
						break;

					case 1:
						break;

					default:
						break;
					}

					stateTransitionDirectory();

				}

				break;

			case 1:							// Write/Store Instruction

				if (node2.currentStateCache == DirectoryBasedCacheState::Invalid) {

					node2.currentStateCache = DirectoryBasedCacheState::Modified;

					stateTransitionDirectory();


				}
				else if (node2.currentStateCache == DirectoryBasedCacheState::Shared) {

					node2.currentStateCache = DirectoryBasedCacheState::Modified;

					node1.currentStateCache = DirectoryBasedCacheState::Invalid;
					node3.currentStateCache = DirectoryBasedCacheState::Invalid;
					node4.currentStateCache = DirectoryBasedCacheState::Invalid;

					switch (node2.cpuWriteHit) {

					case 0:
						break;

					case 1:
						invalidateFlag = 1;				// Sets invalidate flag
						node1.currentStateCache = DirectoryBasedCacheState::Invalid;
						node3.currentStateCache = DirectoryBasedCacheState::Invalid;
						node4.currentStateCache = DirectoryBasedCacheState::Invalid;
						break;

					default:
						break;

					}

					stateTransitionDirectory();

				}
				else if (node2.currentStateCache == DirectoryBasedCacheState::Modified) {

					switch (node2.cpuWriteHit) {
					case 0:
						break;

					case 1:
						node2.currentStateCache = DirectoryBasedCacheState::Modified;
						break;

					default:
						break;
					}

					switch (node2.cpuWriteMiss) {
					case 0:
						node2.currentStateCache = DirectoryBasedCacheState::Modified;
						dataWriteBackFlag = 1;
						node1.currentStateCache = DirectoryBasedCacheState::Invalid;
						node3.currentStateCache = DirectoryBasedCacheState::Invalid;
						node4.currentStateCache = DirectoryBasedCacheState::Invalid;
						break;

					case 1:
						break;

					default:
						break;
					}

					stateTransitionDirectory();

				}

				break;

			default:
				break;
			}

			break;

		case 3:			// Processor 3

			switch (readOrWriteRequest[iter]) {			// Read/Write Instruction Select

			case 0:							// Read/Load Instruction

				if (node3.currentStateCache == DirectoryBasedCacheState::Invalid) {

					node3.currentStateCache = DirectoryBasedCacheState::Shared;

					stateTransitionDirectory();

				}
				else if (node3.currentStateCache == DirectoryBasedCacheState::Shared) {

					node3.currentStateCache = DirectoryBasedCacheState::Shared;

					stateTransitionDirectory();

				}
				else if (node3.currentStateCache == DirectoryBasedCacheState::Modified) {

					switch (node3.cpuReadHit) {
					case 0:
						break;

					case 1:
						node3.currentStateCache = DirectoryBasedCacheState::Modified;
						break;

					default:
						break;
					}

					switch (node3.cpuReadMiss) {
					case 0:
						node3.currentStateCache = DirectoryBasedCacheState::Shared;
						dataWriteBackFlag = 1;
						node2.currentStateCache = DirectoryBasedCacheState::Invalid;
						node1.currentStateCache = DirectoryBasedCacheState::Invalid;
						node4.currentStateCache = DirectoryBasedCacheState::Invalid;
						fetchFlag = 1;
						break;

					case 1:
						break;

					default:
						break;
					}

					stateTransitionDirectory();

				}

				break;

			case 1:							// Write/Store Instruction

				if (node3.currentStateCache == DirectoryBasedCacheState::Invalid) {

					node3.currentStateCache = DirectoryBasedCacheState::Modified;

					stateTransitionDirectory();


				}
				else if (node3.currentStateCache == DirectoryBasedCacheState::Shared) {

					node3.currentStateCache = DirectoryBasedCacheState::Modified;

					node2.currentStateCache = DirectoryBasedCacheState::Invalid;
					node1.currentStateCache = DirectoryBasedCacheState::Invalid;
					node4.currentStateCache = DirectoryBasedCacheState::Invalid;

					switch (node3.cpuWriteHit) {

					case 0:
						break;

					case 1:
						invalidateFlag = 1;				// Sets invalidate flag
						node2.currentStateCache = DirectoryBasedCacheState::Invalid;
						node1.currentStateCache = DirectoryBasedCacheState::Invalid;
						node4.currentStateCache = DirectoryBasedCacheState::Invalid;
						break;

					default:
						break;

					}

					stateTransitionDirectory();

				}
				else if (node3.currentStateCache == DirectoryBasedCacheState::Modified) {

					switch (node3.cpuWriteHit) {
					case 0:
						break;

					case 1:
						node3.currentStateCache = DirectoryBasedCacheState::Modified;
						break;

					default:
						break;
					}

					switch (node3.cpuWriteMiss) {
					case 0:
						node3.currentStateCache = DirectoryBasedCacheState::Modified;
						dataWriteBackFlag = 1;
						node2.currentStateCache = DirectoryBasedCacheState::Invalid;
						node1.currentStateCache = DirectoryBasedCacheState::Invalid;
						node4.currentStateCache = DirectoryBasedCacheState::Invalid;
						break;

					case 1:
						break;

					default:
						break;
					}

					stateTransitionDirectory();

				}

				break;

			default:
				break;
			}

			break;

		case 4:			// Processor 4

			switch (readOrWriteRequest[iter]) {			// Read/Write Instruction Select

			case 0:							// Read/Load Instruction

				if (node4.currentStateCache == DirectoryBasedCacheState::Invalid) {

					node4.currentStateCache = DirectoryBasedCacheState::Shared;

					stateTransitionDirectory();

				}
				else if (node4.currentStateCache == DirectoryBasedCacheState::Shared) {

					node4.currentStateCache = DirectoryBasedCacheState::Shared;

					stateTransitionDirectory();

				}
				else if (node4.currentStateCache == DirectoryBasedCacheState::Modified) {

					switch (node4.cpuReadHit) {
					case 0:
						break;

					case 1:
						node4.currentStateCache = DirectoryBasedCacheState::Modified;
						break;

					default:
						break;
					}

					switch (node4.cpuReadMiss) {
					case 0:
						node4.currentStateCache = DirectoryBasedCacheState::Shared;
						dataWriteBackFlag = 1;
						node2.currentStateCache = DirectoryBasedCacheState::Invalid;
						node3.currentStateCache = DirectoryBasedCacheState::Invalid;
						node1.currentStateCache = DirectoryBasedCacheState::Invalid;
						fetchFlag = 1;
						break;

					case 1:
						break;

					default:
						break;
					}

					stateTransitionDirectory();

				}

				break;

			case 1:							// Write/Store Instruction

				if (node4.currentStateCache == DirectoryBasedCacheState::Invalid) {

					node4.currentStateCache = DirectoryBasedCacheState::Modified;

					stateTransitionDirectory();


				}
				else if (node4.currentStateCache == DirectoryBasedCacheState::Shared) {

					node4.currentStateCache = DirectoryBasedCacheState::Modified;

					node2.currentStateCache = DirectoryBasedCacheState::Invalid;
					node3.currentStateCache = DirectoryBasedCacheState::Invalid;
					node1.currentStateCache = DirectoryBasedCacheState::Invalid;

					switch (node4.cpuWriteHit) {

					case 0:
						break;

					case 1:
						invalidateFlag = 1;				// Sets invalidate flag
						node2.currentStateCache = DirectoryBasedCacheState::Invalid;
						node3.currentStateCache = DirectoryBasedCacheState::Invalid;
						node1.currentStateCache = DirectoryBasedCacheState::Invalid;
						break;

					default:
						break;

					}

					stateTransitionDirectory();

				}
				else if (node4.currentStateCache == DirectoryBasedCacheState::Modified) {

					switch (node4.cpuWriteHit) {
					case 0:
						break;

					case 1:
						node4.currentStateCache = DirectoryBasedCacheState::Modified;
						break;

					default:
						break;
					}

					switch (node4.cpuWriteMiss) {
					case 0:
						node4.currentStateCache = DirectoryBasedCacheState::Modified;
						dataWriteBackFlag = 1;
						node2.currentStateCache = DirectoryBasedCacheState::Invalid;
						node3.currentStateCache = DirectoryBasedCacheState::Invalid;
						node1.currentStateCache = DirectoryBasedCacheState::Invalid;
						break;

					case 1:
						break;

					default:
						break;
					}

					stateTransitionDirectory();

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

// Function that updates the state transition for directories
void stateTransitionDirectory() {

	switch (processorRequestSel[iter]) {

		case 1:		// Processor 1

			switch (readOrWriteRequest[iter]) {

				case 0:

					if (node1.currentStateDirectory == DirectoryBasedDirectoryState::Uncached ) {

						node1.currentStateDirectory = DirectoryBasedDirectoryState::Shared;


					}
					else if (node1.currentStateDirectory == DirectoryBasedDirectoryState::Shared && node1.cpuReadMiss == 0) {


						node1.currentStateDirectory = DirectoryBasedDirectoryState::Shared;


					}
					else if (node1.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

						switch (node1.cpuReadMiss) {

							case 0:
								node1.currentStateDirectory = DirectoryBasedDirectoryState::Shared;
								break;

							case 1:
								break;

							default:
								break;
						}

						switch (dataWriteBackFlag) {

						case 0:
							break;

						case 1:
							node1.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
							break;

						default:
							break;
						}

						switch (fetchFlag) {

						case 0:
							break;

						case 1:
							node1.currentStateDirectory = DirectoryBasedDirectoryState::Shared;
							break;

						default:
							break;
						}

					}

					break;

				case 1:

					if (node1.currentStateDirectory == DirectoryBasedDirectoryState::Uncached && node1.cpuWriteMiss == 0) {

						node1.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;

						node2.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						node3.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						node4.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;


					}
					else if (node1.currentStateDirectory == DirectoryBasedDirectoryState::Shared && node1.cpuWriteMiss == 0) {


						node1.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;

						node2.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						node3.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						node4.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						


					}
					else if (node1.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

						switch (node1.cpuWriteMiss) {

							case 0:
								node1.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;
								break;

							case 1:
								break;

							default:
								break;
						}

						switch (dataWriteBackFlag) {

							case 0:
								break;

							case 1:
								if (node1.cpuWriteMiss == 0) {

								}
								else {
									node1.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
								}
								
								break;

							default:
								break;
						}

					}

					break;

				default:
					break;


			}

			break;

		case 2:		// Processor 2

			switch (readOrWriteRequest[iter]) {

			case 0:

				if (node2.currentStateDirectory == DirectoryBasedDirectoryState::Uncached && node2.cpuReadMiss == 0) {

					node2.currentStateDirectory = DirectoryBasedDirectoryState::Shared;


				}
				else if (node2.currentStateDirectory == DirectoryBasedDirectoryState::Shared && node2.cpuReadMiss == 0) {


					node2.currentStateDirectory = DirectoryBasedDirectoryState::Shared;


				}
				else if (node2.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

					switch (node2.cpuReadMiss) {

					case 0:
						node2.currentStateDirectory = DirectoryBasedDirectoryState::Shared;
						break;

					case 1:
						break;

					default:
						break;
					}

					switch (dataWriteBackFlag) {

					case 0:
						break;

					case 1:
						node2.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						break;

					default:
						break;
					}

					switch (fetchFlag) {

					case 0:
						break;

					case 1:
						node2.currentStateDirectory = DirectoryBasedDirectoryState::Shared;
						break;

					default:
						break;
					}

				}

				break;

			case 1:

				if (node2.currentStateDirectory == DirectoryBasedDirectoryState::Uncached && node2.cpuWriteMiss == 0) {

					node2.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;

					node1.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node3.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node4.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;


				}
				else if (node2.currentStateDirectory == DirectoryBasedDirectoryState::Shared && node2.cpuWriteMiss == 0) {


					node2.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;

					node1.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node3.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node4.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;


				}
				else if (node2.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

					switch (node2.cpuWriteMiss) {

					case 0:
						node2.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;
						break;

					case 1:
						break;

					default:
						break;
					}

					switch (dataWriteBackFlag) {

					case 0:
						break;

					case 1:
						if (node2.cpuWriteMiss == 0) {

						}
						else {
							node2.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						}
						break;

					default:
						break;
					}

				}

				break;

			default:
				break;


			}

			break;

		case 3:		// Processor 3

			switch (readOrWriteRequest[iter]) {

			case 0:

				if (node3.currentStateDirectory == DirectoryBasedDirectoryState::Uncached && node3.cpuReadMiss == 0) {

					node3.currentStateDirectory = DirectoryBasedDirectoryState::Shared;


				}
				else if (node3.currentStateDirectory == DirectoryBasedDirectoryState::Shared && node3.cpuReadMiss == 0) {


					node3.currentStateDirectory = DirectoryBasedDirectoryState::Shared;


				}
				else if (node3.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

					switch (node3.cpuReadMiss) {

					case 0:
						node3.currentStateDirectory = DirectoryBasedDirectoryState::Shared;
						break;

					case 1:
						break;

					default:
						break;
					}

					switch (dataWriteBackFlag) {

					case 0:
						break;

					case 1:
						node3.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						break;

					default:
						break;
					}

					switch (fetchFlag) {

					case 0:
						break;

					case 1:
						node3.currentStateDirectory = DirectoryBasedDirectoryState::Shared;
						break;

					default:
						break;
					}

				}

				break;

			case 1:

				if (node3.currentStateDirectory == DirectoryBasedDirectoryState::Uncached && node3.cpuWriteMiss == 0) {

					node3.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;

					node2.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node1.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node4.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;


				}
				else if (node3.currentStateDirectory == DirectoryBasedDirectoryState::Shared && node3.cpuWriteMiss == 0) {


					node3.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;

					node2.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node1.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node4.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;


				}
				else if (node3.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

					switch (node3.cpuWriteMiss) {

					case 0:
						node3.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;
						break;

					case 1:
						break;

					default:
						break;
					}

					switch (dataWriteBackFlag) {

					case 0:
						break;

					case 1:
						if (node3.cpuWriteMiss == 0) {
							
						}
						else {
							node3.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						}
						
						break;

					default:
						break;
					}

				}

				break;

			default:
				break;


			}

			break;

		case 4:		// Processor 4

			switch (readOrWriteRequest[iter]) {

			case 0:

				if (node4.currentStateDirectory == DirectoryBasedDirectoryState::Uncached && node4.cpuReadMiss == 0) {

					node4.currentStateDirectory = DirectoryBasedDirectoryState::Shared;


				}
				else if (node4.currentStateDirectory == DirectoryBasedDirectoryState::Shared && node4.cpuReadMiss == 0) {


					node4.currentStateDirectory = DirectoryBasedDirectoryState::Shared;


				}
				else if (node4.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

					switch (node4.cpuReadMiss) {

					case 0:
						node4.currentStateDirectory = DirectoryBasedDirectoryState::Shared;
						break;

					case 1:
						break;

					default:
						break;
					}

					switch (dataWriteBackFlag) {

					case 0:
						break;

					case 1:
						node4.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						break;

					default:
						break;
					}

					switch (fetchFlag) {

					case 0:
						break;

					case 1:
						node4.currentStateDirectory = DirectoryBasedDirectoryState::Shared;
						break;

					default:
						break;
					}

				}

				break;

			case 1:

				if (node4.currentStateDirectory == DirectoryBasedDirectoryState::Uncached && node4.cpuWriteMiss == 0) {

					node4.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;

					node2.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node3.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node1.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;


				}
				else if (node4.currentStateDirectory == DirectoryBasedDirectoryState::Shared && node4.cpuWriteMiss == 0) {


					node4.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;

					node2.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node3.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
					node1.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;


				}
				else if (node4.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

					switch (node4.cpuWriteMiss) {

					case 0:
						node4.currentStateDirectory = DirectoryBasedDirectoryState::Exclusive;
						break;

					case 1:
						break;

					default:
						break;
					}

					switch (dataWriteBackFlag) {

					case 0:
						break;

					case 1:
						if (node4.cpuWriteMiss == 0) {

						}
						else {
							node4.currentStateDirectory = DirectoryBasedDirectoryState::Uncached;
						}
						break;

					default:
						break;
					}

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

// Function to determine the supplier source of the data
void supplierDetermine() {

	int temp = -1;

	if (readOrWriteRequest[iter] == 0) {		// Parameters for load requests

		if (node1.currentStateCache == DirectoryBasedCacheState::Invalid && node2.currentStateCache == DirectoryBasedCacheState::Invalid && node3.currentStateCache == DirectoryBasedCacheState::Invalid && node4.currentStateCache == DirectoryBasedCacheState::Invalid) {

			supplier = source::Memory;

		}
		else if (node1.currentStateCache != DirectoryBasedCacheState::Invalid && node2.currentStateCache == DirectoryBasedCacheState::Invalid && node3.currentStateCache == DirectoryBasedCacheState::Invalid && node4.currentStateCache == DirectoryBasedCacheState::Invalid) {

			supplier = source::N1;

		}
		else if (node1.currentStateCache == DirectoryBasedCacheState::Invalid && node2.currentStateCache != DirectoryBasedCacheState::Invalid && node3.currentStateCache == DirectoryBasedCacheState::Invalid && node4.currentStateCache == DirectoryBasedCacheState::Invalid) {

			supplier = source::N2;

		}
		else if (node1.currentStateCache == DirectoryBasedCacheState::Invalid && node2.currentStateCache == DirectoryBasedCacheState::Invalid && node3.currentStateCache != DirectoryBasedCacheState::Invalid && node4.currentStateCache == DirectoryBasedCacheState::Invalid) {

			supplier = source::N3;

		}
		else if (node1.currentStateCache == DirectoryBasedCacheState::Invalid && node2.currentStateCache == DirectoryBasedCacheState::Invalid && node3.currentStateCache == DirectoryBasedCacheState::Invalid && node4.currentStateCache != DirectoryBasedCacheState::Invalid) {

			supplier = source::N4;

		}
		else if (readOrWriteRequest[iter - 1] == 1) {

			temp = processorRequestSel[iter - 1];	// Retrieves previous processor processed

			switch (temp) {
			case 1:
				supplier = source::N1;
				break;

			case 2:
				supplier = source::N2;
				break;

			case 3:
				supplier = source::N3;
				break;

			case 4:
				supplier = source::N4;
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
void clockCycleCount() {

	if (supplier == source::Memory || readOrWriteRequest[iter] == 1) {		// Cache access, directory access, network access, and mem access
		clockCycle += 6;
	}
	else if (processorRequestSel[iter] == supplier) {

		clockCycle += 2;									// Cache access, directory access
	}
	else if (processorRequestSel[iter] != supplier) {
		clockCycle += 3;									// Cache access, directory access, network access
	}


}

// Function to print the proper output format to terminal
void printOutput() {

	int fixPrint = 0;		// Variable to fix print spacing

	if (readOrWriteRequest[iter] == 0) {			// Prints Load/Store of Instruction
		printf("    Load X, ");
	}
	else if (readOrWriteRequest[iter] == 1) {
		printf("   Store X, ");
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

	if (hitOrMissOnRequest[iter] == 0) {			// Prints Load/Store of Instruction
		printf("              Miss");
	}
	else if (hitOrMissOnRequest[iter] == 1) {
		printf("              Hit ");
	}

	if (node1.currentStateCache == DirectoryBasedCacheState::Invalid) {			// Print cache 1 state

		printf("             I");

	}
	else if (node1.currentStateCache == DirectoryBasedCacheState::Shared) {

		printf("             S");

	}
	else if (node1.currentStateCache == DirectoryBasedCacheState::Modified) {

		printf("             M");

	}



	if (node2.currentStateCache == DirectoryBasedCacheState::Invalid) {			// Print cache 2 state

		printf(" I");

	}
	else if (node2.currentStateCache == DirectoryBasedCacheState::Shared) {

		printf(" S");

	}
	else if (node2.currentStateCache == DirectoryBasedCacheState::Modified) {

		printf(" M");

	}


	if (node3.currentStateCache == DirectoryBasedCacheState::Invalid) {			// Print cache 3 state

		printf(" I");

	}
	else if (node3.currentStateCache == DirectoryBasedCacheState::Shared) {

		printf(" S");

	}
	else if (node3.currentStateCache == DirectoryBasedCacheState::Modified) {

		printf(" M");

	}


	if (node4.currentStateCache == DirectoryBasedCacheState::Invalid) {			// Print cache 4 state

		printf(" I");

	}
	else if (node4.currentStateCache == DirectoryBasedCacheState::Shared) {

		printf(" S");

	}
	else if (node4.currentStateCache == DirectoryBasedCacheState::Modified) {

		printf(" M");

	}


	if (node1.currentStateDirectory == DirectoryBasedDirectoryState::Uncached) {			// Print directory 1 state

		printf("            U");

	}
	else if (node1.currentStateDirectory == DirectoryBasedDirectoryState::Shared) {

		printf("            S");

	}
	else if (node1.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

		printf("            E");

	}



	if (node2.currentStateDirectory == DirectoryBasedDirectoryState::Uncached) {			// Print directory 2 state

		printf(" U");

	}
	else if (node2.currentStateDirectory == DirectoryBasedDirectoryState::Shared) {

		printf(" S");

	}
	else if (node2.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

		printf(" E");

	}


	if (node3.currentStateDirectory == DirectoryBasedDirectoryState::Uncached) {			// Print directory 3 state

		printf(" U");

	}
	else if (node3.currentStateDirectory == DirectoryBasedDirectoryState::Shared) {

		printf(" S");

	}
	else if (node3.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

		printf(" E");

	}


	if (node4.currentStateDirectory == DirectoryBasedDirectoryState::Uncached) {			// Print directory 4 state

		printf(" U");

	}
	else if (node4.currentStateDirectory == DirectoryBasedDirectoryState::Shared) {

		printf(" S");

	}
	else if (node4.currentStateDirectory == DirectoryBasedDirectoryState::Exclusive) {

		printf(" E");

	}



	switch (supplier) {									// Prints the supplier source

	case 0:
		fixPrint = 0;
		printf("             Memory");
		break;

	case 1:
		fixPrint = 1;
		printf("               N1");
		break;

	case 2:
		fixPrint = 1;
		printf("               N2");
		break;

	case 3:
		fixPrint = 1;
		printf("               N3");
		break;

	case 4:
		fixPrint = 1;
		printf("               N4");
		break;

	case 5:
		fixPrint = 0;
		printf("           Proc. Store");
		break;
	}

	if (fixPrint == 0) {							// Prints the current clock cycle
		printf("\t\t%d", clockCycle);
	}
	else if (fixPrint == 1) {
		printf("\t\t%d", clockCycle);
	}


	printf("\n");

}
