#include "main.h"

int main(int argc, char *argv[])
{
	if(argc < 2) err("Invalid arguments\n");

	std::string fname=argv[1];


    std::ifstream fd(fname.c_str());
    std::string line;
    std::vector<std::string> lines;

	if(fd != NULL)
	{
        while (std::getline(fd, line)){
            lines.push_back(line);
        }

		std::queue<process> processList;
		process::parse(lines, processList);
		FCFS(processList);

        fd.close();
	}

    return 0;
}

std::string queueToString(std::queue<process> queue){
    if (queue.empty()){
        return "[Q empty]";
    }
    std::string queueString = "[Q ";

    while (!queue.empty()){
        queueString += queue.front().getID();
        queue.pop();

        if (!queue.empty())
            queueString += " ";
    }
    
    queueString += "]";
    return queueString;
}

void sortFCFS(std::queue<process>& processList)
{
	std::vector<process> tempPList;
	int pSize=processList.size();
	for(int i=0;i<pSize;++i)
	{
		process p=processList.front();
		processList.pop();

		tempPList.push_back(p);

		processList.push(p);
	}

	//sort the vector
	std::sort(tempPList.begin(), tempPList.end(), process::FCFSComp);

	//clear the queue
	for(int i=0;i<pSize;++i)
		processList.pop();

	//add the new ordered queue
	for(int i=0;i<pSize;++i)
		processList.push(tempPList[i]);
}

void FCFS(std::queue<process> processList)
{
	std::queue<process> cpuQ;
	std::cout<<"time 0ms: Simulator started for FCFS ";
	std::cout<<queueToString(cpuQ)<<std::endl;

	//sort the processes
	sortFCFS(processList);

	//"run the processes"
	int timeElapsed=0;
	while(processList.size() > 0)
	{
		//set the process variable
		bool addBack=false;
		bool cpuAddBack=false;
		process p=processList.front();
		processList.pop();

		//time elapsed so far
		timeElapsed=p.getArrivalTime();
		std::cout<<"time "<<timeElapsed<<"ms: ";

		//arrival time
		if(timeElapsed == p.getInitialArrivalTime())
		{
			std::cout<<"Process "<<p.getID()<<" arrived ";

			addBack=true;
			cpuAddBack=true;
			//p.setArrivalTime(backP.getArrivalTime()+4);
			p.setCPU(false);
			p.setIO(false);
		}
		else//CPU and IO
		{
			if(!p.isCPU() && !p.isIO())
			{
				//the burst
				std::cout<<"Process "<<p.getID()<<" started using the CPU ";

				addBack=true;
				cpuAddBack=false;
				cpuQ.pop();
				p.setArrivalTime(timeElapsed+p.getInitialArrivalTime());
				p.setCPU(true);
				p.setIO(false);
			}
			else if(p.isCPU())
			{
				//do a burst
				p.decrementNumBurstsLeft();

				//just another burst
				if(p.getNumBurstsLeft() > 0)
				{
					std::cout<<"Process "<<p.getID()<<" completed a CPU burst; ";
					std::cout<<p.getNumBurstsLeft()<<" to go ";
					std::cout<<queueToString(cpuQ)<<std::endl;

					//IO blocked
					std::cout<<"time "<<timeElapsed<<"ms: ";
					std::cout<<"Process "<<p.getID()<<" blocked on I/O until time ";
					std::cout<<(p.getArrivalTime())<<"ms";

					addBack=true;
					cpuAddBack=false;
					p.setArrivalTime(timeElapsed+p.getIOTime());
					p.setCPU(false);
					p.setIO(true);
				}
				else//no more bursts left
				{
					std::cout<<"Process "<<p.getID()<<" terminated ";

					addBack=false;
					cpuAddBack=false;
					p.setCPU(false);
					p.setIO(false);
				}
			}
			else if(p.isIO())
			{
				std::cout<<"Process "<<p.getID()<<" completed I/O ";

				addBack=true;
				cpuAddBack=true;
				p.setArrivalTime(timeElapsed+4);
				p.setCPU(true);
				p.setIO(false);
			}
		}

		//add the p to the qs if necassary
		if(cpuAddBack) cpuQ.push(p);
		if(addBack) processList.push(p);

		std::cout<<queueToString(cpuQ)<<std::endl;

		//sort the processes
		sortFCFS(processList);
	}
}

void SJF(const std::queue<process>& processList)
{
    int timeElapsed = 0;
    std::cout << "time " << timeElapsed << "ms: Simulator started for SJF " << std::endl;

}

void roundRobin(const std::queue<process>& processList)
{
	//
}

void err(const char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}
