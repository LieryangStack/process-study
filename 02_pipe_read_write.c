/*************************************************************************************************************************************************
 * @filename: 02_pipe_read_write.c
 * @author: EryangLi
 * @version: 1.0
 * @date: Nov-8-2023
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
#include <unistd.h>    
#include <stdlib.h>    
#include <string.h>    
#include <sys/types.h>    
#include <sys/stat.h>    
#include <fcntl.h>    
#include <assert.h>
#include <sys/wait.h>

int
main (int argc, char *argv[]) {

  int fds[2];
  /**
   * @param fds[0] 用于管道读端
   * @param fds[1] 用于管道写端
   * @return 调用成功返回 0，否则返回 -1
  */
  int n = pipe (fds);
  assert (n == 0);

  pid_t id = fork();
  assert (id >= 0);

  if (id == 0) { /* 子进程 */
    close (fds[0]); /*子进程关闭读取*/
    const char* s = "你好，我是子进程，正在进行通信";
    int cnt = 0;
    for (cnt = 0; cnt < 10; cnt++) {
      char buffer[1024];
      snprintf (buffer, sizeof(buffer), "child -> parent say: %s cnt = %d", \
                s, cnt);
      write (fds[1], buffer, strlen(buffer));
      /*每秒写一次*/
      sleep (1);
    }
    close (fds[1]);
    exit (0);
  }

  /* 父进程 */
  close (fds[1]); /*父进程关闭写入端，既父进程进行读取*/
  while (1) {
    char buffer[1024];
    ssize_t s = read (fds[0], buffer, sizeof(buffer) -1 );
    if (s > 0) {
      buffer[s] = 0;
      printf ("Get  Message : %s | mypid = %d\n", buffer, getpid());
    }
    if (buffer[s-1] == '9')
      break;
  }
  n = waitpid (id, NULL, 0);
  assert (n == id);
  close (fds[0]);


  return 0;
}