/*************************************************************************************************************************************************
 * @filename: 02_named_pipes_client.c
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
#include<string.h>

#define MY_FIFO "./fifo"  /*我的管道就在当前路径下放着*/

int
main (int argc, char* argv[]) {

 //不需要创建一个fifo,只需要获取即可    
  int fd=open(MY_FIFO, O_WRONLY); //不需要O_CREAT，因为本来就存在    
  if(fd < 0)    
  {    
    perror("open");    
    return  1;    
  }    
  //业务逻辑    
  while(1) {    
    printf("请输入# ");    
    fflush(stdout);                                                                                                                                                     
    char buffer[64]={0};    
    //先把数据从标准输入拿到我们的client进程内部    
    ssize_t s=read(0, buffer, sizeof(buffer)-1); //键盘输入的时候，\n也是输入字符的一部分    
    if(s > 0) {    
      buffer[s-1]=0; //去掉键盘输入的\n    
      printf("%s\n", buffer);    
    
      //拿到了数据    
      write(fd, buffer, strlen(buffer));  //不要-1，管道也是文件    
    
      //读一条消息，把这条消息回显出来，并且发送给对方     
    }    
  }    
    
  close(fd);    
  return 0;    

}