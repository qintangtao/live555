#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sem_comm.h"

 
pid_t pid;
int shmid;//要映射的共享内存区标识符
int semid;//定义信号量，用于实现共享内存的进程之间的互斥
char *shm_addr;//共享内存映射到的地址

typedef struct share_mem
{
	int len;
	int idx;
	unsigned char data[1024*512];
}share_mem;

void function_init()
{
	//创建一个信号量
	semid = semget(ftok("/home",2), 1, 0666|IPC_CREAT);
	sem_init(semid, 1);//初始值为1
	
	//创建共享内存
	shmid = shmget(80000, sizeof(share_mem), 0666);
	if (shmid < 0) {
		perror("shmget");
		exit(1);
	} else {
		printf("create shard-memory shmid: %d\n", shmid);
	}
	
}
 
void function_end()
{
	//删除信号量
	sem_del(semid);	
	//解除共享内存映射
	if ((shmdt(shm_addr)) < 0) {
		perror("shmdt");
		exit(1);
	} 
		
	//删除共享内存IPC_RMID	删除共享内存段
	if (shmctl(shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	} else {
		printf("Delete shared-memory\n");
	}
		
}

int main()
{
	struct share_mem* pch_shmem;
	FILE *fp;
	int pos;

	function_init();
	
	//映射共享内存
	shm_addr = shmat(shmid, 0, 0);

	pch_shmem = ( struct share_mem* )shm_addr;

	while(1)
	{
		sem_p(semid);//信号值减一
		
		printf("readlen:%d, idx:%d\n",pch_shmem->len, pch_shmem->idx);

		sem_v(semid);

		//usleep(60*1000);
	}

			
	//function_end();	

	exit(0);
}