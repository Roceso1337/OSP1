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
    process* currentProcess = new process();
    std::deque<process> readyQueue;
    bool busy = false;

    std::cout << "time " << timeElapsed << "ms: Simulator started for SJF " << std::endl;

    while (1){
        //can start a new process
        if (!busy && currentProcess == NULL){
            process p = processList.front();
            process currentProcess = p;
            busy = true;
            std::cout << "TIME PLACEHOLDER: Process " << p.getID() 
                << " arrived " << queueToString(readyQueue) << std::endl;
        }    
    }
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
