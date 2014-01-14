#include<unistd.h>
#include<sys/wait.h>
#include<malloc.h>
#include<stdlib.h>

pid_t* makeChilds(pid_t* child, const int PROC);

int main(int argc, char* argv[])
{
	pid_t* child = NULL;
	const pid_t PPID = getpid();
	const int PROC = atoi(*(argv + 1));
	int status = 0;

	if (PROC)
		child = makeChilds(child, PROC);

	if (PPID != getpid() || !PROC)
		exit(0);

	sleep(30);

	free(child);

	return 0;
}

pid_t* makeChilds(pid_t* child, const int PROC)
{
	int i;

	child = (pid_t*) malloc(PROC * sizeof(pid_t));

	for (i = 0; i < PROC; i++)
		*(child + i) = 0;

	*child = fork();
	for (i = 1; i < PROC; i++)
		if (*(child + (i - 1)))
			*(child + i) = fork();

	return child;
}
