/*************************************************************************************************************************************************
 * @filename: pipe.c
 * @author: EryangLi
 * @version: 1.0
 * @date: Nov-6-2023
 * @brief:  1. 
 * 
 *          2.
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
#include <unistd.h>    
#include <stdlib.h>    
#include <string.h>    
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>    
    
int 
main(int argc, char *argv[]) {    
  int pipefd[2]={0};

  if(pipe(pipefd) !=0 )    
  {    
    perror("pipe error!");    
    return 1;    
  }    
  
  /**
   * pidfd[0] pidfd[1] 哪一个是读，哪一个是写呢？
   * 0下标：读取端  1下标：写入端    
  */
  printf("pipefd[0]: %d\n",pipefd[0]); //3    
  printf("pipefd[1]: %d\n",pipefd[1]); //4    
    
  /**
   * fork() 调用一次，返回两次
   * 在父进程返回子进程的PID,在子进程返回 0
  */
  if(fork() == 0) { /* 子进程 */
    pid_t pid = getpid();  // 获取当前进程的PID
    printf("The Child Process ID is %d\n", pid);

    printf ("child pipefd = %p\n", pipefd);

    /* 我们让父进程读取，子进程写入    */
    close(pipefd[0]);    
    
    const char *msg = "hello Linux";    
    while(1)    
    {    
      write(pipefd[1], msg, strlen(msg));//这里strlen(msg)+1 是不需要的，因为这是向管道写入管道也是文件，因为\0是C语言的标准，    
                                         //文件里面我们只关心内容，\0并不是文件内容    
      sleep(1);    
    }    
 
    exit(0);
  }
  
  /* 父进程 */
  pid_t pid = getpid();  // 获取当前进程的PID
  printf("The Parent Process ID is %d\n", pid);
  printf ("parent pipefd = %p\n", pipefd);
  close(pipefd[1]);
  while(1)
  {
    char buffer[64]={0};
    //返回值代表你读到的字节数，0表示文件结束啦，如果read的返回值是0，意味着子进程关闭文件描述符了，子进程关闭了就没人写了相当于读到文件结尾
    ssize_t s = read(pipefd[0], buffer, sizeof(buffer));
    if(s==0)
    {
      break;
    }
    else if(s>0)
    {
      buffer[s]=0;
      printf("chid say to father# %s\n", buffer);
    }
    else 
    {
      break;
    }
 
  }                                                                                                                                                                     
 
  return 0;
}