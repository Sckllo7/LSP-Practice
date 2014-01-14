#include<unistd.h>
#include<sys/wait.h>
#include<malloc.h>
#include<stdlib.h>

#define NO_OF_ZOMBIES 300000

int makeChilds(pid_t* child);

int main(int argc, char* argv[])
{
	pid_t child[NO_OF_ZOMBIES] = {0,};
	const pid_t PPID = getpid();
	int status = 0;

	makeChilds(child);

	if (PPID != getpid())
		exit(0);

	sleep(30);

	return 0;
}

int makeChilds(pid_t* child)
{
	int i;

	for (i = 0; i < NO_OF_ZOMBIES; i++)
		child[NO_OF_ZOMBIES] = 0;

	*child = fork();
	for (i = 1; i < NO_OF_ZOMBIES; i++)
		if (*(child + (i - 1)))
			*(child + i) = fork();
	return 0;
}
