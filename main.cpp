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

        //int n = lines.size();
        //int m = 1;
        int t_cs = 8;
        //int t_slice = 84;

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

// make temporary "current" process
// make new queue of ready queue
// if arrival time <= elapsed time
void SJF(std::deque<process> processList, int t_cs)
{
    int timeElapsed = 0;
    process *currentProcess = new process();
    std::deque<process> *readyQueue = new std::deque<process>; 
    std::deque<process> *ioQueue = new std::deque<process>;
    bool busy = false;
    bool firstProcess = true;

    std::cout << "time " << timeElapsed << "ms: Simulator started for SJF " 
        << queueToString(*readyQueue) << std::endl;

    while (1){
        for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
            process *newProc = new process(*itr);
            if (itr->getInitialArrivalTime() == timeElapsed){
                readyQueue->push_back(*newProc);
                std::cout << process::printTime(timeElapsed) << " Process " 
                    << newProc->getID() << " arrived " << queueToString(*readyQueue) << std::endl;
                processList.erase(itr);
				delete newProc;
            }

            if (itr == processList.end())
                break;
        }

        std::sort(readyQueue->begin(), readyQueue->end(), process::SJTComp);

        
        if (currentProcess !=  NULL){
        }
        
        if (busy && currentProcess->getCPUBurstTime() == currentProcess->getTimeRunning()){
            currentProcess->decrementNumBurstsLeft();

            if (currentProcess->getNumBurstsLeft() == 0){
                std::cout << process::printTime(timeElapsed) << " Process " << 
                   currentProcess->getID() << " terminated " << queueToString(*readyQueue) << std::endl; 
            } else {
                std::cout << process::printTime(timeElapsed) << " Process " << 
                    currentProcess->getID() << " completed a CPU burst; " << 
                    currentProcess->getNumBurstsLeft() << " to go " <<
                    queueToString(*readyQueue) << std::endl;

                int ioBlockTime = timeElapsed +  currentProcess->getIOTime();

                std::cout << process::printTime(timeElapsed) << " Process " <<
                    currentProcess->getID() << " blocked on I/O until time " <<
                    ioBlockTime << " " << queueToString(*readyQueue) << std::endl;

                if (readyQueue->empty())
                    firstProcess = true;
                
                currentProcess->setIOEnd(ioBlockTime);
                ioQueue->push_back(*currentProcess);
                std::sort(ioQueue->begin(), ioQueue->end(), process::IOComp);
            }

            busy = false;
        }

        if (!ioQueue->empty()){
            std::sort(ioQueue->begin(), ioQueue->end(), process::IOComp);
            process *io = new process(ioQueue->front());
            if (timeElapsed == io->getIOEnd()){
                if (io->getNumBurstsLeft() == 0){
                    std::cout << process::printTime(timeElapsed) << "Process " <<
                        io->getID() << " terminated " << queueToString(*readyQueue) << std::endl;
                } else {
                    io->setTimeRunning(0);
                    if (!busy && readyQueue->empty())
                        firstProcess = true;
                    readyQueue->push_back(*io);
                    std::sort(readyQueue->begin(), readyQueue->end(), process::SJTComp);
                    std::cout << process::printTime(timeElapsed) << "Process " <<
                        io->getID() << " completed I/O " << queueToString(*readyQueue) << std::endl;

                }
                ioQueue->pop_front();
            }
            delete io;
        }
        //can start a new process 
        if (!busy && !readyQueue->empty()){ 
            process *newProc = new process(readyQueue->front());
            *currentProcess = *newProc;
            busy = true; 
            readyQueue->pop_front(); 
            int start = 0; 
            if (firstProcess){ 
                start = timeElapsed + t_cs/2;
                firstProcess = false; 
            } else 
                start = timeElapsed + t_cs; 
            int end = start + currentProcess->getCPUBurstTime();
            currentProcess->setCpuBurstStart(start);
            currentProcess->setCpuBurstEnd(end);
        } else {
            if (timeElapsed == currentProcess->getCpuBurstStart()){
                std::cout << process::printTime(timeElapsed) << " Process " << currentProcess->getID()
                   << " started using the CPU " << queueToString(*readyQueue) << std::endl; 
            }
        } 

        if (readyQueue->empty() && ioQueue->empty() && !busy){
            std::cout << process::printTime(timeElapsed+4) << " Simulator ended for SJF" << std::endl;
            delete currentProcess;
            break;
        }

        if (currentProcess->getCpuBurstStart() <= timeElapsed){
            currentProcess->increaseTimeRunning();
        }

        timeElapsed++;
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
