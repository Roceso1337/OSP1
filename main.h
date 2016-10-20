#ifndef _MAIN
#define _MAIN

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "process.h"

struct process;

void FCFS(const std::vector<process>&);
void SJF(const std::vector<process>&);
void roundRobin(const std::vector<process>&);
void err(const char*);

#endif
