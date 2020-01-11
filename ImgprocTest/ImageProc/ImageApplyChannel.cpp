#include "ImageApplyChannel.h"

CImageApplyChannel::CImageApplyChannel()
	: m_channel(Invalid)
{
}

CImageApplyChannel::CImageApplyChannel(Channel channel)
	: m_channel(channel)
{
}

CImageApplyChannel::~CImageApplyChannel(void)
{
}

void CImageApplyChannel::apply(cv::Mat& pDib,int side)
{
#ifdef LOG
	FileTools::write_log("imgprc.txt", "enter CImageApplyChannel apply");
#endif // LOG
	cv::Mat dst(pDib.rows, pDib.cols, CV_8UC1);
	int fromTo[2] = { 0, 0 };
	switch (m_channel)
	{
	case Red:
		fromTo[0] = 2;
		cv::mixChannels(&pDib, 1, &dst, 1, fromTo, 1);
		break;
	case Green:
		fromTo[0] = 1;
		cv::mixChannels(&pDib, 1, &dst, 1, fromTo, 1);
		break;
	case Blue:
		fromTo[0] = 0;
		cv::mixChannels(&pDib, 1, &dst, 1, fromTo, 1);
		break;
	case Except_Red:
		except_channel(pDib, dst, 2);
		break;
	case Except_Green:
		except_channel(pDib, dst, 1);
		break;
	case Except_Blue:
		except_channel(pDib, dst, 0);
		break;
	default:
		break;
	}
	pDib.release();
	pDib = dst;
#ifdef LOG
	FileTools::write_log("imgprc.txt", "exit CImageApplyChannel apply");
#endif // LOG
}

void CImageApplyChannel::except_channel(const cv::Mat & src, cv::Mat & dst, int channel)
{
	int rows = src.total();
	cv::Mat src_temp(rows, 3, CV_8UC1, src.data);
	cv::Mat dst_temp(rows, 1, CV_8UC1, dst.data);

	cv::Mat temp1, temp2;
	switch (channel)
	{
	case 0:
		temp1 = src_temp(cv::Rect(1, 0, 1, rows));
		temp2 = src_temp(cv::Rect(2, 0, 1, rows));
		cv::addWeighted(temp1, 0.84, temp2, 0.16, 0, dst_temp);
		break;
	case 1:
		temp1 = src_temp(cv::Rect(0, 0, 1, rows));
		temp2 = src_temp(cv::Rect(2, 0, 1, rows));
		cv::addWeighted(temp1, 0.73, temp2, 0.27, 0, dst_temp);
		break;
	case 2:
		temp1 = src_temp(cv::Rect(0, 0, 1, rows));
		temp2 = src_temp(cv::Rect(1, 0, 1, rows));
		cv::addWeighted(temp1, 0.33, temp2, 0.67, 0, dst_temp);
		break;
	default:
		break;
	}
}
