#ifndef __H264_FRAMED_LIVE_SOURCE_HH
#define __H264_FRAMED_LIVE_SOURCE_HH

#include "FramedSource.hh"
class H264FramedLiveSource : public FramedSource
{
public:
	static H264FramedLiveSource* createNew(UsageEnvironment& env);

protected:
	H264FramedLiveSource(UsageEnvironment& env);
	~H264FramedLiveSource();

private:
	virtual void doGetNextFrame();
	virtual unsigned maxFrameSize() const;

private:
	int m_shmid;//要映射的共享内存区标识符
	int m_semid;//定义信号量，用于实现共享内存的进程之间的互斥
	char *m_shm_addr;//共享内存映射到的地址
};

#endif //__H264_FRAMED_LIVE_SOURCE_HH