#include "H264LiveVideoServerMediaSubssion.hh"
#include "H264FramedLiveSource.hh"
#include "H264VideoStreamFramer.hh"

H264LiveVideoServerMediaSubssion* H264LiveVideoServerMediaSubssion::createNew(UsageEnvironment& env, char const* fileName, Boolean reuseFirstSource)
{
    return new H264LiveVideoServerMediaSubssion(env, fileName, reuseFirstSource);
}

H264LiveVideoServerMediaSubssion::H264LiveVideoServerMediaSubssion(UsageEnvironment& env, char const* fileName, Boolean reuseFirstSource)
: H264VideoFileServerMediaSubsession(env, fFileName, reuseFirstSource)
{
}

H264LiveVideoServerMediaSubssion::~H264LiveVideoServerMediaSubssion()
{
}

FramedSource* H264LiveVideoServerMediaSubssion::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate)
{
    printf("createNewStreamSource %s\n", fFileName);

    estBitrate = 1000; // kbps, estimate

    // Create the video source:
    H264FramedLiveSource* liveSource = H264FramedLiveSource::createNew(envir());
    if (liveSource == NULL) return NULL;

    // Create a framer for the Video Elementary Stream:
    return H264VideoStreamFramer::createNew(envir(), liveSource);
}