#ifndef __SEM_COMM_H__
#define __SEM_COMM_H__

/**
*信号量相关的函数调用接口复杂，将它们封装成二维单个
*信号量的几个基本函数。
*信号量初始化函数init_sem()
*P操作加一sem_p()
*V操作减一sem_v()
*从系统中删除信号量del_sem()
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>



extern int sem_init(int sem_id, int init_value);
extern int sem_p(int sem_id);
extern int sem_v(int sem_id);
extern int sem_del(int sem_id);

#endif //__SEM_COMM_H__