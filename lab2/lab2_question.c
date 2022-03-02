#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int counter = 0;
	int child1 = fork();

	// if child == 0, then it is the child process
	if (child1 == 0)
	{

		for (counter = 0; counter < 10; counter++)
		{
			printf("child %d process, counter = %d\n", getpid(), counter);
			sleep(1);
		}
	}
	else // this is the parent process
	{
		// create a second child
		int child2 = fork();

		// if child2 == 0, the current process is the second child
		if (child2 == 0)
		{
			for (counter = 0; counter < 10; counter++)
			{
				printf("child %d process, counter = %d\n", getpid(), counter);
				sleep(1);
			}
		}
		else // parent process
		{
			for (counter = 0; counter < 10; counter++)
			{
				printf("parent %d process, counter = %d\n", getppid(), counter);
				sleep(1);
			}
		}
	}
}
