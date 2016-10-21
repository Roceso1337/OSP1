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

		std::deque<process> processList;
		process::parse(lines, processList);

        int n = lines.size();
        int m = 1;
        int t_cs = 8;
        int t_slice = 84;

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

void FCFS(std::deque<process> processList)
{
	//"run the processes"
	while(processList.size() > 0)
	{
		//set the process variable
		process p=processList.front();
		processList.pop_front();

		//run the process
		/*for(int i=0;i<process.;++i)
		{
			//
		}*/
	}
}

// make temporary "current" process
// make new queue of ready queue
// if arrival time <= elapsed time
void SJF(std::deque<process> processList, int t_cs)
{
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
                ioQueue->push_back(currentProcess);
            }

            busy = false;
        }

        if (!ioQueue.empty()){
            std::sort(ioQueue.begin(), ioQueue.end(), process::IOComp);
        }

    }
}

void roundRobin(const std::deque<process>& processList)
{
	//
}

void err(const char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}
