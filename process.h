#ifndef _PROCESS
#define _PROCESS

#include <queue>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <algorithm>
#include "main.h"

class process
{
private:

	std::string id;
	int arrivalTime;
	int cpuBurstTime;
	int numBursts;
	int ioTime;

public:

	//<proc-id>|<initial-arrival-time>|<cpu-burst-time>|<num-bursts>|<io-time>
	process();
	process(std::string, int, int, int, int);
	static void parse(std::vector<std::string>&, std::queue<process>&);
    std::string getID();
    int getArrivalTime();
    int getCPUBurst();
    int getNumBursts();
    int getIOTime();

	static bool FCFSComp(const process &p1, const process &p2){
		return p1.arrivalTime < p2.arrivalTime;
	}

	static bool SJTComp(const process &p1, const process &p2){
		return p1.cpuBurstTime < p2.cpuBurstTime;
	}
};

#endif
