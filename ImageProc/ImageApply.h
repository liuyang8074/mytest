#ifndef IMAGE_APPLY_H
#define IMAGE_APPLY_H

#include <vector>
#include <memory>
#include <opencv2/opencv.hpp>

#if defined(LOG)
#include "Device/filetools.h"
#endif

#if !defined _HG_IMAGE_PROCESS_BUILD
#define _HG_IMAGE_PROCESS_API  __declspec(dllexport)
#else _HG_IMAGE_PROCESS_API __declspec(dllimport)
#endif // !_HG_IMAGE_PROCESS_BUILD

class _HG_IMAGE_PROCESS_API CImageApply
{
public:
	CImageApply(void);
	virtual ~CImageApply(void);

	virtual void apply(cv::Mat& pDib,int side) = 0;
};

typedef std::shared_ptr<CImageApply> ImageApplyPtr;

#endif //!IMAGE_APPLY_H
