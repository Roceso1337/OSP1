#ifndef _PROCESS
#define _PROCESS

#include <deque>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <algorithm>
#include "main.h"
#include <sstream>

class process
{
private:

	std::string id;
	int initialArrivalTime;
	int arrivalTime;
	int EndTime;
	int cpuBurstTime;
	int numBursts;
	int numBurstsLeft;
	int ioTime;
	bool cpu;
	bool io;
    int timeRunning;
    int cpuBurstStart;
    int cpuBurstEnd;
    int ioEndTime;

public:

	//<proc-id>|<initial-arrival-time>|<cpu-burst-time>|<num-bursts>|<io-time>
	process();
	process(std::string, int, int, int, int);
	static void parse(std::vector<std::string>&, std::deque<process>&);
    std::string getID();
    int getInitialArrivalTime();
    int getArrivalTime();
    void setArrivalTime(int);
    int getCPUBurstTime();
    int getNumBursts();
    void decrementNumBurstsLeft();
    int getNumBurstsLeft();
    int getIOTime();
    int getCpuBurstStart();
    int getBurstEnd();
    int getIOEnd();
    int getTimeRunning() { return timeRunning; }
    void setIOEnd(int);
    void setCpuBurstStart(int);
    void setCpuBurstEnd(int); 
    void increaseTimeRunning() { timeRunning++; }
    void setTimeRunning(int time) { timeRunning = time; }

	static bool FCFSComp(const process &p1, const process &p2){
		if(p1.arrivalTime == p2.arrivalTime) return p1.id < p2.id;
		return p1.arrivalTime < p2.arrivalTime;
	}

	static bool SJTComp(const process &p1, const process &p2){
		if(p1.cpuBurstTime == p2.cpuBurstTime) return p1.id < p2.id;
		return p1.cpuBurstTime < p2.cpuBurstTime;
	}


	static bool IOComp(const process &p1, const process &p2){
        return p1.ioEndTime > p2.ioEndTime;
	}

	static std::string printTime(int i){
		std::stringstream time;
		time << i;
		return "time " + time.str() +"ms: ";
	}
};

#endif
