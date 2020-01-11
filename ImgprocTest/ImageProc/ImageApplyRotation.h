#ifndef IMAGE_APPLY_ROTATION_H
#define IMAGE_APPLY_ROTATION_H

#include "ImageApply.h"

class _HG_IMAGE_PROCESS_API CImageApplyRotation : public CImageApply
{
public:
	enum RotationType
	{
		Invalid,
		Rotate_90_clockwise,
		Rotate_180,
		Rotate_90_anti_clockwise,

		AutoTextOrientation
	};

public:

	CImageApplyRotation(RotationType rotation, bool isBackTransposed = false, int dpi = 200, const char* tessdataPath = nullptr);

	virtual ~CImageApplyRotation();

	virtual void apply(cv::Mat & pDib, int side) override;

	bool isBackTransposed() { return m_rotation; }

	int getDPI() { return m_dpi; }

	RotationType getRotationType() { return m_rotation; }

	void setBackTransposed(bool enabled) { m_backTranspose = enabled; }

	void setDPI(int dpi) { m_dpi = dpi; }

	void setRotationType(RotationType type) { m_rotation = type; }

private:
	RotationType m_rotation;
	bool m_backTranspose;
	int m_dpi;

	void* osd;
};

#endif // !IMAGE_APPLY_ROTATION_H
