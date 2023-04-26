#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "sem_comm.h"

//#include <string>
//using namespace std;


 
pid_t pid;
int shmid;//要映射的共享内存区标识符
int semid;//定义信号量，用于实现共享内存的进程之间的互斥
char *shm_addr;//共享内存映射到的地址

typedef struct share_mem
{
	int len;
	unsigned char data[1024*512];
}share_mem;

void function_init()
{
	//创建一个信号量
	semid = semget(ftok("/home",2), 1, 0666|IPC_CREAT);
	sem_init(semid, 1);//初始值为1
	
	//创建共享内存
	shmid = shmget(80000, sizeof(share_mem), 0666|IPC_CREAT);
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
	int loop = 10000;

	function_init();
	
	//映射共享内存
	shm_addr = shmat(shmid, 0, 0);

	pch_shmem = ( struct share_mem* )shm_addr;
	pch_shmem->len = 0;

	fp = fopen("test.264", "rb");
	if (fp)
	{
		while (loop--)
		{
			pos = 0;
			fseek(fp, 0, SEEK_SET);
			while(1)
			{
				sem_p(semid);
				
#if 1
				if (pch_shmem->len > 0)
				{
					//printf("wait write:%d,pos:%d\n",pch_shmem->len,pos);
					sem_v(semid);
					//usleep(1000);
					continue;
				}
#endif

				pch_shmem->len = fread(pch_shmem->data, sizeof(unsigned char), 1024*512, fp);

				printf("writelen:%d,pos:%d\n",pch_shmem->len,pos);

				pos += pch_shmem->len;

				sem_v(semid);

				if(pch_shmem->len<=0)
				{
					perror("readlen \n");
					break;
				}


				usleep(25*1000);
			}
		}
		

		fclose(fp);
	}
	else
	{
		printf("error:open file %s failed!", "test.264");
	}

			
	function_end();	

	exit(0);
}