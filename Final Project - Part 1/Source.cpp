//***********************************************************************
// ECGR 5181 - Assignment 5 - Cache Simulation							*		
// (C++ Verision)														*
//																		*
// This program will utilize custom structures/variables to simulate 	*
// how a computer processor would pipeline a looped event in assembly	*
// instructions via C code for single and multiple processors.			*
// 																		*															
// Kendall Britton, Jaron Price, Sreekanksha Gangadhar		2022-11-07	* 
//***********************************************************************

#include <iostream>
#include <unordered_map>
#include <list>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

#define CACHESIZE 256
#define CACHELINESIZE 32
#define ASSOCIATIVITY 0

// Data structure that simulates a LRU Cache
typedef struct CacheLRU {

	list<int> keyStorage;	// Keeps tracks on keys/last LRU used

	unordered_map<int, list<int>::iterator> mapKeys;		// Stores references of keys in cache

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
vector<int> lastElementUsed1Optimize;
vector<int> lastElementUsed2Optimize;
vector<int> lastElementUsed3Optimize;
vector<int> lastElementUsed4Optimize;
vector<int> lastElementUsed5Optimize;
vector<int> lastElementUsed6Optimize;
vector<int> lastElementUsed7Optimize;
vector<int> lastElementUsed8Optimize;


int cache1PositionArray[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
int cache2PositionArray[16] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
int cache3PositionArray[4] = { -1, -1, -1, -1 };
int cache4PositionArray[2][4] = { {-1, -1, -1, -1}, {-1, -1, -1, -1} };
int cache5PositionArray[4][2] = { {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1} };


fstream myFile;
CacheLRU cache1, cache2, cache3, cache4, cache5;

void readFromMemory();
void readBinaryValue(int numSets);
void LRU4WayCacheMiss();
void LRU4WayCacheHit();
void LRUOptimizeCacheHit();
void LRUOptimizeCacheMiss();

int main() {


	cache1.capacitySize = 256;			// Cache 1 details
	cache1.cacheLineSize = 32;
	cache1.cacheAssociativity = 1;

	cache1.numberOfOffsetBits = log2(cache1.cacheLineSize);
	cache1.numberOfSets = cache1.capacitySize / cache1.cacheLineSize;
	cache1.numberOfIndexBitsPerSet = log2(cache1.numberOfSets);

	cache1.numberOfTagBits = cache1.numberOfBitsInAddress - cache1.numberOfOffsetBits - cache1.numberOfIndexBitsPerSet;
	cache1.numberOfTotalBlocks = cache1.numberOfSets * cache1.cacheAssociativity;

	cache2.capacitySize = 512;			// Cache 2 details
	cache2.cacheLineSize = 32;
	cache2.cacheAssociativity = 1;

	cache2.numberOfOffsetBits = log2(cache2.cacheLineSize);
	cache2.numberOfSets = cache2.capacitySize / cache2.cacheLineSize;
	cache2.numberOfIndexBitsPerSet = log2(cache2.numberOfSets);

	cache2.numberOfTagBits = cache2.numberOfBitsInAddress - cache2.numberOfOffsetBits - cache1.numberOfIndexBitsPerSet;
	cache2.numberOfTotalBlocks = cache2.numberOfSets * cache2.cacheAssociativity;

	cache3.capacitySize = 256;			// Cache 3 details
	cache3.cacheLineSize = 64;
	cache3.cacheAssociativity = 1;

	cache3.numberOfOffsetBits = log2(cache3.cacheLineSize);
	cache3.numberOfSets = cache3.capacitySize / cache3.cacheLineSize;
	cache3.numberOfIndexBitsPerSet = log2(cache3.numberOfSets);

	cache3.numberOfTagBits = cache3.numberOfBitsInAddress - cache3.numberOfOffsetBits - cache1.numberOfIndexBitsPerSet;
	cache3.numberOfTotalBlocks = cache3.numberOfSets * cache3.cacheAssociativity;

	cache4.capacitySize = 256;			// Cache 4 details
	cache4.cacheLineSize = 32;
	cache4.cacheAssociativity = 4;

	cache4.numberOfOffsetBits = log2(cache4.cacheLineSize);
	cache4.numberOfSets = cache4.capacitySize / (cache4.cacheLineSize * cache4.cacheAssociativity);
	cache4.numberOfIndexBitsPerSet = log2(cache4.numberOfSets);

	cache4.numberOfTagBits = cache4.numberOfBitsInAddress - cache4.numberOfOffsetBits - cache1.numberOfIndexBitsPerSet;
	cache4.numberOfTotalBlocks = cache4.numberOfSets * cache4.cacheAssociativity;

	cache5.capacitySize = 256;			// Cache 5 details
	cache5.cacheLineSize = 32;
	cache5.cacheAssociativity = 2;

	cache5.numberOfOffsetBits = log2(cache5.cacheLineSize);
	cache5.numberOfSets = cache5.capacitySize / (cache5.cacheLineSize * cache5.cacheAssociativity);
	cache5.numberOfIndexBitsPerSet = log2(cache5.numberOfSets);

	cache5.numberOfTagBits = cache5.numberOfBitsInAddress - cache5.numberOfOffsetBits - cache5.numberOfIndexBitsPerSet;
	cache5.numberOfTotalBlocks = cache5.numberOfSets * cache5.cacheAssociativity;


	myFile.open("addresses.txt", ios::in);

	for (int i = 0; i < 1000; i++) {

		readFromMemory();

		readBinaryValue(cache1.numberOfSets);

		if (cache1PositionArray[indexSetBitFromBinary] == tagBitFromBinary) {				// Determines hit or misses for cache 1

			cache1.numberOfHits++;

		}
		else if (cache1PositionArray[indexSetBitFromBinary] == NULL || cache1PositionArray[indexSetBitFromBinary] != tagBitFromBinary) {


			cache1.numberOfMisses++;

		}

		cache1PositionArray[indexSetBitFromBinary] = tagBitFromBinary;		// Assigns tag at location



		readBinaryValue(cache2.numberOfSets);

		if (cache2PositionArray[indexSetBitFromBinary] == tagBitFromBinary) {				// Determines hit or misses for cache 2

			cache2.numberOfHits++;

		}
		else if (cache2PositionArray[indexSetBitFromBinary] == NULL || cache2PositionArray[indexSetBitFromBinary] != tagBitFromBinary) {


			cache2.numberOfMisses++;

		}

		cache2PositionArray[indexSetBitFromBinary] = tagBitFromBinary;		// Assigns tag at location



		readBinaryValue(cache3.numberOfSets);

		if (cache3PositionArray[indexSetBitFromBinary] == tagBitFromBinary) {				// Determines hit or misses for cache 3

			cache3.numberOfHits++;

		}
		else if (cache3PositionArray[indexSetBitFromBinary] == NULL || cache3PositionArray[indexSetBitFromBinary] != tagBitFromBinary) {


			cache3.numberOfMisses++;

		}

		cache3PositionArray[indexSetBitFromBinary] = tagBitFromBinary;		// Assigns tag at location

		readBinaryValue(cache4.numberOfSets);

		if (cache4PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary || cache4PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary || cache4PositionArray[indexSetBitFromBinary][2] == tagBitFromBinary || cache4PositionArray[indexSetBitFromBinary][3] == tagBitFromBinary) {				// Determines hit or misses for cache 4

			cache4.numberOfHits++;

			LRU4WayCacheHit();

		}
		else {


			cache4.numberOfMisses++;

			LRU4WayCacheMiss();

		}

		readBinaryValue(cache5.numberOfSets);

		if (cache5PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary || cache5PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {				// Determines hit or misses for cache 4

			cache5.numberOfHits++;

			LRUOptimizeCacheHit();

		}
		else {


			cache5.numberOfMisses++;

			LRUOptimizeCacheMiss();

		}


		cache1.totalAccesses++;
		cache2.totalAccesses++;
		cache3.totalAccesses++;
		cache4.totalAccesses++;
		cache5.totalAccesses++;

	}

	myFile.close();

	cache1.lookUpTime = (ceil(log2(cache1.capacitySize / cache1.cacheLineSize)) * cache1.cacheAssociativity) + (cache1.numberOfMisses * 100);
	cache2.lookUpTime = (ceil(log2(cache2.capacitySize / cache2.cacheLineSize)) * cache2.cacheAssociativity) + (cache2.numberOfMisses * 100);
	cache3.lookUpTime = (ceil(log2(cache3.capacitySize / cache3.cacheLineSize)) * cache3.cacheAssociativity) + (cache3.numberOfMisses * 100);
	cache4.lookUpTime = (ceil(log2(cache4.capacitySize / cache4.cacheLineSize)) * cache4.cacheAssociativity) + (cache4.numberOfMisses * 100);
	cache5.lookUpTime = (ceil(log2(cache5.capacitySize / cache5.cacheLineSize)) * cache5.cacheAssociativity) + (cache5.numberOfMisses * 100);



	printf("Cache 1:\n  Hit Rate: %f\n  Compulsory Miss Rate: %f\n  Total Trace Simulation Time: %d\n\n", ((double)cache1.numberOfHits / 1000) * 100, ((double)cache1.numberOfMisses / 1000) * 100, cache1.lookUpTime);
	printf("Cache 2:\n  Hit Rate: %f\n  Compulsory Miss Rate: %f\n  Total Trace Simulation Time: %d\n\n", ((double)cache2.numberOfHits / 1000) * 100, ((double)cache2.numberOfMisses / 1000) * 100, cache2.lookUpTime);
	printf("Cache 3:\n  Hit Rate: %f\n  Compulsory Miss Rate: %f\n  Total Trace Simulation Time: %d\n\n", ((double)cache3.numberOfHits / 1000) * 100, ((double)cache3.numberOfMisses / 1000) * 100, cache3.lookUpTime);
	printf("Cache 4:\n  Hit Rate: %f\n  Compulsory Miss Rate: %f\n  Total Trace Simulation Time: %d\n\n", ((double)cache4.numberOfHits / 1000) * 100, ((double)cache4.numberOfMisses / 1000) * 100, cache4.lookUpTime);
	printf("Cache 5:\n  Hit Rate: %f\n  Compulsory Miss Rate: %f\n  Total Trace Simulation Time: %d\n\n", ((double)cache5.numberOfHits / 1000) * 100, ((double)cache5.numberOfMisses / 1000) * 100, cache5.lookUpTime);



	return 0;
}

void readFromMemory() {							// Reads memory address location

	decimalAddressValue = 0;
	int decimalPosition = 7;

	if (!myFile.is_open()) {
		cout << "Error opening file" << endl;
	}
	else {
		string line;
		int bitPosition = 0;

		for (int j = 0; j < 1; j++) {

			getline(myFile, line);

			for (int i = 2; i < 10; i++) {								// Determine tag bit value

				if (line[i] == 'A' || line[i] == 'a') {

					binaryArray[bitPosition] = 1;
					binaryArray[bitPosition + 1] = 0;
					binaryArray[bitPosition + 2] = 1;
					binaryArray[bitPosition + 3] = 0;

					decimalAddressValue += 10 * pow(16, decimalPosition);
				}
				else if (line[i] == 'B' || line[i] == 'b') {

					binaryArray[bitPosition] = 1;
					binaryArray[bitPosition + 1] = 0;
					binaryArray[bitPosition + 2] = 1;
					binaryArray[bitPosition + 3] = 1;

					decimalAddressValue += 11 * pow(16, decimalPosition);
				}
				else if (line[i] == 'C' || line[i] == 'c') {

					binaryArray[bitPosition] = 1;
					binaryArray[bitPosition + 1] = 1;
					binaryArray[bitPosition + 2] = 0;
					binaryArray[bitPosition + 3] = 0;

					decimalAddressValue += 12 * pow(16, decimalPosition);
				}
				else if (line[i] == 'D' || line[i] == 'd') {

					binaryArray[bitPosition] = 1;
					binaryArray[bitPosition + 1] = 1;
					binaryArray[bitPosition + 2] = 0;
					binaryArray[bitPosition + 3] = 1;

					decimalAddressValue += 13 * pow(16, decimalPosition);
				}
				else if (line[i] == 'E' || line[i] == 'e') {

					binaryArray[bitPosition] = 1;
					binaryArray[bitPosition + 1] = 1;
					binaryArray[bitPosition + 2] = 1;
					binaryArray[bitPosition + 3] = 0;

					decimalAddressValue += 14 * pow(16, decimalPosition);
				}
				else if (line[i] == 'F' || line[i] == 'f') {

					binaryArray[bitPosition] = 1;
					binaryArray[bitPosition + 1] = 1;
					binaryArray[bitPosition + 2] = 1;
					binaryArray[bitPosition + 3] = 1;

					decimalAddressValue += 15 * pow(16, decimalPosition);
				}
				else if (line[i] == '0') {

					binaryArray[bitPosition] = 0;
					binaryArray[bitPosition + 1] = 0;
					binaryArray[bitPosition + 2] = 0;
					binaryArray[bitPosition + 3] = 0;

					decimalAddressValue += 0 * pow(16, decimalPosition);
				}
				else if (line[i] == '1') {

					binaryArray[bitPosition] = 0;
					binaryArray[bitPosition + 1] = 0;
					binaryArray[bitPosition + 2] = 0;
					binaryArray[bitPosition + 3] = 1;

					decimalAddressValue += 1 * pow(16, decimalPosition);
				}
				else if (line[i] == '2') {

					binaryArray[bitPosition] = 0;
					binaryArray[bitPosition + 1] = 0;
					binaryArray[bitPosition + 2] = 1;
					binaryArray[bitPosition + 3] = 0;

					decimalAddressValue += 2 * pow(16, decimalPosition);
				}
				else if (line[i] == '3') {

					binaryArray[bitPosition] = 0;
					binaryArray[bitPosition + 1] = 0;
					binaryArray[bitPosition + 2] = 1;
					binaryArray[bitPosition + 3] = 1;

					decimalAddressValue += 3 * pow(16, decimalPosition);
				}
				else if (line[i] == '4') {

					binaryArray[bitPosition] = 0;
					binaryArray[bitPosition + 1] = 1;
					binaryArray[bitPosition + 2] = 0;
					binaryArray[bitPosition + 3] = 0;

					decimalAddressValue += 4 * pow(16, decimalPosition);
				}
				else if (line[i] == '5') {

					binaryArray[bitPosition] = 0;
					binaryArray[bitPosition + 1] = 1;
					binaryArray[bitPosition + 2] = 0;
					binaryArray[bitPosition + 3] = 1;

					decimalAddressValue += 5 * pow(16, decimalPosition);
				}
				else if (line[i] == '6') {

					binaryArray[bitPosition] = 0;
					binaryArray[bitPosition + 1] = 1;
					binaryArray[bitPosition + 2] = 1;
					binaryArray[bitPosition + 3] = 0;

					decimalAddressValue += 6 * pow(16, decimalPosition);
				}
				else if (line[i] == '7') {

					binaryArray[bitPosition] = 0;
					binaryArray[bitPosition + 1] = 1;
					binaryArray[bitPosition + 2] = 1;
					binaryArray[bitPosition + 3] = 1;

					decimalAddressValue += 7 * pow(16, decimalPosition);
				}
				else if (line[i] == '8') {

					binaryArray[bitPosition] = 1;
					binaryArray[bitPosition + 1] = 0;
					binaryArray[bitPosition + 2] = 0;
					binaryArray[bitPosition + 3] = 0;

					decimalAddressValue += 8 * pow(16, decimalPosition);
				}
				else if (line[i] == '9') {

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

}

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

void LRU4WayCacheMiss() {

	int temp;

	if (indexSetBitFromBinary == 0 && cache4PositionArray[0][0] == -1) {

		cache4PositionArray[0][0] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 0) {

			cache4.lruList1[0] = 1;
			lastElementUsed1.push_back(0);

		}

	}
	else if (indexSetBitFromBinary == 1 && cache4PositionArray[1][0] == -1) {

		cache4PositionArray[1][0] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 1) {

			cache4.lruList1[0] = 1;
			lastElementUsed1.push_back(0);

		}

	}
	else if (indexSetBitFromBinary == 0 && cache4PositionArray[0][1] == -1) {

		cache4PositionArray[0][1] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 0) {

			cache4.lruList1[1] = 1;
			lastElementUsed1.push_back(1);

		}

	}
	else if (indexSetBitFromBinary == 1 && cache4PositionArray[1][1] == -1) {

		cache4PositionArray[1][1] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 1) {

			cache4.lruList2[1] = 1;
			lastElementUsed2.push_back(1);

		}

	}
	else if (indexSetBitFromBinary == 0 && cache4PositionArray[0][2] == -1) {

		cache4PositionArray[0][2] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 0) {

			cache4.lruList1[2] = 1;
			lastElementUsed1.push_back(2);

		}

	}
	else if (indexSetBitFromBinary == 1 && cache4PositionArray[1][2] == -1) {

		cache4PositionArray[1][2] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 1) {

			cache4.lruList2[2] = 1;
			lastElementUsed2.push_back(2);

		}

	}
	else if (indexSetBitFromBinary == 0 && cache4PositionArray[0][3] == -1) {

		cache4PositionArray[0][3] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 0) {

			cache4.lruList1[3] = 1;
			lastElementUsed1.push_back(3);

		}

	}
	else if (indexSetBitFromBinary == 1 && cache4PositionArray[1][3] == -1) {

		cache4PositionArray[1][3] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 1) {

			cache4.lruList2[3] = 1;
			lastElementUsed2.push_back(3);

		}

	}
	else {

		if (indexSetBitFromBinary == 0) {

			temp = lastElementUsed1.back();

			lastElementUsed1.pop_back();

			if (temp == 0) {

				cache4PositionArray[0][temp] = tagBitFromBinary;
				lastElementUsed1.push_back(0);

			}
			else if (temp == 1) {

				cache4PositionArray[0][temp] = tagBitFromBinary;
				lastElementUsed1.push_back(1);

			}
			else if (temp == 2) {

				cache4PositionArray[0][temp] = tagBitFromBinary;
				lastElementUsed1.push_back(2);

			}
			else if (temp == 3) {

				cache4PositionArray[0][temp] = tagBitFromBinary;
				lastElementUsed1.push_back(3);

			}

		}
		else if (indexSetBitFromBinary == 1) {

			temp = lastElementUsed2.back();

			lastElementUsed2.pop_back();

			if (temp == 0) {

				cache4PositionArray[1][temp] = tagBitFromBinary;
				lastElementUsed2.push_back(0);

			}
			else if (temp == 1) {

				cache4PositionArray[1][temp] = tagBitFromBinary;
				lastElementUsed2.push_back(1);

			}
			else if (temp == 2) {

				cache4PositionArray[1][temp] = tagBitFromBinary;
				lastElementUsed2.push_back(2);

			}
			else if (temp == 3) {

				cache4PositionArray[1][temp] = tagBitFromBinary;
				lastElementUsed2.push_back(3);

			}

		}

	}

}

