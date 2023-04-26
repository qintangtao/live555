#include "H264FramedLiveSource.hh"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/shm.h>
#include <sys/sem.h>

extern "C" {
	#include "sem_comm.h"
}


#define REV_BUF_SIZE  (1024*512)

typedef struct share_mem
{
	int len;
	unsigned char data[REV_BUF_SIZE];
}share_mem;

H264FramedLiveSource* H264FramedLiveSource::createNew(UsageEnvironment& env)
{
    H264FramedLiveSource* newSource = new H264FramedLiveSource(env);
    return newSource;
}

#define ENABLED_SEM

H264FramedLiveSource::H264FramedLiveSource(UsageEnvironment& env)
: FramedSource(env)
{
#ifdef ENABLED_SEM
	//创建一个信号量
	m_semid = semget(ftok("/home",2), 1, 0666|IPC_CREAT);
	sem_init(m_semid, 1);
	
	//创建共享内存
	m_shmid = shmget(80000, sizeof(share_mem), 0666);
	if (m_shmid < 0) {
		perror("shmget");
		exit(1);
	} else {
		printf("create shard-memory shmid: %d\n", m_shmid);
	}

	//映射共享内存
	m_shm_addr = (char *)shmat(m_shmid, 0, 0);
#endif	

	printf("Create H264FramedLiveSource\n");
}

H264FramedLiveSource::~H264FramedLiveSource()
{
	printf("Delete H264FramedLiveSource\n");

#ifdef ENABLED_SEM	
#if 0
	//删除信号量
	sem_del(m_semid);	
	
	//解除共享内存映射
	if ((shmdt(m_shm_addr)) < 0) {
		perror("shmdt");
		exit(1);
	} 
		
	//删除共享内存IPC_RMID	删除共享内存段
	if (shmctl(m_shmid, IPC_RMID, NULL) == -1) {
		perror("shmctl");
		exit(1);
	} else {
		printf("Delete shared-memory\n");
	}
#endif	

#endif	
}

unsigned H264FramedLiveSource::maxFrameSize() const
{
    return REV_BUF_SIZE;
}

void H264FramedLiveSource::doGetNextFrame()
{
#ifdef ENABLED_SEM
    struct share_mem* pch_shmem;

    pch_shmem = ( struct share_mem* )m_shm_addr;

	while(1)
	{
		sem_p(m_semid);

#if 1
		if (pch_shmem->len == 0) {
			printf("wait read len:%d, fMaxSize:%d\n",pch_shmem->len, fMaxSize);
			sem_v(m_semid);
			continue;
		}
#endif		
		printf("read len:%d, fMaxSize:%d\n",pch_shmem->len, fMaxSize);

	    fFrameSize = pch_shmem->len > fMaxSize ? fMaxSize : pch_shmem->len;

	    for (int i=0; i< fFrameSize; i++)
			fTo[i] = pch_shmem->data[i];

#if 1
		pch_shmem->len = 0;
#endif
		sem_v(m_semid);
		break;
	}
    

#endif	

	nextTask() = envir().taskScheduler().scheduleDelayedTask(0,(TaskFunc*)FramedSource::afterGetting, this);
	//FramedSource::afterGetting(this);
}