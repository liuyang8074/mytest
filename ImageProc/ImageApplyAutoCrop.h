#ifndef IMAGE_APPLY_AUTO_CROP_H
#define IMAGE_APPLY_AUTO_CROP_H

#include "ImageApply.h"

class _HG_IMAGE_PROCESS_API CImageApplyAutoCrop : public CImageApply
{
public:
	CImageApplyAutoCrop();

	CImageApplyAutoCrop(bool isCrop, bool isDesaskew, bool isFillBlank, const cv::Size& fixedSize, double threshold = 40, int noise = 7, int indent = 5);

	virtual ~CImageApplyAutoCrop();

	virtual void apply(cv::Mat& pDib, int side);

	bool isAutoCrop() { return m_isCrop; }

	bool isFillBlank() { return m_isFillBlank; }

	bool isDesaskew() { return m_isDesaskew; }

	double threshold() { return m_threshold; }

	int noise() { return m_noise; }

	int indent() { return m_indent; }

	cv::Size fixedSize() { return m_fixedSize; }

	void setAutoCrop(bool enabled) { m_isCrop = enabled; }

	void setFillBlank(bool enabled) { m_isFillBlank = enabled; }

	void setDesaskew(bool enabled) { m_isDesaskew = enabled; }

	void setThreshold(double value) { m_threshold = value; }

	void setNoise(int value) { m_noise = value; }

	void setIndent(int value) { m_indent = value; }

	void setFixedSize(cv::Size size) { m_fixedSize = size; }

private:

	static cv::Rect getCenterRoi(const cv::Size& origin, const cv::Size& dst);

private:
	bool m_isCrop;
	bool m_isFillBlank;
	bool m_isDesaskew;

	double m_threshold;
	int m_noise;
	int m_indent;
	cv::Size m_fixedSize;

};

#endif // !IMAGE_APPLY_AUTO_CROP_H


