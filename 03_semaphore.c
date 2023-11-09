/*************************************************************************************************************************************************
 * @filename: 03_semaphore.c
 * @author: EryangLi
 * @version: 1.0
 * @date: Nov-8-2023
 * @brief:  
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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define FTOK_PATH "./"
#define FTOK_PROJID 0x22

/* 信号量操作的联合结构 */
union semun {
  int val; /*  */
  struct semid_ds *buf; /*  */
  unsigned int *arry; /*  */
};

/* 全局变量 */
union semun sem_union;

int semid;

/**
 * @brief: 信号量初始化
*/
int 
semaphore_init (void) {
  key_t key;
  int semid; /* 信号量的ID */
  union semun sem_union;
  sem_union.val = 0;   /* 将信号量的值设为0 */
  /* 获取IPC关键字key */
  key = ftok (FTOK_PATH, FTOK_PROJID);
  if (key < 0) {
    printf ("ftok() get key failure:%s\n", strerror(errno));
  }

  printf ("ftok() get key successfully!\n");

  /* 创建或获取信号量，信号量不存在则创建 */
  /**
   * @brief: 创建一个新的信号量集合，或者访问现有的集合。
   * @param key: ftok生成键值
   * @param nsems: 参数可以指定新的集合中应该创建的信号量的数目
   * @param semflg: 打开信号的方式
   *                IPC_CREAT: 如果内核中不存在这样的信号量集合，则把它创建出来。如果存在则获取该信号集合
   *                IPC_EXCL: 当与IPC_CREAT一起使用时，如果信号量集合早已存在，则操作将失败。
  */
  semid = semget (key, 1, IPC_CREAT | 0644);
  if (semid < 0) {
		printf("semget() get semid  failure:%s\n",strerror(errno));
		return -2;
  }

  /* 初始化信号集，设置信号量集中的一个单独的信号量的值，并使用该参数 */
  if(semctl(semid,0,SETVAL,sem_union) < 0) {
  printf("semctl() set initial value failure: %s\n", strerror(errno));
  }
  printf("semaphore get key_t[0x%x] and semid[%d]\n", key, semid);
  
  return semid;	
}

/**
 * @brief: 父进程进行P操作
*/
int 
semaphore_p (int semid) {
  struct sembuf  _sembuf;
  /* 第一个信号编号为0，最后一个为nsems-1 */
  _sembuf.sem_num = 0;
  /* 操作为负操作（P操作），semid ≥ |sem_op| 才能继续运行，否则阻塞 */
  _sembuf.sem_op  =  -1;
  /* 
   * 信号操作标识，程序结束时（正常退出或异常终止），保证信号值会被重设为semop()调用前的值。
   * 避免程序在异常情况下结束时未解锁锁定的资源，造成资源被永远锁定。造成死锁。 
   */
  _sembuf.sem_flg  =  SEM_UNDO;
  
  if(semop(semid,&_sembuf,1) < 0) {
    printf("semop  P  operator   failure:%s\n",strerror(errno));
    return -1;
  }
  return 0;
}


/**
 * @brief: 父进程进行V操作
*/
int 
semaphore_v (int semid){
  struct sembuf  _sembuf;
  _sembuf.sem_num = 0;		
  _sembuf.sem_op  =  1;		
  _sembuf.sem_flg  =  SEM_UNDO;	
  if(semop(semid,&_sembuf,1) < 0) {
    printf("semop V  operator   failure:%s\n",strerror(errno));
    return -1;
  }
  return 0;
}

/**
 * @brief: 删除信号量
*/
void 
semaphore_term (int semid) {
  if (semctl (semid, 0, IPC_RMID, sem_union) < 0) {
    printf("semctl() delete semaphore ID failure:%s\n",strerror(errno));
  }
  return;
}

int
main (int argc, char* argv[]) {

  pid_t pid;
  int i;
  int semid;

  if((semid = semaphore_init()) < 0) {
    printf("semaphore initial failure:%s\n", strerror(errno));
    return -1;
  }

  if ((pid = fork()) < 0) {
    printf("create child process failure:%s\n", strerror(errno));
    return -2;
  } else if (0 == pid) {
    printf("child process[%d] start running and do something now...\n", getpid());
    sleep(3);
    printf("child process done.\n");
    semaphore_v(semid);		//子进程进行V操作
    sleep(1);
    printf("child process exit now\n");
    exit(0);
  }
  printf("parent process P operator wait child process over.\n");
  semaphore_p(semid);		//子进程没进行v操作前，父进程在阻塞在此
  printf("parent process[%d] start running and do something now...\n", getppid());
  sleep(2);
  printf("parent process destroy semaphore and exit\n");
  semaphore_term(semid);

  return 0;
}

