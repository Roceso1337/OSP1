#include "process.h"

process::process()
{
	id="";
	arrivalTime=-1;
	cpuBurstTime=-1;
	numBursts=-1;
	ioTime=-1;
}

process::process(std::string newID, int newArrivalTime, int newCPUBurstTime, int newNumBursts, int newIOTime)
{
	id=newID;
	arrivalTime=newArrivalTime;
	cpuBurstTime=newCPUBurstTime;
	numBursts=newNumBursts;
	ioTime=newIOTime;
}

void process::parse(std::vector<std::string>& text, std::vector<process>& processList)
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
            splitText = strtok(parseString, "|");
        }

        if (paramList.size() != 5)
            return;

        std::string newID = paramList[0];
        int newArrivalTime = atoi(paramList[1].c_str());
        int newCPUBurstTime = atoi(paramList[2].c_str());
        int newNumBursts = atoi(paramList[3].c_str());
        int newIOTime = atoi(paramList[4].c_str());

        //create the process and add it to the vector
        processList.push_back(process(newID, newArrivalTime, newCPUBurstTime, newNumBursts, newIOTime));
	}
}
