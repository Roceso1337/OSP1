#include "main.h"

const int 

int main(int argc, char *argv[])
{
	if(argc < 3) err("Invalid arguments\n");

	std::string inFname=argv[1];
	std::string outFname=argv[2];

	std::ifstream fd(inFname.c_str());
	std::string line;
	std::vector<std::string> lines;

	if(fd != NULL)
	{
		while (std::getline(fd, line)){
			lines.push_back(line);
		}

		std::deque<process> processList;
		process::parse(lines, processList);
		FCFS(processList);

		int n = lines.size();
		const int m = 1;
		const int t_cs = 8;
		const int t_slice = 84;

		SJF(processList, t_cs);

		fd.close();
	}

	return 0;
}

std::string queueToString(std::deque<process> queue){
	if (queue.empty()){
		return "[Q empty]";
	}
	std::string queueString = "[Q ";

	while (!queue.empty()){
		queueString += queue.front().getID();
		queue.pop_front();

		if (!queue.empty())
			queueString += " ";
	}
	
	queueString += "]";
	return queueString;
}

void sortFCFS(std::deque<process>& processList, std::deque<process>& cpuQ, std::deque<process>& ioQ)
{
	std::sort(processList.begin(), processList.end(), process::FCFSComp);
	std::sort(cpuQ.begin(), cpuQ.end(), process::FCFSComp);
	std::sort(ioQ.begin(), ioQ.end(), process::FCFSComp);
}

void FCFS(std::deque<process> processList)
{
	//int m=1;
	int t_cs=8;
	process running=process();

	std::deque<process> cpuQ;
	std::deque<process> ioQ;
	std::cout<<"time 0ms: Simulator started for FCFS ";
	std::cout<<queueToString(cpuQ)<<std::endl;

	//sort the processes
	sortFCFS(processList, cpuQ, ioQ);

	//"run the processes"
	int timeElapsed=0;
	while((processList.size() > 0) ||//arrivals
		(cpuQ.size() > 0) ||//cpu
		(ioQ.size() > 0) ||//io
		(running.getID() != ""))//the cpu is still running
	{
		process ap;
		process cpup;
		process iop;
		
		//arrivals
		if(!processList.empty())
			ap=processList.front();
		//cpu
		if(!cpuQ.empty())
			cpup=cpuQ.front();
		//io
		if(!ioQ.empty())
			iop=ioQ.front();

		//still have arrivals left
		bool taskCompleted=false;
		if(!processList.empty())
		{
			if(((cpuQ.empty()) || (ap.getInitialArrivalTime() < cpup.getArrivalTime())) &&
				((ioQ.empty()) || (ap.getInitialArrivalTime() < iop.getArrivalTime())))
			{
				//set the timeElapsed and print
				timeElapsed=ap.getInitialArrivalTime();
				std::cout<<"time "<<timeElapsed<<"ms: ";
				std::cout<<"Process "<<ap.getID()<<" arrived ";

				//set the new arrival time
				if(!cpuQ.empty())
					ap.setArrivalTime(cpuQ.back().getArrivalTime()+t_cs);
				else
					ap.setArrivalTime(timeElapsed+t_cs);

				//set some data
				taskCompleted=true;

				//change the deques
				cpuQ.push_back(ap);
				processList.pop_front();
			}
		}

		if(!taskCompleted)
		{
			if(((cpuQ.empty()) || (running.getArrivalTime() < cpup.getArrivalTime())) &&
				((ioQ.empty()) || (running.getArrivalTime() < iop.getArrivalTime())))
			{
				//set the timeElapsed and print
				timeElapsed=running.getArrivalTime();
				std::cout<<"time "<<timeElapsed<<"ms: ";

				//do a burst
				running.decrementNumBurstsLeft();

				//just another burst
				if(running.getNumBurstsLeft() > 0)
				{
					std::cout<<"Process "<<running.getID()<<" completed a CPU burst; ";
					std::cout<<running.getNumBurstsLeft()<<" to go ";
					std::cout<<queueToString(cpuQ)<<std::endl;

					//IO blocked
					std::cout<<"time "<<timeElapsed<<"ms: ";
					std::cout<<"Process "<<running.getID()<<" blocked on I/O until time ";
					std::cout<<(running.getArrivalTime())<<"ms";

					//set the new arrival time
					running.setArrivalTime(timeElapsed+running.getIOTime());

					//set some data
					running=process();

					//change the deques
					cpuQ.pop_front();
					ioQ.push_back(running);
				}
				else//no more bursts left
				{
					std::cout<<"Process "<<cpup.getID()<<" terminated ";

					//set the new arrival time
					cpup.setArrivalTime(timeElapsed+cpup.getInitialArrivalTime());

					//set some data
					running=process();

					//change the deques
					cpuQ.pop_front();
				}

				taskCompleted=true;
			}
		}

		if(!taskCompleted)//cpu and io
		{
			//cpu or io
			bool doCPU=false;
			if(!cpuQ.empty())
			{
				if((ioQ.empty()) || (cpup.getArrivalTime() < iop.getArrivalTime()))
					doCPU=true;
			}

			if(doCPU)//cpu
			{
				//set the timeElapsed and print
				timeElapsed=cpup.getArrivalTime();
				std::cout<<"time "<<timeElapsed<<"ms: ";

				std::cout<<"Process "<<cpup.getID()<<" started using the CPU ";

				//set the new arrival times
				cpup.setArrivalTime(timeElapsed+cpup.getCPUBurstTime());
				for(unsigned int i=0;i<cpuQ.size();++i)
					cpuQ[i].setArrivalTime(cpuQ[i].getArrivalTime()+cpup.getCPUBurstTime());

				//set some data
				running=cpup;

				//change the deques
				cpuQ.pop_front();
			}
			else//io
			{
				//set the timeElapsed and print
				timeElapsed=iop.getArrivalTime();
				std::cout<<"time "<<timeElapsed<<"ms: ";

				if(!ioQ.empty())
				{
					std::cout<<"Process "<<iop.getID()<<" completed I/O ";

					//set the timeElapsed
					timeElapsed=iop.getArrivalTime();

					//set the new arrival time
					iop.setArrivalTime(timeElapsed+t_cs);

					//change the deques
					cpuQ.push_back(iop);
					ioQ.pop_front();
				}
			}
		}

		std::cout<<queueToString(cpuQ)<<std::endl;

		//sort
		sortFCFS(processList, cpuQ, ioQ);
	}
}

