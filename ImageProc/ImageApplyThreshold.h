#ifndef IMAGE_APPLY_THRESHOLD_H
#define IMAGE_APPLY_THRESHOLD_H
#include "ImageApply.h"

class _HG_IMAGE_PROCESS_API CImageApplyThreshold :public CImageApply {
public:
	CImageApplyThreshold(float thre=128.0);
	virtual ~CImageApplyThreshold();
	virtual void apply(cv::Mat& pDib, int side);
private:
	float m_thresh;
};
#endif // !IMAGE_APPLY_THRESHOLD_H
