#ifndef IMAGE_APPLY_CHANNEL_H
#define IMAGE_APPLY_CHANNEL_H

#include "imageapply.h"

class _HG_IMAGE_PROCESS_API CImageApplyChannel : public CImageApply
{
public:

	typedef enum channel
	{
		Red,
		Green,
		Blue,
		Invalid,
		Except_Red,
		Except_Green,
		Except_Blue
	}Channel;

public:
	
	CImageApplyChannel();

	CImageApplyChannel(Channel channel);

	virtual ~CImageApplyChannel(void);

	virtual void apply(cv::Mat& pDib,int side);

    void setChannel(Channel channel) { m_channel = channel; }//添加
private:

	void except_channel(const cv::Mat& src, cv::Mat& dst, int channel);

private:

	Channel m_channel;
};

#endif // !IMAGE_APPLY_CHANNEL_H
