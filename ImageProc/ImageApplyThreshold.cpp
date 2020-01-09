#include "ImageApplyThreshold.h"

CImageApplyThreshold::CImageApplyThreshold(float thre) :m_thresh(thre)
{
}

CImageApplyThreshold::~CImageApplyThreshold()
{
}

void CImageApplyThreshold::apply(cv::Mat& pDib, int side)
{
#ifdef LOG
	FileTools::write_log("imgprc.txt", "enter CImageApplyThreshold apply");
#endif // LOG
	if (pDib.channels() >= 3)
		return;
	if (m_thresh == 0.0f)
		m_thresh = 140.0;
	cv::threshold(pDib, pDib, m_thresh, 255.0,cv::THRESH_BINARY);
#ifdef LOG
	FileTools::write_log("imgprc.txt", "exit CImageApplyThreshold apply");
#endif // LOG
}
