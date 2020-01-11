#ifndef IMAGE_APPLY_OUT_HOLE_H
#define IMAGE_APPLY_OUT_HOLE_H

#include "opencv2/opencv.hpp"
#include <vector>
#include "ImageApply.h"
class _HG_IMAGE_PROCESS_API ImageOutHole
{

public:

	ImageOutHole(void);

	~ImageOutHole(void);

public:

	void puncture(cv::Mat& front, cv::Mat& back, double threshold, float edgeScale, float borderSize, bool isDoubleFaces = true);

private:

	void getRoi(cv::RotatedRect rrect_front, cv::RotatedRect rrect_back, cv::Size srcSize, cv::Rect& roi_front,
		cv::Rect& roi_back, cv::RotatedRect& mask_rotatedRect);

	std::vector<std::vector<cv::Point> > filterPoly(std::vector<std::vector<cv::Point>>& contours, const std::vector<cv::Vec4i> &m, cv::RotatedRect roi,
		float edgeScale, float areaThreshold);

	cv::Scalar getBackGroudColor(const cv::Mat& image, const std::vector<cv::Point> pixelPoints);
};

#endif // !IMAGE_APPLY_OUT_HOLE_H