void LRU4WayCacheHit() {

	if (indexSetBitFromBinary == 0) {

		if (cache4PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed1.size(); i++) {

				if (lastElementUsed1.at(i) == 0) {
					lastElementUsed1.erase(lastElementUsed1.begin() + i);
				}

			}

			lastElementUsed1.push_back(0);

		}
		else if (cache4PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed1.size(); i++) {

				if (lastElementUsed1.at(i) == 1) {
					lastElementUsed1.erase(lastElementUsed1.begin() + i);
				}

			}

			lastElementUsed1.push_back(1);

		}
		else if (cache4PositionArray[indexSetBitFromBinary][2] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed1.size(); i++) {

				if (lastElementUsed1.at(i) == 2) {
					lastElementUsed1.erase(lastElementUsed1.begin() + i);
				}

			}

			lastElementUsed1.push_back(2);

		}
		else if (cache4PositionArray[indexSetBitFromBinary][3] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed1.size(); i++) {

				if (lastElementUsed1.at(i) == 3) {
					lastElementUsed1.erase(lastElementUsed1.begin() + i);
				}

			}

			lastElementUsed1.push_back(3);

		}

	}
	else if (indexSetBitFromBinary == 1) {

		if (cache4PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed2.size(); i++) {

				if (lastElementUsed2.at(i) == 0) {
					lastElementUsed2.erase(lastElementUsed2.begin() + i);
				}

			}

			lastElementUsed2.push_back(0);

		}
		else if (cache4PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed2.size(); i++) {

				if (lastElementUsed2.at(i) == 1) {
					lastElementUsed2.erase(lastElementUsed2.begin() + i);
				}

			}

			lastElementUsed2.push_back(1);

		}
		else if (cache4PositionArray[indexSetBitFromBinary][2] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed2.size(); i++) {

				if (lastElementUsed2.at(i) == 2) {
					lastElementUsed2.erase(lastElementUsed2.begin() + i);
				}

			}

			lastElementUsed2.push_back(2);

		}
		else if (cache4PositionArray[indexSetBitFromBinary][3] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed2.size(); i++) {

				if (lastElementUsed2.at(i) == 3) {
					lastElementUsed2.erase(lastElementUsed2.begin() + i);
				}

			}

			lastElementUsed2.push_back(3);

		}

	}

}

