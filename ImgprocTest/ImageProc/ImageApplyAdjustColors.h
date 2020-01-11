#ifndef IMAGE_APPLY_ADJUST_COLOR_H
#define IMAGE_APPLY_ADJUST_COLOR_H

#include "ImageApply.h"

class _HG_IMAGE_PROCESS_API CImageApplyAdjustColors : public CImageApply
{
public:

    CImageApplyAdjustColors(int brightness, int contrast, float gamma);

	virtual ~CImageApplyAdjustColors(void);

	virtual void apply(cv::Mat& pDib, int side);

	void setAdjustColors(int brightness, int contrast, float gamma);

	int getContrast() { return m_contrast; }

	int getBrightness() { return m_brightness; }

	double getGamma() { return m_gamma; }

	void setBrightness(int brightness);

	void setContrast(int contrast);

	void setGamma(float gamma);

	static CImageApplyAdjustColors* getObj();

private:

	CImageApplyAdjustColors(void);

	void update_lutData();

private:

	int m_brightness;
	int m_contrast;
	float m_gamma;
	cv::Mat lut;
};

#endif // !IMAGE_APPLY_ADJUST_COLOR_H


