#include "ImageApplyCrop.h"

CImageApplyCrop::CImageApplyCrop(void)
{
}


CImageApplyCrop::~CImageApplyCrop(void)
{
}

void CImageApplyCrop::apply(cv::Mat& pDib,int side)
{
#ifdef LOG
	FileTools::write_log("imgprc.txt", "enter CImageApplyCrop apply");
#endif // LOG
	if (m_roi.x < 0 || m_roi.y < 0 || m_roi.br().x >= pDib.cols || m_roi.br().y >= pDib.rows || m_roi.width == 0 || m_roi.height == 0) 
		return;

	pDib = pDib(m_roi).clone();
#ifdef LOG
	FileTools::write_log("imgprc.txt", "exit CImageApplyCrop apply");
#endif // LOG
}
