#ifndef IMAGE_APPLY_DISCARD_BLANK_H
#define IMAGE_APPLY_DISCARD_BLANK_H

#include "ImageApply.h"

class _HG_IMAGE_PROCESS_API CImageApplyDiscardBlank : public CImageApply
{
public:
	CImageApplyDiscardBlank(bool isnormal=true);
	virtual ~CImageApplyDiscardBlank(void);

	virtual void apply(cv::Mat& pDib,int  side); 

private:
	void setIntensity(int val);
	void setMinArea(int val);
	int ProcessRectR(cv::Mat & image, cv::RotatedRect & rotatedRect, std::vector<cv::Point>& maxContour, double scale, double thresh, int blobAreaSize);
	bool Scalar_LE(cv::Scalar& val1, cv::Scalar& val2);
	void GetContours(const cv::Mat& src, std::vector<std::vector<cv::Point>>& contours, std::vector<cv::Vec4i>& hierarchy, int retr = cv::RETR_CCOMP);
private:
	int dSize;
	
	bool m_res;
	cv::Scalar devTh;
	
	bool isNormalDiscard;
	cv::Mat getRoiMat(cv::Mat& pDib);
};

#endif // !IMAGE_APPLY_DISCARD_BLANK_H
