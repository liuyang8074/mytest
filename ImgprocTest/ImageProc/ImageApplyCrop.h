#ifndef IMAGE_APPLY_CROP_H
#define IMAGE_APPLY_CROP_H

#include "imageapply.h"

class _HG_IMAGE_PROCESS_API CImageApplyCrop : public CImageApply
{

public:

	CImageApplyCrop(void);

	virtual ~CImageApplyCrop(void);

	virtual void apply(cv::Mat& pDib,int side);

	cv::Rect getROI() { return m_roi; }

	void setROI(const cv::Rect& rect) { m_roi = rect; }

private:

	cv::Rect m_roi;
};

#endif // !IMAGE_APPLY_CROP_H
