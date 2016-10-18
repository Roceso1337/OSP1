#ifndef _PROCESS
#define _PROCESS

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
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
	static void parse(std::vector<std::string>&, std::vector<process>&);
};

#endif
