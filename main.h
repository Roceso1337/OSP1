#ifndef _MAIN
#define _MAIN

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "process.h"

struct process;

std::string intTOstring(int);
std::string floatTOstring(float);
std::string queueToString(std::deque<process>);
void schedule(std::deque<process>, int, int, float&, float&, float&, int&, int&, int);
void err(const char*);

#endif
