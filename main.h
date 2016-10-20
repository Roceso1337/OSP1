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

std::string queueToString(std::deque<process>);
void sortFCFS(std::deque<process>&);
void FCFS(std::deque<process>);
void SJF(const std::deque<process>&);
void roundRobin(const std::deque<process>&);
void err(const char*);

#endif