void LRUOptimizeCacheMiss() {

	int temp;

	if (indexSetBitFromBinary == 0 && cache5PositionArray[0][0] == -1) {

		cache5PositionArray[0][0] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 0) {

			cache5.lruList1[0] = 1;
			lastElementUsed1Optimize.push_back(0);

		}

	}
	else if (indexSetBitFromBinary == 0 && cache5PositionArray[0][1] == -1) {

		cache5PositionArray[0][1] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 0) {

			cache5.lruList1[1] = 1;
			lastElementUsed1Optimize.push_back(1);

		}

	}
	else if (indexSetBitFromBinary == 1 && cache5PositionArray[1][0] == -1) {

		cache5PositionArray[1][0] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 1) {

			cache5.lruList1[0] = 1;
			lastElementUsed2Optimize.push_back(0);

		}

	}
	else if (indexSetBitFromBinary == 1 && cache5PositionArray[1][1] == -1) {

		cache5PositionArray[1][1] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 1) {

			cache5.lruList2[1] = 1;
			lastElementUsed2Optimize.push_back(1);

		}

	}
	else if (indexSetBitFromBinary == 2 && cache5PositionArray[2][0] == -1) {

		cache5PositionArray[2][0] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 2) {

			cache5.lruList1[0] = 1;
			lastElementUsed3Optimize.push_back(0);

		}

	}
	else if (indexSetBitFromBinary == 2 && cache5PositionArray[2][1] == -1) {

		cache5PositionArray[2][1] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 2) {

			cache5.lruList2[1] = 1;
			lastElementUsed3Optimize.push_back(1);

		}

	}
	else if (indexSetBitFromBinary == 3 && cache5PositionArray[3][0] == -1) {

		cache5PositionArray[3][0] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 3) {

			cache5.lruList1[0] = 1;
			lastElementUsed4Optimize.push_back(0);

		}

	}
	else if (indexSetBitFromBinary == 3 && cache5PositionArray[3][1] == -1) {

		cache5PositionArray[3][1] = tagBitFromBinary;		// Assigns tag at location

		if (indexSetBitFromBinary == 3) {

			cache5.lruList2[1] = 1;
			lastElementUsed4Optimize.push_back(1);

		}

	}
	else {

		if (indexSetBitFromBinary == 0) {

			temp = lastElementUsed1Optimize.back();

			lastElementUsed1Optimize.pop_back();

			if (temp == 0) {

				cache5PositionArray[0][temp] = tagBitFromBinary;
				lastElementUsed1Optimize.push_back(0);

			}
			else if (temp == 1) {

				cache5PositionArray[0][temp] = tagBitFromBinary;
				lastElementUsed1Optimize.push_back(1);

			}

		}
		else if (indexSetBitFromBinary == 1) {

			temp = lastElementUsed2Optimize.back();

			lastElementUsed2Optimize.pop_back();

			if (temp == 0) {

				cache5PositionArray[1][temp] = tagBitFromBinary;
				lastElementUsed2Optimize.push_back(0);

			}
			else if (temp == 1) {

				cache5PositionArray[1][temp] = tagBitFromBinary;
				lastElementUsed2Optimize.push_back(1);

			}

		}
		else if (indexSetBitFromBinary == 2) {

			temp = lastElementUsed3Optimize.back();

			lastElementUsed3Optimize.pop_back();

			if (temp == 0) {

				cache5PositionArray[2][temp] = tagBitFromBinary;
				lastElementUsed3Optimize.push_back(0);

			}
			else if (temp == 1) {

				cache5PositionArray[2][temp] = tagBitFromBinary;
				lastElementUsed3Optimize.push_back(1);

			}

		}
		else if (indexSetBitFromBinary == 3) {

			temp = lastElementUsed4Optimize.back();

			lastElementUsed4Optimize.pop_back();

			if (temp == 0) {

				cache5PositionArray[3][temp] = tagBitFromBinary;
				lastElementUsed4Optimize.push_back(0);

			}
			else if (temp == 1) {

				cache5PositionArray[3][temp] = tagBitFromBinary;
				lastElementUsed4Optimize.push_back(1);

			}

		}

	}

}

