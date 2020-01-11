#include "ImageApplySharpen.h"

CImageApplySharpen::CImageApplySharpen()
	: kernel(5, 5, CV_32FC1)
{
	float kernel_data[] = { -0.1f, 0, 0, 0, -0.1f, 0, 0, 0, 0, 0, 0, 0, 1.5f, 0, 0, 0, 0, 0, 0, 0, -0.1f, 0, 0, 0, -0.1f };
	memcpy(kernel.data, kernel_data, sizeof(float) * 25);
}

CImageApplySharpen::~CImageApplySharpen()
{
}

void CImageApplySharpen::apply(cv::Mat & pDib, int side)
{
#ifdef LOG
	FileTools::write_log("imgprc.txt", "enter CImageApplySharpen apply");
#endif // LOG
	cv::filter2D(pDib, pDib, pDib.depth(), kernel);
#ifdef LOG
	FileTools::write_log("imgprc.txt", "exit CImageApplySharpen apply");
#endif // LOG
}
