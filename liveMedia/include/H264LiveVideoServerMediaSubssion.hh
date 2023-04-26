#ifndef __H264_LIVE_VIDEO_SERVER_MEDIA_SUBSSION_HH
#define __H264_LIVE_VIDEO_SERVER_MEDIA_SUBSSION_HH

#include "H264VideoFileServerMediaSubsession.hh"

class H264LiveVideoServerMediaSubssion : public H264VideoFileServerMediaSubsession {
public:
  static H264LiveVideoServerMediaSubssion*
  createNew(UsageEnvironment& env, char const* fileName, Boolean reuseFirstSource);

protected:
  H264LiveVideoServerMediaSubssion(UsageEnvironment& env,
				      char const* fileName, Boolean reuseFirstSource);
      // called only by createNew();
  virtual ~H264LiveVideoServerMediaSubssion();

protected: // redefined virtual functions
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,unsigned& estBitrate);  
};

#endif //__H264_LIVE_VIDEO_SERVER_MEDIA_SUBSSION_HH