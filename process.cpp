#include "process.h"

process::process()
{
	id="";
    initialArrivalTime=-1;
	arrivalTime=-1;
    EndTime=-1;
	cpuBurstTime=-1;
	numBursts=-1;
    numBurstsLeft=-1;
	ioTime=-1;
}

process::process(std::string newID, int newArrivalTime, int newCPUBurstTime, int newNumBursts, int newIOTime)
{
	id=newID;
    initialArrivalTime=newArrivalTime;
	arrivalTime=initialArrivalTime;
    EndTime=0;
	cpuBurstTime=newCPUBurstTime;
	numBursts=newNumBursts;
    numBurstsLeft=numBursts;
	ioTime=newIOTime;
    cpuBurstTimeLeft = cpuBurstTime;
}

void process::parse(std::vector<std::string>& text, std::deque<process>& processList)
{
	processList.clear();

    //go through each byte
	for(unsigned int i=0;i<text.size();++i)
	{
        if (text[i][0] == '#')
            continue;
        if (text[i].empty())
            continue;
        //if we made it this far, new process!
        char* parseString = &text[i][0]; 
        char* splitText = strtok(parseString, "|");
        std::vector<std::string> paramList;  

        while (splitText != NULL){
            std::string convText(splitText);
            paramList.push_back(convText);
            splitText = strtok(NULL, "|");
        }

        if (paramList.size() != 5)
            exit(EXIT_FAILURE);

        std::string newID = paramList[0];
        int newArrivalTime = atoi(paramList[1].c_str());
        int newCPUBurstTime = atoi(paramList[2].c_str());
        int newNumBursts = atoi(paramList[3].c_str());
        int newIOTime = atoi(paramList[4].c_str());

        //create the process and add it to the queue
        processList.push_back(process(newID, newArrivalTime, newCPUBurstTime, newNumBursts, newIOTime));
	}
}

std::string process::getID(){
	return id;
}

int process::getInitialArrivalTime(){
	return initialArrivalTime;
}

int process::getArrivalTime(){
	return arrivalTime;
}

void process::setArrivalTime(int newArrivalTime){
	arrivalTime=newArrivalTime;
}

int process::getCPUBurstTime(){
	return cpuBurstTime;
}

int process::getCPUBurstTimeLeft(){
    return cpuBurstTimeLeft;
}

int process::getNumBursts(){
	return numBursts;
}

void process::decrementNumBurstsLeft(){
	--numBurstsLeft;
}

int process::getNumBurstsLeft(){
	return numBurstsLeft;
}

int process::getIOTime(){
	return ioTime;
}

int process::getCpuBurstStart(){
    return cpuBurstStart;
}

int process::getBurstEnd(){
    return cpuBurstEnd;
}
int process::getIOEnd(){
    return ioEndTime;
}

void process::setIOEnd(int time){
    ioEndTime = time;
}

void process::setCpuBurstStart(int time){
    cpuBurstStart = time;
}

void process::setCpuBurstEnd(int time){ 
    cpuBurstEnd = time;
}

void process::setCpuBurstTimeLeft(int newTime){
    cpuBurstTimeLeft = newTime;
}