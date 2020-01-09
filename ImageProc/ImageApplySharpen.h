#ifndef IMAGE_APPLY_SHARPEN_H
#define IMAGE_APPLY_SHARPEN_H

#include "ImageApply.h"

class _HG_IMAGE_PROCESS_API CImageApplySharpen : public CImageApply
{
public:
	CImageApplySharpen();

	virtual ~CImageApplySharpen();

	virtual void apply(cv::Mat& pDib, int side);
private:
	cv::Mat kernel;
};

#endif // !IMAGE_APPLY_SHARPEN_H