void FCFS(std::deque<process> processList)
{
	//int m=1;
	int t_cs=8;

	std::deque<process> cpuQ;
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
		processList.pop_front();

		//time elapsed so far
		timeElapsed=p.getArrivalTime();
		std::cout<<"time "<<timeElapsed<<"ms: ";

		//arrival time
		if(timeElapsed == p.getInitialArrivalTime())
		{
			std::cout<<"Process "<<p.getID()<<" arrived ";

			addBack=true;
			cpuAddBack=true;
			p.setArrivalTime(timeElapsed+t_cs);
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
				cpuQ.pop_front();
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
				p.setArrivalTime(timeElapsed+t_cs);
				p.setCPU(true);
				p.setIO(false);
			}
		}

		//add the p to the qs if necassary
		if(cpuAddBack) cpuQ.push_back(p);
		if(addBack) processList.push_back(p);

		std::cout<<queueToString(cpuQ)<<std::endl;

		//sort the processes
		sortFCFS(processList);
	}
}

// make temporary "current" process
// make new queue of ready queue
// if arrival time <= elapsed time
void SJF(std::deque<process> processList, int t_cs)
{
	int timeElapsed = 0;
	process* currentProcess = new process();
	std::deque<process> readyQueue;
	std::deque<process> ioQueue;
	bool busy = false;

	std::cout << "time " << timeElapsed << "ms: Simulator started for SJF " 
		<< queueToString(readyQueue) << std::endl;

	while (1){
		for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
			if (itr->getArrivalTime() <= timeElapsed){
				readyQueue.push_back(*itr);
				std::cout << process::printTime(timeElapsed) << " Process " 
					<< itr->getID() << " arrived " << queueToString(readyQueue) << std::endl;
				processList.erase(itr);
			}

			if (itr == processList.end())
				break;
		}

		std::sort(readyQueue.begin(), readyQueue.end(), process::SJTComp);

		//can start a new process
		if (!busy && !readyQueue.empty()){
			currentProcess  = *readyQueue.front();
			busy = true;
			std::cout << process::printTime(elaspedTime) << " Process " << currentProcess.getID() 
				<< " arrived " << queueToString(readyQueue) << std::endl;
			readyQueue.pop_front();
			if (currentProcess.getNumBurstsLeft() > 0)
				currentProcess.decrementNumBurstLeft();
				readyQueue.push_back(currentProcess);
		}

	}
}


