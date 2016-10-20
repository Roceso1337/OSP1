#ifndef _MAIN
#define _MAIN

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include "process.h"

struct process;

void FCFS(const std::queue<process>&);
void SJF(const std::queue<process>&);
void roundRobin(const std::queue<process>&);
void err(const char*);

#endif
