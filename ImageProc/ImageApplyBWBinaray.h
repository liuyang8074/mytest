#ifndef IMAGE_APPLY_BW_BINARAY_H
#define IMAGE_APPLY_BW_BINARAY_H

#include "ImageApply.h"


class _HG_IMAGE_PROCESS_API CImageApplyBWBinaray:public CImageApply
{
public:

	enum ThresholdType
	{
		THRESH_BINARY,
		THRESH_OTSU,

		ADAPTIVE_GAUSSIAN,
		ADAPTIVE_MEAN,

		ERROR_DIFFUSION
	};

	CImageApplyBWBinaray();

	virtual ~CImageApplyBWBinaray(void);

	virtual void apply(cv::Mat& pDib,int side);

	double getThreshold() { return m_threshold; }

	ThresholdType getThresholdType() { return m_type; }

	int getBlockSize() { return m_blockSize; }

	double getConstant() { return m_constant; }

	void setThreshold(double value) { m_threshold = value; }

	void setThresholdType(ThresholdType type) { m_type = type; }

	void setBlockSize(int value) { m_blockSize = value; }

	void setConstant(double value) { m_constant = value; }

private:
	double m_threshold;

	ThresholdType m_type;

	int m_blockSize;

	double m_constant;
};

#endif //!IMAGE_APPLY_BW_BINARAY_H

