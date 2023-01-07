#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <iomanip>
using namespace std;

// Base program for CPU0.s
#define ARRAY_A_ADDR 0x0400
#define ARRAY_B_ADDR 0x0800
#define ARRAY_C_ADDR 0x0C00

typedef struct memoryArray {
    float mainArray[256] = {};
    float* mainArrayAddress[256] = {};
};

float* ARRAY_A = (float*)(void*)ARRAY_A_ADDR;
float* ARRAY_B = (float*)(void*)ARRAY_B_ADDR;
float* ARRAY_C = (float*)(void*)ARRAY_C_ADDR;

int main() {

    srand(time(0));
    
    memoryArray A;
    memoryArray B;
    memoryArray C;

    for (int i = 0; i < 256; i++) {
        A.mainArrayAddress[i] = ARRAY_A + (0x1) * i;
        B.mainArrayAddress[i] = ARRAY_B + (0x1) * i;
        C.mainArrayAddress[i] = ARRAY_C + (0x1) * i;

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
    
	for (int i = 0; i < 10; i++) {
		C.mainArray[i] = A.mainArray[i] - B.mainArray[i];

        printf("Array A Index Value %d: %f    Array B Index Value %d: %f    Array C Index Value %d: %f\n", i, A.mainArray[i], i, B.mainArray[i], i, C.mainArray[i]);
	}


    cout << endl;

    for (int j = 0; j < 10; j++) {

        printf("Array C Address: %p     Array C Value: %f\n", C.mainArrayAddress[j], C.mainArray[j]);

    }
}