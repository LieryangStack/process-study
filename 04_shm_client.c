//头文件 包含用ftok()创建的key 需要的参数Pathname和proj_id
#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#define pathname "/home/lieryang/Desktop/daily/20231105"
#define proj_id 0x36
#define SIZE 4096

//client.c
// #include"serve.h"
int main()
{
  key_t key=ftok(pathname,proj_id);
  if(key<0)
  {
    perror("ftok");
  }

  int shmid=shmget(key,SIZE,IPC_CREAT);//创建
  char* mem=(char*)shmat(shmid,NULL,0);//挂接

  //开始通信
  
  int i=0;
  while(1)
  {
    mem[i] = 'A'+ i;//这里是直接对开辟出来的共享内存进行使用 对其进行赋值操作  然后可以让另一个进程打印出来
    i++;
    mem[i]='\0';
    // sleep(1);
  }
  shmdt(mem);//去关联 
  shmctl(shmid,IPC_RMID,NULL);//释放
  return 0;
}
