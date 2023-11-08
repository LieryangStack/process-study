/*************************************************************************************************************************************************
 * @filename: 02_named_pipes_server.c
 * @author: EryangLi
 * @version: 1.0
 * @date: Nov-8-2023
 * @brief:  对应 02_管道.md
 *          1. 
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


#include<stdio.h>    
#include<sys/stat.h>    
#include<sys/types.h>    
#include<fcntl.h>        
#include<unistd.h>

#define MY_FIFO "./fifo"  /*我的管道就在当前路径下放着*/

int
main (int argc, char* argv[]) {

  if (mkfifo (MY_FIFO, 0666) < 0) {
    perror ("mkfifo");
    return -1;
  }

  int fd = open(MY_FIFO, O_RDONLY);
  if(fd < 0)                         
  {           
    perror("open");
    return 2;               
  }                                                                                                                 
                 
  //业务逻辑， 可以进行对应的读写
  while(1) {                              
    char buffer[64]={0};                 
    ssize_t s=read(fd, buffer, sizeof(buffer)-1); // 减1的目的就是不想让他把缓冲区打满，因为要把它当做字符串来看
    if(s > 0) {                  
      //success
      buffer[s]=0; // 字符串结束
      printf("client# %s\n", buffer);
            
    } else if(s == 0) {    
      //peer close 
      printf("client quit ...\n");
      break;    
    } else 
    {
      //error
      perror("read");
      break;
    }
  }
 
  close(fd);

  return 0;
}