//Daniel Sze
//Robert Carneiro
//Brendan Courson
#include "main.h"

int main(int argc, char *argv[])
{
	if(argc < 3) err("Invalid arguments\n");

	std::string inFname=argv[1];
	std::string outFname=argv[2];

    std::ifstream fd(inFname.c_str());
    std::string line;
    std::vector<std::string> lines;
    std::string fileOutput="";

	if(fd != NULL)
	{
        while (std::getline(fd, line)){
            lines.push_back(line);
        }

		std::deque<process> processList;
		process::parse(lines, processList);

		float avgCPUBurstTime=0;
		float avgWaitTime=0;
		float avgTurnAroundTime=0;
		int contextSwitches=0;
		int preemptions=0;

		//int n = lines.size();
        //int m = 1;
        int t_cs = 8;
        int t_slice = 84;

		schedule(processList, t_cs, t_slice, avgCPUBurstTime, avgWaitTime,
                avgTurnAroundTime, contextSwitches, preemptions, 2);

		char buffer[20000];
		sprintf(buffer, "Algorithm FCFS\n");
		sprintf(buffer, "%s-- average CPU burst time: %.2f ms\n", buffer, avgCPUBurstTime);
		sprintf(buffer, "%s-- average wait time: %.2f ms\n", buffer, avgWaitTime);
		sprintf(buffer, "%s-- average turnaround time: %.2f ms\n", buffer, avgTurnAroundTime);
		sprintf(buffer, "%s-- total number of context switches: %d\n", buffer, contextSwitches);
		sprintf(buffer, "%s-- total number of preemptions: %d\n", buffer, preemptions);

		avgCPUBurstTime=0;
		avgWaitTime=0;
		avgTurnAroundTime=0;
		contextSwitches=0;
		preemptions=0;

        schedule(processList, t_cs, t_slice, avgCPUBurstTime, avgWaitTime,
                avgTurnAroundTime, contextSwitches, preemptions, 0);

		sprintf(buffer, "%sAlgorithm SJF\n", buffer);
		sprintf(buffer, "%s-- average CPU burst time: %.2f ms\n", buffer, avgCPUBurstTime);
		sprintf(buffer, "%s-- average wait time: %.2f ms\n", buffer, avgWaitTime);
		sprintf(buffer, "%s-- average turnaround time: %.2f ms\n", buffer, avgTurnAroundTime);
		sprintf(buffer, "%s-- total number of context switches: %d\n", buffer, contextSwitches);
		sprintf(buffer, "%s-- total number of preemptions: %d\n", buffer, preemptions);

		avgCPUBurstTime=0;
		avgWaitTime=0;
		avgTurnAroundTime=0;
		contextSwitches=0;
		preemptions=0;

        schedule(processList, t_cs, t_slice, avgCPUBurstTime, avgWaitTime,
                avgTurnAroundTime, contextSwitches, preemptions, 1);

		sprintf(buffer, "%sAlgorithm RR\n", buffer);
		sprintf(buffer, "%s-- average CPU burst time: %.2f ms\n", buffer, avgCPUBurstTime);
		sprintf(buffer, "%s-- average wait time: %.2f ms\n", buffer, avgWaitTime);
		sprintf(buffer, "%s-- average turnaround time: %.2f ms\n", buffer, avgTurnAroundTime);
		sprintf(buffer, "%s-- total number of context switches: %d\n", buffer, contextSwitches);
		sprintf(buffer, "%s-- total number of preemptions: %d\n", buffer, preemptions);

		fileOutput+=buffer;

        fd.close();
	}

	std::ofstream fd2(outFname.c_str());

	if(fd2 != NULL)
	{
		fd2.write(fileOutput.c_str(), fileOutput.length());
		fd2.close();
	}

    return 0;
}

std::string intTOstring(int number)
{
   std::stringstream ss;
   ss<<number;
   return ss.str();
}

