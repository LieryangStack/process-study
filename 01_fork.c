#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int 
main(int argc, char* argv[]) {
	
  int n = 1;

	pid_t pid = fork();

	if(pid == -1)
	{
		perror("fork err");
		return -1;	
	}
	if(pid == 0) /*子进程*/
	{
    //sleep(1);
    printf ("n = %d\n", n);
		printf("child process = %d\n", getpid());	
        /*	test2
        while(1)
        {
            printf("fork process\n");
            sleep(1);
        }
        */
	}
	if(pid > 0) {
    
    n = 11;

		printf("parent process = %d\n", getpid());		


        /*	test1
        sleep(1);
        */
        /*	test2
        while(1)
        {
        	sleep(1);
        }
        */
	}

	printf("=== process end ===\n");
	return 0;
}
