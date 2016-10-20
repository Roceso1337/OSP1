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

		std::vector<process> processList;
		process::parse(lines, processList);

        fd.close();
	}

    return 0;
}

void FCFS(const std::vector<process>& processList)
{
	std::queue<process> q;
	for(int i=0;i<processList.size();++i)
		q.push(processList[i]);

	//"run the processes"
	while(q.size() > 0)
	{
		//set the process variable
		process p=q.front();
		q.pop();

		//run the process
		for()
		{
			//
		}
	}
}

void SJF(const std::vector<process>& processList)
{
	//
}

void roundRobin(const std::vector<process>& processList)
{
	//
}

void err(const char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}