std::string floatTOstring(float number)
{
   std::stringstream ss;
   ss<<number;
   return ss.str();
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

void schedule(std::deque<process> processList, int t_cs, int t_slice, float& avgCPUBurstTime,
        float& avgWaitTime, float& avgTurnAroundTime, int& contextSwitches, int& preemptions, int rrFlag)
{
	int burstCount=0;
    int waitCount=0;
    int timeElapsed = 0;
    int initialProcesses = 0;
    process *currentProcess = new process();
    std::deque<process> *readyQueue = new std::deque<process>; 
    std::deque<process> *ioQueue = new std::deque<process>;
    bool busy = false;
    bool nonSwitch = true;
    int numNonSwitches = 0;
    int rrCounter = 0;
    std::string mode = "";

    if (rrFlag == 0)
        mode = "SJF";
    else if (rrFlag == 1)
        mode = "RR";
    else if (rrFlag == 2)
        mode = "FCFS";

    //gathering the total number of bursts
    for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
        burstCount += itr->getCPUBurstTime()*itr->getNumBursts();
    }

    std::cout << "time " << timeElapsed << "ms: Simulator started for " << mode << " " 
        << queueToString(*readyQueue) << std::endl;

    while (1){
        //check to see if any processes are able to arrive
        for (std::deque<process>::iterator itr = processList.begin(); itr != processList.end(); itr++){
            process *newProc = new process(*itr);

            if (itr->getInitialArrivalTime() == timeElapsed){
                readyQueue->push_back(*newProc);

                if (rrFlag == 0)
                    std::sort(readyQueue->begin(), readyQueue->end(), process::SJTComp);
                else
                    std::sort(readyQueue->begin(), readyQueue->end(), process::FCFSComp);

                std::cout << process::printTime(timeElapsed) << "Process " 
                    << newProc->getID() << " arrived " << queueToString(*readyQueue) << std::endl;
                initialProcesses++;
                processList.erase(itr);
				delete newProc;
            }
            
            if (itr == processList.end())
                break;
        }

        if (rrFlag == 0)
            std::sort(readyQueue->begin(), readyQueue->end(), process::SJTComp);

        //process finished
        if (busy && currentProcess->getCPUBurstTime() == currentProcess->getTimeRunning()){
            currentProcess->decrementNumBurstsLeft();

            //no more bursts left
            if (currentProcess->getNumBurstsLeft() == 0){
                std::cout << process::printTime(timeElapsed) << "Process " << 
                   currentProcess->getID() << " terminated " << queueToString(*readyQueue) << std::endl; 
            }
            //process still has some bursts left, block it
            else { 
                initialProcesses++;
                std::cout << process::printTime(timeElapsed) << "Process " << 
                    currentProcess->getID() << " completed a CPU burst; " << 
                    currentProcess->getNumBurstsLeft() << " to go " <<
                    queueToString(*readyQueue) << std::endl;

                int ioBlockTime = timeElapsed +  currentProcess->getIOTime();

                std::cout << process::printTime(timeElapsed) << "Process " <<
                    currentProcess->getID() << " blocked on I/O until time " <<
                    ioBlockTime << "ms " << queueToString(*readyQueue) << std::endl;

                if (!busy && readyQueue->empty()){
                    nonSwitch = true;
                }
                
                currentProcess->setIOEnd(ioBlockTime);
                ioQueue->push_back(*currentProcess);
                std::sort(ioQueue->begin(), ioQueue->end(), process::IOComp);
            }
            
            rrCounter = 0;
            busy = false;
        }

        if (busy && (rrFlag == 1) && rrCounter%t_slice==0 && rrCounter != 0){
            if (!readyQueue->empty()){
                process *newProc = new process(readyQueue->front());
                readyQueue->push_back(*currentProcess);
                int timeLeft = currentProcess->getCPUBurstTime() - currentProcess->getTimeRunning();

                std::cout << process::printTime(timeElapsed) << "Time slice expired; process " <<
                    currentProcess->getID() << " preempted with " << timeLeft << "ms to go " 
                    << queueToString(*readyQueue) << std::endl;

                busy = false;
                rrCounter = 0;
                preemptions++;
                delete newProc;
            } else {
                std::cout << process::printTime(timeElapsed) << "Time slice expired; no preemption" <<
                   " because ready queue is empty " << queueToString(*readyQueue) << std::endl;
                rrCounter = 0;
            }
        }

        //check to see if any IO blocks are finished
        if (!ioQueue->empty()){
            std::sort(ioQueue->begin(), ioQueue->end(), process::IOComp);
            process *io = new process(ioQueue->front());
            if (timeElapsed == io->getIOEnd()){
                if (io->getNumBurstsLeft() == 0){
                    std::cout << process::printTime(timeElapsed) << "Process " <<
                        io->getID() << " terminated " << queueToString(*readyQueue) << std::endl;
                }
                else {
                    io->setTimeRunning(0);

                    if (!busy && readyQueue->empty()){
                        nonSwitch = true;
                    }

                    readyQueue->push_back(*io);
                    
                    if (rrFlag == 0)
                        std::sort(readyQueue->begin(), readyQueue->end(), process::SJTComp);

                    std::cout << process::printTime(timeElapsed) << "Process " <<
                        io->getID() << " completed I/O " << queueToString(*readyQueue) << std::endl;

                }
                ioQueue->pop_front();
            }
            delete io;
        }

        //can load a new process 
        if (!busy && !readyQueue->empty()){ 
            process *newProc = new process(readyQueue->front());
            *currentProcess = *newProc;
            busy = true; 
            readyQueue->pop_front(); 
            int start = 0; 

            if (nonSwitch){ 
                numNonSwitches++;
                start = timeElapsed + t_cs/2;
                nonSwitch = false; 
            }
            else 
                start = timeElapsed + t_cs; 

            int end = start + currentProcess->getCPUBurstTime();
            currentProcess->setCpuBurstStart(start);
            currentProcess->setCpuBurstEnd(end);
            rrCounter = 0;
        }
        // can start a process
        else {
            if (timeElapsed == currentProcess->getCpuBurstStart()){
                contextSwitches++;
                std::cout << process::printTime(timeElapsed) << "Process " << currentProcess->getID()
                   << " started using the CPU " << queueToString(*readyQueue) << std::endl; 
            }
        } 

        for (std::deque<process>::iterator itr = readyQueue->begin(); itr != readyQueue->end(); itr++){
            if (currentProcess->getID() != itr->getID())
                waitCount++;
        }

        if (readyQueue->empty() && ioQueue->empty() && !busy){
            timeElapsed += t_cs/2;
            std::cout << process::printTime(timeElapsed) << "Simulator ended for " << mode << std::endl;
            delete currentProcess;
            delete readyQueue;
            delete ioQueue;
            break;
        }

        if (currentProcess->getCpuBurstStart() <= timeElapsed){
            currentProcess->increaseTimeRunning();
            rrCounter++;
        }

        timeElapsed++;
    }

    avgWaitTime = (float)waitCount/(float)(contextSwitches-preemptions);
    avgCPUBurstTime = (float)burstCount/(float)(contextSwitches-preemptions);
    avgTurnAroundTime = ((float)burstCount + (float)waitCount +
            ((float)contextSwitches*(float)t_cs - 0.5*t_cs*numNonSwitches)
            )/(float)(contextSwitches-preemptions);
} 

void err(const char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}