void LRUOptimizeCacheHit() {

	if (indexSetBitFromBinary == 0) {

		if (cache5PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed1Optimize.size(); i++) {

				if (lastElementUsed1Optimize.at(i) == 0) {
					lastElementUsed1Optimize.erase(lastElementUsed1Optimize.begin() + i);
				}

			}

			lastElementUsed1Optimize.push_back(0);

		}
		else if (cache5PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

			for (int i = 0; i < lastElementUsed1Optimize.size(); i++) {

				if (lastElementUsed1Optimize.at(i) == 1) {
					lastElementUsed1Optimize.erase(lastElementUsed1Optimize.begin() + i);
				}

			}

			lastElementUsed1Optimize.push_back(1);

		}
		else if (indexSetBitFromBinary == 1) {

			if (cache5PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

				for (int i = 0; i < lastElementUsed2Optimize.size(); i++) {

					if (lastElementUsed2Optimize.at(i) == 0) {
						lastElementUsed2Optimize.erase(lastElementUsed2Optimize.begin() + i);
					}

				}

				lastElementUsed2Optimize.push_back(0);

			}
			else if (cache5PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

				for (int i = 0; i < lastElementUsed2Optimize.size(); i++) {

					if (lastElementUsed2Optimize.at(i) == 1) {
						lastElementUsed2Optimize.erase(lastElementUsed2Optimize.begin() + i);
					}

				}

				lastElementUsed2Optimize.push_back(1);

			}

		}
		else if (indexSetBitFromBinary == 2) {

			if (cache5PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

				for (int i = 0; i < lastElementUsed3Optimize.size(); i++) {

					if (lastElementUsed3Optimize.at(i) == 0) {
						lastElementUsed3Optimize.erase(lastElementUsed3Optimize.begin() + i);
					}

				}

				lastElementUsed3Optimize.push_back(0);

			}
			else if (cache5PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

				for (int i = 0; i < lastElementUsed3Optimize.size(); i++) {

					if (lastElementUsed3Optimize.at(i) == 1) {
						lastElementUsed3Optimize.erase(lastElementUsed3Optimize.begin() + i);
					}

				}

				lastElementUsed3Optimize.push_back(1);

			}

		}
		else if (indexSetBitFromBinary == 3) {

			if (cache5PositionArray[indexSetBitFromBinary][0] == tagBitFromBinary) {

				for (int i = 0; i < lastElementUsed4Optimize.size(); i++) {

					if (lastElementUsed4Optimize.at(i) == 0) {
						lastElementUsed4Optimize.erase(lastElementUsed4Optimize.begin() + i);
					}

				}

				lastElementUsed4Optimize.push_back(0);

			}
			else if (cache5PositionArray[indexSetBitFromBinary][1] == tagBitFromBinary) {

				for (int i = 0; i < lastElementUsed4Optimize.size(); i++) {

					if (lastElementUsed4Optimize.at(i) == 1) {
						lastElementUsed4Optimize.erase(lastElementUsed4Optimize.begin() + i);
					}

				}

				lastElementUsed4Optimize.push_back(1);

			}

		}

	}

}
