#include "main.h"

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

		FCFS(processList, 8);

        //int n = lines.size();
        //int m = 1;
        //int t_cs = 8;
        //int t_slice = 84;

        //SJF(processList, t_cs);


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

void FCFS(std::deque<process> processList, int t_cs)
{
	//int m=1;

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
				{
					if(running.getID() != "")
						ap.setArrivalTime(running.getArrivalTime()+(t_cs));
					else
						ap.setArrivalTime(timeElapsed+(t_cs/2));
				}

				//set some data
				taskCompleted=true;

				//change the deques
				cpuQ.push_back(ap);
				processList.pop_front();
			}
		}

		if((!taskCompleted) && (running.getID() != ""))//cpu
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
					std::cout<<(timeElapsed+running.getIOTime())<<"ms";

					//set the new arrival time
					running.setArrivalTime(timeElapsed+running.getIOTime());

					//change the deques
					ioQ.push_back(running);

					//clear running
					running=process();
				}
				else//no more bursts left
				{
					std::cout<<"Process "<<running.getID()<<" terminated ";

					//set the new arrival time
					running.setArrivalTime(timeElapsed+running.getInitialArrivalTime());

					//clear running
					running=process();
				}

				taskCompleted=true;
			}
		}

		if((!taskCompleted) && (running.getID() == ""))//cpu start
		{
			if(!cpuQ.empty())
			{
				if((ioQ.empty()) || (cpup.getArrivalTime() < iop.getArrivalTime()))
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
					taskCompleted=true;

					//change the deques
					cpuQ.pop_front();
				}
			}
		}

		if(!taskCompleted)//io
		{
			if(!ioQ.empty())
			{
				//set the timeElapsed and print
				timeElapsed=iop.getArrivalTime();
				std::cout<<"time "<<timeElapsed<<"ms: ";
				std::cout<<"Process "<<iop.getID()<<" completed I/O ";

				//set the new arrival time
				//set the new arrival time
				if(!cpuQ.empty())
					iop.setArrivalTime(cpuQ.back().getArrivalTime()+t_cs);
				else
				{
					if(running.getID() != "")
						iop.setArrivalTime(running.getArrivalTime()+(t_cs));
					else
						iop.setArrivalTime(timeElapsed+(t_cs/2));
				}

				//set some data
				taskCompleted=true;

				//change the deques
				cpuQ.push_back(iop);
				ioQ.pop_front();
			}
		}

		std::cout<<queueToString(cpuQ)<<std::endl;

		//sort
		sortFCFS(processList, cpuQ, ioQ);

	}

	timeElapsed+=4;

	std::cout<<"time "<<timeElapsed<<"ms: Simulator ended for FCFS"<<std::endl;
}

// make temporary "current" process
// make new queue of ready queue
// if arrival time <= elapsed time
void SJF(std::deque<process> processList, int t_cs){
int timeElapsed = 0;
process *currentProcess;
std::deque<process> readyQueue;
std::deque<process> ioQueue;
bool busy = false;

std::cout << "time " << timeElapsed << "ms: Simulator started for SJF " 
    << queueToString(readyQueue) << std::endl;

	while (1){
	    for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
	        if (itr->getArrivalTime() >= timeElapsed){
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
	        currentProcess  = &(readyQueue.front());
	        busy = true;
	        std::cout << process::printTime(timeElapsed) << " Process " << currentProcess->getID() 
	            << " arrived " << queueToString(readyQueue) << std::endl;
	        readyQueue.pop_front();
	        if (currentProcess->getNumBurstsLeft() > 0){
	            currentProcess->decrementNumBurstsLeft();
	            readyQueue.push_back(*currentProcess);
	        }
	    }    

	    if (busy && currentProcess->getCPUBurstTime() == currentProcess->getTimeRunning()){
	        currentProcess->decrementNumBurstsLeft();

	        if (currentProcess->getNumBurstsLeft() == 0){
	            std::cout << process::printTime(timeElapsed) << " Process " << 
	               currentProcess->getID() << " terminated " << queueToString(readyQueue) << std::endl; 
	        } else {
	            std::cout << process::printTime(timeElapsed) << " Process " << 
	                currentProcess->getID() << " completed a CPU burst; " << 
	                currentProcess->getNumBurstsLeft() << " to go " <<
	                queueToString(readyQueue) << std::endl;

	            int ioBlockTime = timeElapsed +  currentProcess->getIOTime();

	            std::cout << process::printTime(timeElapsed) << " Process " <<
	                currentProcess->getID() << " blocked on I/O until time " <<
	                ioBlockTime << " " << queueToString(readyQueue) << std::endl;
	            
	            currentProcess->setIOEnd(ioBlockTime);
	            ioQueue.push_back(*currentProcess);
	        }

	        busy = false;
	    }

	    if (!ioQueue.empty()){
	        std::sort(ioQueue.begin(), ioQueue.end(), process::IOComp);
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

	std::deque<process>::iterator itr = processList.begin();
	//jump to time of first process arrival
	timeElapsed = itr->getArrivalTime(); //CHECK

//START OF PRIMARY LOOP
	while(1){
		for (; itr != processList.end(); itr++){
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
			currentProcess = &readyQueue.front();
			busy = true;
			std::cout << process::printTime(timeElapsed) << " Process " << currentProcess->getID() 
				<< " arrived " << queueToString(readyQueue) << std::endl;
			readyQueue.pop_front();
		}

	//RUN TIME SLICE

		if(currentProcess->getCPUBurstTimeLeft() > t_slice){
			//update timeElapsed
			timeElapsed += t_slice;
			//update remaining burst time
			currentProcess->setCpuBurstTimeLeft(currentProcess->getCPUBurstTimeLeft()-t_slice);
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
				readyQueue.push_back(*currentProcess);
				currentProcess = &readyQueue.front();
				readyQueue.pop_front();
				
				//print output
				std::cout << process::printTime(timeElapsed) << " Time slice expired; process " << currentProcess->getID()
					<< " preemted with " << currentProcess->getCPUBurstTimeLeft() << "ms to go " << queueToString(readyQueue) << std::endl;
			}else{ //readyQueue is empty, continue with process
				//do nothing???
			}


		}else{ //remaining CPU time <= t_slice (finishes burst)
			//update timeElapsed
			timeElapsed += currentProcess->getCPUBurstTimeLeft();
			//check for new process arrival in time slice
			for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
				if (itr->getArrivalTime() <= timeElapsed){
					readyQueue.push_back(*itr);
					std::cout << process::printTime(timeElapsed) << "Process " 
						<< itr->getID() << " arrived " << queueToString(readyQueue) << std::endl;
					processList.erase(itr);
				}
			}
			if(currentProcess->getNumBurstsLeft()-1 > 0){ //still has more burst cycles left
				currentProcess->decrementNumBurstsLeft();
				//reset CPUBurstTimeLeft
				currentProcess->setCpuBurstTimeLeft(currentProcess->getCPUBurstTime());
			}else{ //no more burst cycles left, terminate process
				//print output
				std::cout << process::printTime(timeElapsed) << "Process " 
					<< currentProcess->getID() << " terminated " << queueToString(readyQueue) << std::endl;
			}

			if(!readyQueue.empty()){ //continue if ready queue is not empty
				currentProcess = &readyQueue.front();
				readyQueue.pop_front();
				
				//print output
				std::cout << currentProcess->printTime(timeElapsed) << "Process ";
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
