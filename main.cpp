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

        fd.close();
	}

    return 0;
}

std::string queueToString(std::queue<process> queue){
    if (queue.empty()){
        return "[Q empty]";
    }
    std::string queueString = "[";

    while (!queue.empty()){
        queueString += queue.front().getID();
        queue.pop();

        if (!queue.empty())
            queueString += " ";
    }
    
    queueString += "]";
    return queueString;
}

void FCFS(std::queue<process> processList)
{
	//"run the processes"
	while(processList.size() > 0)
	{
		//set the process variable
		process p=processList.front();
		processList.pop();

		//run the process
		/*for(int i=0;i<process.;++i)
		{
			//
		}*/
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
