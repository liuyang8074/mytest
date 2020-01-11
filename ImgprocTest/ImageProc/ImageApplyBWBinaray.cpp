#include "ImageApplyBWBinaray.h"

CImageApplyBWBinaray::CImageApplyBWBinaray()
	: m_threshold(127)
	, m_type(THRESH_BINARY)
	, m_blockSize(25)
	, m_constant(5)
{
}


CImageApplyBWBinaray::~CImageApplyBWBinaray(void)
{
}

void CImageApplyBWBinaray::apply(cv::Mat& pDib,int side)
{
	if (pDib.channels() == 3)
		cv::cvtColor(pDib, pDib, cv::COLOR_BGR2GRAY);

	switch (m_type)
	{
	case THRESH_BINARY:
		cv::threshold(pDib, pDib, m_threshold, 255, CV_THRESH_BINARY);
		break;
	case THRESH_OTSU:
		cv::threshold(pDib, pDib, m_threshold, 255, CV_THRESH_OTSU);
		break;
	case ADAPTIVE_GAUSSIAN:
		cv::adaptiveThreshold(pDib, pDib, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, m_blockSize, m_constant);
		break;
	case ADAPTIVE_MEAN:
		cv::adaptiveThreshold(pDib, pDib, 255, cv::ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, m_blockSize, m_constant);
		break;
	case ERROR_DIFFUSION:
		break;
	default:
		break;
	}
}