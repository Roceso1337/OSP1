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

void process::parse(std::string text, std::vector<process>& processList)
{
	processList.clear();

	//go through each byte
	for(unsigned int i=0;i<text.length();++i)
	{
		//skip the line(s)
		if((i == 0) || (text[i-1] == '\n'))
		{
			if(text[i] == '\n') continue;
			else if(text[i] == '#')
			{
				while(text[i] != '\n')
					++i;
				continue;
			}

			//if we made it this far, new process!

			//the id
			size_t breakPoint=text.find('|', i);
			if(breakPoint == std::string::npos) return;
			std::string newID=text.substr(i, breakPoint);
			i=breakPoint+1;
			//the arrival time
			breakPoint=text.find('|', i);
			if(breakPoint == std::string::npos) return;
			int newArrivalTime=atoi(text.substr(i, breakPoint).c_str());
			i=breakPoint+1;
			//the burst time
			breakPoint=text.find('|', i);
			if(breakPoint == std::string::npos) return;
			int newCPUBurstTime=atoi(text.substr(i, breakPoint).c_str());
			i=breakPoint+1;
			//the number of bursts
			breakPoint=text.find('|', i);
			if(breakPoint == std::string::npos) return;
			int newNumBursts=atoi(text.substr(i, breakPoint).c_str());
			i=breakPoint+1;
			//the io time
			breakPoint=text.find('\n', i);
			//can be std::string::npos since that end of file
			int newIOTime=atoi(text.substr(i, breakPoint).c_str());
			if(breakPoint > std::string::npos) i=breakPoint+1;
			else return;

			//create the process and add it to the vector
			processList.push_back(process(newID, newArrivalTime, newCPUBurstTime, newNumBursts, newIOTime));
		}
	}
}