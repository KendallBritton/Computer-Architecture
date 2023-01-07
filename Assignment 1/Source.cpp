//***********************************************************************
// ECGR 5181 - Assignment 1 - Event Driven Simulator	(C++ Verision)	*
//																		*
// This program will utilize custom classes and vectors to simulate how *
// a computer processor would handle/process events that the computer 	*
// asks it to do via instructions.										*
// 																		*															
// Kendall Britton, 2022-09-07											* 
//***********************************************************************


// List of libraries that are required to run the program
#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <queue>
#include <stdlib.h>
#include <thread>
using namespace std;

#define MAXEVENTCOUNT 100		// Defines the max number of events using a macro


// The data structure to create/process an event
typedef struct Event {
	double eventTime;
	double eventValue;

	Event(double t, double v) {
		eventTime = t;
		eventValue = v;
	}

	Event() {

	}

};


// The data structure to used in priority queue to properly order events
typedef struct eventComp {
	bool operator() (Event left, Event right) {
		if (left.eventTime > right.eventTime) {				// First sorts events due to time

			return left.eventTime > right.eventTime;

		}
		else if (left.eventTime == right.eventTime) {	   // If time of events is the same, sorts by event values

			if (left.eventValue <= right.eventValue) {
				return left.eventTime < right.eventTime;
			} 

		}
		else {
			return left.eventTime > right.eventTime;	  // Places events at latter times last
		}

	}

};


// The various vectors used to complete the program
vector<Event> eventList;
priority_queue <Event, vector<Event, allocator<Event>>, eventComp> MEQ;
vector<Event> newEvent;


void popAndPrint();		// Declares the function 

int totalEventCount = 0;		// Initiates event counter
int initialTimeExecution = 0;	// Inititates time to finish initial events


// Runs the main program
int main() {

	auto timeStart = chrono::steady_clock::now();		// Starts capturing time of program
	srand(time(0));

	cout << "Initial 20 events..." << endl;

	for (int i = 0; i < 20; i++) {		// Generates first 20 events

		auto timeEnd = chrono::steady_clock::now();																							// Stops capturing time
		int currentTime = int(std::chrono::duration_cast <std::chrono::milliseconds> (timeEnd - timeStart).count());						// Calculates elasped time
		eventList.push_back(Event(currentTime, rand() % 100));																				// Generates a random value for event at specific time executed
		if (i > 9) {
			cout << "Event " << i << ":  Time: " << eventList.at(i).eventTime << " ms   Value: " << eventList.at(i).eventValue << endl;		// Outputs results
		}
		else {
			cout << "Event " << i << ":   Time: " << eventList.at(i).eventTime << " ms   Value: " << eventList.at(i).eventValue << endl;		// Outputs results
		}
	}

	initialTimeExecution = eventList.at(19).eventTime;		// Time to finish initial events

	for (int i = 0; i < 20; i++) {		// Puts 20 initial events into priority queue

		MEQ.push(eventList.at(i));

	}

	cout << endl << "All 100 events..." << endl;

	popAndPrint();		// Runs function to read/generate new values 

	return 0;
}


// Function that reads the Master Event Queue and outputs values
void popAndPrint() {


	auto newTimeStart = chrono::steady_clock::now();	// Start capturing elasped time


	while (totalEventCount < MAXEVENTCOUNT) {																								// Continues to generate new events until 100 events
		if (totalEventCount > 9) {
			cout << "Event " << totalEventCount << ":  Time: " << MEQ.top().eventTime << " ms   Value: " << MEQ.top().eventValue << endl;	// Outputs current event processed
		}
		else {
			cout << "Event " << totalEventCount << ":   Time: " << MEQ.top().eventTime << " ms   Value: " << MEQ.top().eventValue << endl;	// Outputs current event processed
		}
		newEvent.push_back(Event(MEQ.top().eventTime + ((double)(rand() % initialTimeExecution) + initialTimeExecution), rand() % 100));	// Generates new event
		MEQ.push(newEvent.at(0));																											// Pushes new event into MEQ
		MEQ.pop();																															// Removes processed event from MEQ
		newEvent.pop_back();																												// Removes newly generated event from new event vector
		totalEventCount++;																													// Update total event counter
	}

}