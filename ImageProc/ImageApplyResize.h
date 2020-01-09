#ifndef IMAGE_APPLY_RESIZE_H
#define IMAGE_APPLY_RESIZE_H

#include "imageapply.h"

class _HG_IMAGE_PROCESS_API CImageApplyResize : public CImageApply
{
public:

	enum ResizeType
	{
		RATIO,
		DSIZE
	};

public:
	CImageApplyResize();

	CImageApplyResize(ResizeType type, const cv::Size& size, double fx, double fy);

	virtual ~CImageApplyResize(void);

	virtual void apply(cv::Mat& pDib,int side);

	double getFX() { return m_fx; }

	double getFY() { return m_fy; }

	cv::Size getDSize() { return m_dSize; }

	ResizeType getType() { return m_type; }

	void setFX(double value) { m_fx = value; }

	void setFY(double value) { m_fy = value; }

	void setDSize(const cv::Size& size) { m_dSize = size; }

	void setType(ResizeType type) { m_type = type; }
	
private:
	double m_fx;
	double m_fy;
	cv::Size m_dSize;
	ResizeType m_type;
};

#endif // !IMAGE_APPLY_RESIZE_H
