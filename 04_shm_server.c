#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#define pathname "/home/lieryang/Desktop/daily/20231105"
#define proj_id 0x36
#define SIZE 4096

int main()
{
  key_t key=ftok(pathname,proj_id);//生成key
  if(key<0)
  {
    perror("ftok");
    return 1;
  }
  /* 0644 表示所有者具有读和写权限，属组和其他人只有读权限 */
  int shmid= shmget(key,SIZE,IPC_CREAT|IPC_EXCL|0644);//通过key创建出共享内存
  if(shmid<0)
  {
    perror("shmget");
    return 2;
  }
  /**
   * @param __shmflg：如果是0 就是读写模式，SHM_RD0NLY就是只读模式
  */
  char* mem=(char*)shmat(shmid,NULL,0);//挂接共享内存

  //开始通信
  while(1)
  {
    printf("client sent: %s\n",mem);
    // sleep(1);
  }
  shmdt(mem);//去关联共享内存
  shmctl(shmid,IPC_RMID,NULL);//释放共享内存
  return 0;
}