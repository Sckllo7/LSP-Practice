#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<malloc.h>
#include"/usr/include/linux/inotify.h"

#define BUF_LEN 100

pid_t* makeChilds(pid_t* child, const int PROC);

int mining(int sel)
{
	char buf[150];
	char* logPath = "/var/log/secure*";
	char try[8] = {0,};
	char* logCopy = "mining";
	char startDate[16];
	char lastDate[16];
	int maxLine = 0;
	int tmp = 10;
	int swit = 9;
	int i;
	FILE *fp;

	if (!sel)
		strcpy(try, "Failed");
	else	
		strcpy(try, "Accepted");

	sprintf (buf, "rm -rf __buf* maxLine %s", logCopy);
	system (buf);

	sprintf (buf, "sudo cat %s | grep %s > %s", logPath, try, logCopy);
	system (buf);
	sprintf (buf, "wc -l %s > maxLine", logCopy);
	system (buf);

	sprintf (buf, "sed 's/  / 0/g' ./%s > __buf0__", logCopy);
	system (buf);

	sprintf (buf, "sort -M __buf0__ > %s", logCopy);
	system (buf);

	fp = fopen (logCopy, "r");
	fgets (startDate, 16, fp);
	fclose(fp);

	fp = fopen ("maxLine","r");
	fscanf (fp, "%d", &maxLine);
	fclose(fp);

	for (i = 1; i < maxLine + 1; i++)
	{
		sprintf (buf, "sed -n '%dp' %s > __buf__", i, logCopy);
		system (buf);
	
		fp = fopen ("__buf__","r");

		fgets(lastDate, 16, fp);

		fseek (fp, 3, SEEK_CUR);

		fscanf (fp, "%d", &tmp);
		fclose (fp);

		if (tmp < 10)
			swit++;

		sprintf (buf, "sudo cat __buf__ | cut -f %d -d \" \" >> __buf2__", swit);
		system (buf);
		sprintf (buf, "sudo cat __buf__ | cut -f %d -d \" \" > __buf2.1__", swit);
		system (buf);

		fopen ("__buf2.1__", "r");
		fscanf (fp, " %s", buf);
		fclose (fp);

		if (!sel)
			if (!(strcmp(buf, "invalid")))
				swit += 2;
		swit += 2;

		sprintf (buf, "sudo cat __buf__ | cut -f %d -d \" \" >> __buf3__", swit);
		system (buf);

		swit = 9;
		tmp = 10;

		if (!(i % 100))
		{
			system ("clear;echo -------------User ID---------------------");
			system("cat __buf2__ | sort | uniq -c | sort -g");
			system ("echo ------------IP Address-------------------");
			system("cat __buf3__ | sort | uniq -c | sort -g");

			sprintf (buf, "echo '       -- %f%% ( %d / %d )'", i / (float)maxLine * 100, i, maxLine);
			system (buf);

			sprintf (buf, "echo '       -- ( %s )  ~  ( %s )'", startDate, lastDate);
			system (buf);
		}
	}
	
	system ("clear; echo -------------User ID---------------------");
	system("cat __buf2__ | sort | uniq -c | sort -g");
	system ("echo ------------IP Address-------------------");
	system("cat __buf3__ | sort | uniq -c | sort -g");
	sprintf (buf, "echo '       -- ( %s )  ~  ( %s )'", startDate, lastDate);
	system (buf);
	system ("echo '       -- Complete!!'");

	sprintf (buf, "rm -rf __buf__ __buf2__ __buf3__ __buf2.1__ maxLine %s", logCopy);
	system (buf);

	return 0;
}

int inot(char* notifyPath)
{
	int fd;
	int wd;

	char buf[BUF_LEN] __attribute__((aligned(4)));
	ssize_t len, i = 0;
	struct inotify_event *event;
	fd = inotify_init();

	while(1)
	{
		i = 0;

		if (fd == -1)
		{
			perror("inotify_init");
			return 0;
		}

		wd = inotify_add_watch (fd, notifyPath, IN_ALL_EVENTS);

		if (wd == -1)
		{
			perror ("inotify_add_watch");
			return 0;
		}

		len = read (fd, buf, BUF_LEN);

		while(i < len)
		{
			event = (struct inotify_event *) &buf[i];

			//printf ("wd = %d, mask = %d, cookie = %d, len = %d, dir = %s\n", event -> wd, event -> mask, event -> cookie, event -> len, (event -> mask & IN_ISDIR) ? "yes" : "no");
			if (event -> len)
				printf ("name = %s\n", event -> name);

			i += sizeof (struct inotify_event) + event -> len;
		}
	}
	return 1;
}

int realTimeMonitoring()
{
	FILE *fp;
	int tmp = 10;
	int swit = 9;
	char buf[150];
	char buf2[16];
	char buf3[16];
	
	system ("rm -rf __rtm* rtm_maxLine");
	system ("tail -f /var/log/secure > __rtm.0__");

	while(1)
	{

	system ("cat __rtm.0__ | grep Failed > __rtm.1__");

	fp = fopen("__rtm.1__","r");

	if(!fgetc(fp))
	{
		fclose(fp);
		continue;
	}

	fclose (fp);

	system ("wc -l __rtm.0__ > rtm_maxLine");
	system ("sed 's/  / 0/g' ./__rtm.1__ > __rtm.2__");
	system ("echo 0 > __rtm.1__");
	system("sort -M __rtm.2__ > __rtm.3__");

	fp = fopen ("__rtm.3__","r");
	
	fseek (fp, 3, SEEK_CUR);

	fscanf (fp, "%d", &tmp);
	fclose (fp);

	if (tmp < 10)
		swit++;

	sprintf (buf, "sudo cat __rtm.3__ | cut -f %d -d \" \" > __rtm.3.1__", swit);
	system (buf);

	fopen ("__rtm.3.1__", "r");
	fscanf (fp, " %s", buf);
	fclose (fp);

	swit += 2;

	if (!(strcmp(buf, "invalid")))
		swit += 2;

	sprintf (buf, "sudo cat __rtm.3__ | cut -f %d -d \" \" > __rtm.3.2__", swit);
	system (buf);

	swit = 9;

	fp = fopen ("__rtm.3.1__","r");
	fscanf (fp, "%s", buf2);
	fclose (fp);

	fp = fopen ("__rtm.3.2__","r");
	fscanf (fp, "%s", buf3);
	fclose (fp);

	sprintf (buf, "echo %s is attacked me from %s", buf2, buf3);
	system(buf);

	}

	return 0;
}

int zombies(int num, int t)
{
	pid_t* child = NULL;
	const pid_t PPID = getpid();
//	int swit = 2;
//	int status = 0;

	child = makeChilds(child, num);

	if (PPID != getpid())
		exit(0);
/*
	if (PPID != getpid() || !PROC)
		execvp(*(argv + swit), (argv + swit));

	while(wait(&status) > 0);
*/
	sleep(t);
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

int main(int argc, char* argv[])
{
	char sel;

	if (argv[1][1] == 'm')
		mining((int)argv[2][0] - '0');

	else if (argv[1][1] == 't')
		inot(*(argv + 2));

	else if (argv[1][1] == 'z')
		zombies(atoi(*(argv + 2)), atoi(*(argv + 3)));
	else
	{
		printf ("m : mining (0 : Failed, 1 : Accepted)\n");
		printf ("t : monitoring (logPath) \n");
		printf ("z : zombies (number of zombies, time)\n");
	}

	return 0;
}