void sortRR(std::deque<process>& processList){
	std::sort(processList.begin(), processList.end(), process::FCFSComp);
}

//when the time slice expires, check the queue to determine if a context switch should occur.
void roundRobin(std::deque<process> processList, int t_cs, int t_slice){
	int timeElapsed = 0;
	process* currentProcess = new process();
	std::deque<process> readyQueue;
	std::deque<process> ioQueue;
	bool busy = false;

	std::cout << "time " << timeElapsed << "ms: Simulator started for RR " 
		<< queueToString(readyQueue) << std::endl;

	sortRR(processList);
	readyQueue = processList;

	//jump to time of first process arrival
	timeElapsed = processList.begin().getArrivalTime; //CHECK

//START OF PRIMARY LOOP
	while(1){
		for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
			if (itr->getArrivalTime() <= timeElapsed){
				readyQueue.push_back(*itr);
				std::cout << process::printTime(timeElapsed) << " Process " 
					<< itr->getID() << " arrived " << queueToString(readyQueue) << std::endl;
				processList.erase(itr);
			}

			if (itr == processList.end())
				break;
		}

	//TRY TO START NEW PROCESS
		if (!busy && !readyQueue.empty()){
			currentProcess  = *readyQueue.front();
			busy = true;
			std::cout << process::printTime(elaspedTime) << " Process " << currentProcess.getID() 
				<< " arrived " << queueToString(readyQueue) << std::endl;
			readyQueue.pop_front();
		}

	//RUN TIME SLICE

		if(currentProcess.cpuBurstTimeLeft() > t_slice){
			//update timeElapsed
			timeElapsed += t_slice;
			//update remaining burst time
			currentProcess.setCpuBurstTimeLeft(currentProcess.cpuBurstTimeLeft()-t_slice);
			//check for new process arrival in time slice
			for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
				if (itr->getArrivalTime() <= timeElapsed){
					readyQueue.push_back(*itr);
					std::cout << process::printTime(timeElapsed) << " Process " 
						<< itr->getID() << " arrived " << queueToString(readyQueue) << std::endl;
					processList.erase(itr);
				}
			}

			//preemption
			if(!readyQueue.empty()){ //switch processes if readyQueue is not empty
				readyQueue.push_back(currentProcess);
				currentProcess = readyQueue.front();
				readyQueue.pop_front();
				
				//print output
				std::cout << process::printTime(timeElapsed) << " Time slice expired; process " << currentProcess.getID()
					<< " preemted with " << currentProcess.cpuBurstTimeLeft() << "ms to go " << queueToString(readyQueue) << std::endl;
			}else{ //readyQueue is empty, continue with process
				//do nothing???
			}


		}else{ //remaining CPU time <= t_slice (finishes burst)
			//update timeElapsed
			timeElapsed += currentProcess.cpuBurstTimeLeft();
			//check for new process arrival in time slice
			for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
				if (itr->getArrivalTime() <= timeElapsed){
					readyQueue.push_back(*itr);
					std::cout << process::printTime(timeElapsed) << "Process " 
						<< itr->getID() << " arrived " << queueToString(readyQueue) << std::endl;
					processList.erase(itr);
				}
			}
			if(currentProcess.getNumBurstsLeft()-1 > 0){ //still has more burst cycles left
				currentProcess.decrementNumBurstsLeft();
				//reset CPUBurstTimeLeft
				currentProcess.setCpuBurstTimeLeft(currentProcess.getCPUBurstTime());
			}else{ //no more burst cycles left, terminate process
				//print output
				std::cout << process::printTime(timeElapsed) << "Process " 
					<< itr->getID() << " terminated " << queueToString(readyQueue) << std::endl;
			}

			if(!readyQueue.empty()){ //continue if ready queue is not empty
				currentProcess = readyQueue.front();
				readyQueue.pop_front();
				
				//print output
				std::cout << process.printTime(timeElapsed) << "Process "
			}else{ //readyQueue is empty, continue with process

			}


		}

	}

		
}

void checkForNewArrivals(std::deque<process> processList, int timeElapsed, std::deque<process> readyQueue){
	for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
		if (itr->getArrivalTime() <= timeElapsed){
			readyQueue.push_back(*itr);
			std::cout << process::printTime(timeElapsed) << " Process " 
				<< itr->getID() << " arrived " << queueToString(readyQueue) << std::endl;
			processList.erase(itr);
		}
	}
}

void err(const char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}
