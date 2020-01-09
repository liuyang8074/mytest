#include "ImageApplyResize.h"

CImageApplyResize::CImageApplyResize()
	: m_fx(1.0)
	, m_fy(1.0)
	, m_type(RATIO)
{
}

CImageApplyResize::CImageApplyResize(ResizeType type, const cv::Size& size, double fx, double fy)
	: m_type(type)
	, m_dSize(size)
	, m_fx(fx)
	, m_fy(fy)
{
}


CImageApplyResize::~CImageApplyResize(void)
{
}


void CImageApplyResize::apply(cv::Mat& pDib,int side)
{
#ifdef LOG
	FileTools::write_log("imgprc.txt", "enter CImageApplyResize apply");
#endif // LOG
	if (m_type == RATIO)
		cv::resize(pDib, pDib, cv::Size(0, 0), m_fx, m_fy);
	else
		cv::resize(pDib, pDib, m_dSize);
#ifdef LOG
	FileTools::write_log("imgprc.txt", "exit CImageApplyResize apply");
#endif // LOG
}
