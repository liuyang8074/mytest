#ifndef IMAGE_PROCESS_PUBLIC_H
#define IMAGE_PROCESS_PUBLIC_H

#include "opencv2/opencv.hpp"
#include <vector>

namespace hg
{
	void convexHull(const std::vector<cv::Point>& src, std::vector<cv::Point>& dst, bool clockwise = false);

	void fillBlackBackGround(cv::Mat& src, std::vector<cv::Point> points);

	void fillPoly(cv::Mat& image, const std::vector<cv::Point>& contours, const cv::Scalar& color);

	void findContours(const cv::Mat& src, std::vector<std::vector<cv::Point>>& contours, std::vector<cv::Vec4i>& hierarchy,
		int retr = cv::RETR_LIST, int method = cv::CHAIN_APPROX_SIMPLE, cv::Point offset = cv::Point(0, 0));

	cv::RotatedRect getBoundingRect(const std::vector<cv::Point>& contour);

	std::vector<cv::Point> getMaxContour(const std::vector<std::vector<cv::Point>>& contours, const std::vector<cv::Vec4i>& hierarchy);

	std::vector<cv::Point> getVertices(const cv::RotatedRect& rect);

	void polyIndent(std::vector<cv::Point>& points, float indent);

	void threshold_Mat(const cv::Mat& src, cv::Mat& dst, double thre);

	cv::Mat transforColor(const cv::Mat& src);

	cv::Point warpPoint(cv::Point p, const cv::Mat& warp_mat);
}

#endif // !IMAGE_PROCESS_C_H
