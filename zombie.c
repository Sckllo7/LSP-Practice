#include<unistd.h>
#include<stdlib.h>

int main(int argc, char* argv[])
{
	pid_t pid = 0;

	do
		pid = fork();
	while(pid);

	exit(0);

	return 0;
}
