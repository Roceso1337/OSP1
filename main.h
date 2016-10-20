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

void FCFS(std::deque<process>);
void SJF(std::deque<process>, int);
void roundRobin(std::deque<process>);
void err(const char*);

#endif
