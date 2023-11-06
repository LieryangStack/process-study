/*************************************************************************************************************************************************
 * @filename: 01_fork.c
 * @author: EryangLi
 * @version: 1.0
 * @date: Nov-6-2023
 * @brief:  对应 01_fork详解.md
 *          1. 学习fork函数特性
 *          2. getppid（）和 getpid（）函数作用
 * @note: 
 * @history: 
 *          1. Date:
 *             Author:
 *             Modification:
 *      
 *          2. ..
 *************************************************************************************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int 
main(int argc, char* argv[]) {
	int i = 0;
  for (i = 0; i < 2; i++) {
    pid_t fpid = fork();
    if (fpid == 0 ){ /* 子进程 */
      printf ("i = %d Child %4d->%4d %4d\n", i, getppid(), getpid(), fpid);
    } else if(fpid > 0) {  /* 父进程返回子进程PID */
      printf ("i = %d Parent %4d->%4d %4d\n", i, getppid(), getpid(), fpid);
    } else {
      perror ("creat process error\n");
    }
  }
	return 0;
}
