#include "main.h"

int main(int argc, char *argv[])
{
	if(argc < 2) err("Invalid arguments\n");

	std::string fname=argv[1];

	FILE* fd=fopen(fname.c_str(), "r");

	if(fd != NULL)
	{
		//get the file size
		fseek(fd, 0, SEEK_END);
		long fSize=ftell(fd);
		fseek(fd, 0, SEEK_SET);

		//read in the data
		char* buffer=new char[fSize+1];
		fread(buffer, 1, fSize, fd);
		std::string fileContents=buffer;
		delete buffer;

		std::vector<process> processList;
		process::parse(fileContents, processList);

		//close the fd
		fclose(fd);
	}

    return 0;
}

void err(const char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(EXIT_FAILURE);
}