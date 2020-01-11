#include "ImageApplyOutHole.h"
#include "ImageProcess_Public.h"

#ifdef LOG
#include "Device/filetools.h"
#endif // LOG

ImageOutHole::ImageOutHole(void)
{
}

ImageOutHole::~ImageOutHole(void)
{
}

void ImageOutHole::puncture(cv::Mat& front, cv::Mat& back, double threshold, float edgeScale, float borderSize, bool isDoubleFaces)
{
#ifdef LOG
	FileTools::write_log("imgprc.txt", "enter ImageOutHole apply");
#endif // LOG
	//��ֵ��������ͼ��
	threshold = std::min(std::max(threshold, 1.0), 254.0);
	cv::Mat front_thre, back_thre;
	hg::threshold_Mat(front, front_thre, threshold);
	hg::threshold_Mat(back, back_thre, threshold);

	//�����ֵ��ͼ��ˮƽ��ת
	cv::flip(back_thre, back_thre, 1);  //1:Horizontal

	//������ͼ��Ѱ��
	std::vector<std::vector<cv::Point>> contours_front, contours_back;
	std::vector<cv::Vec4i> b1_front, b1_back;
	hg::findContours(front_thre.clone(), contours_front, b1_front, cv::RETR_EXTERNAL);
	hg::findContours(back_thre.clone(), contours_back, b1_back, cv::RETR_EXTERNAL);

	//��ȡ������ͼ���������
	std::vector<cv::Point> maxContour_front = hg::getMaxContour(contours_front, b1_front);
	std::vector<cv::Point> maxContour_back = hg::getMaxContour(contours_back, b1_back);

	cv::RotatedRect rrect_front = hg::getBoundingRect(maxContour_front);          //��ȡ���������������С��Ӿ���
	cv::RotatedRect rrect_back = hg::getBoundingRect(maxContour_back);            //��ȡ���������������С��Ӿ���

	//��ȡ������ͼ���ص���������
	cv::Rect roi_front, roi_back;
	cv::RotatedRect mask_rotatedRect;
	getRoi(rrect_front, rrect_back, cv::Size(front.cols, front.rows), roi_front, roi_back, mask_rotatedRect);

	cv::Mat roiMat_front(front_thre, roi_front);                             //�������ֵͼ���н�ȡ�ص�����
	cv::Mat roiMat_back(back_thre, roi_back);                                //�ڷ����ֵͼ���н�ȡ�ص�����

	//�������ֵͼ���������㣬�����ο�������0�������ط����Ϊ255
	//Ϊ�˱���׶����׹ᴩֽ�ߣ���Ϊ����ֽ��������ȷ�����п׶�Ϊ���ͼ�Σ������뱳��ճ����
	cv::Mat mask;
	bitwise_or(roiMat_front, roiMat_back, mask);                            //�����㣬�������ֵͼ���ص�

	//��ֵͼ���ص�ͼ����ɫȡ�������ͣ���ȡ����
	std::vector<std::vector<cv::Point>> contours_mask;
	std::vector<cv::Vec4i> b1_mask;
	bitwise_not(mask, mask);          

	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
	dilate(mask, mask, element, cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, cv::Scalar(255));      //�����㷨������׶���ͨ�������

	polylines(mask, hg::getVertices(mask_rotatedRect), true, cv::Scalar(0), 15);                         //����ֽ�ž��α�Ե

	hg::findContours(mask.clone(), contours_mask, b1_mask, cv::RETR_TREE);                              //��ȡ�ص�ͼ������

	//���˷ǿ׶�����ͨ����
	std::vector<std::vector<cv::Point>> hole_contours = filterPoly(contours_mask, b1_mask, mask_rotatedRect, edgeScale, borderSize);
	for (int i = 0; i < hole_contours.size(); i++) 
		cv::drawContours(mask, hole_contours, i, cv::Scalar(127), 2);

	for (size_t i = 0; i < hole_contours.size(); i++)
	{
		cv::Scalar color = getBackGroudColor(front(roi_front), hole_contours[i]);
		cv::Mat temp = front(roi_front);
		hg::fillPoly(temp, hole_contours[i], color);
	}

	if (isDoubleFaces)
	{
		int width_ = roi_back.width;
		roi_back.x = back.cols - roi_back.width - roi_back.x;               //��Ϊ֮ǰ����ͼ��ת����������ROIҲҪ������Ӧ��ת
		for (size_t i = 0; i < hole_contours.size(); i++)
		{
			std::vector<cv::Point> hole_contour;
			for (size_t j = 0; j < hole_contours[i].size(); j++)
				hole_contour.push_back(cv::Point(width_ - hole_contours[i][j].x - 1, hole_contours[i][j].y));

			cv::Scalar color = getBackGroudColor(back(roi_back), hole_contour);
			cv::Mat temp = back(roi_back);
			hg::fillPoly(temp, hole_contour, color);
		}
	}
#ifdef LOG
	FileTools::write_log("imgprc.txt", "exit ImageOutHole apply");
#endif // LOG
}

void ImageOutHole::getRoi(cv::RotatedRect rrect_front, cv::RotatedRect rrect_back, cv::Size srcSize,
	cv::Rect& roi_front, cv::Rect& roi_back, cv::RotatedRect& mask_rotatedRect)
{
	cv::Size size(static_cast<int>(rrect_front.size.width + rrect_back.size.width) / 2, static_cast<int>(rrect_front.size.height + rrect_back.size.height) / 2);
	float angle = (rrect_front.angle + rrect_back.angle) / 2;

	rrect_front.size = rrect_back.size = size;
	rrect_front.angle = rrect_back.angle = angle;

	roi_front = rrect_front.boundingRect();
	roi_back = rrect_back.boundingRect();

	if (roi_front.width != roi_back.width || roi_front.height != roi_back.height)
	{
		roi_front.height = roi_back.height;
		roi_front.width = roi_back.width;
	}

	cv::Point offset(0, 0);
	int top = std::min(roi_front.y, roi_back.y);
	if (top < 0)
	{
		roi_front.y -= top;
		roi_back.y -= top;
		roi_front.height += top;
		roi_back.height += top;
		offset.y += top;
	}

	int left = std::min(roi_front.x, roi_back.x);
	if (left < 0)
	{
		roi_front.x -= left;
		roi_back.x -= left;
		roi_front.width += left;
		roi_back.width += left;
		offset.x += left;
	}

	int right = std::max(roi_front.x + roi_front.width, roi_back.x + roi_back.width);
	if (right >= srcSize.width)
	{
		roi_front.width -= (right - srcSize.width + 1);
		roi_back.width -= (right - srcSize.width + 1);
	}

	int bottom = std::max(roi_front.y + roi_front.height, roi_back.y + roi_back.height);
	if (bottom >= srcSize.height)
	{
		roi_front.height -= (bottom - srcSize.height + 1);
		roi_back.height -= (bottom - srcSize.height + 1);
	}

	mask_rotatedRect.center = cv::Point((roi_front.width + offset.x) / 2, (roi_front.height + offset.y) / 2);
	mask_rotatedRect.size = size;
	mask_rotatedRect.angle = angle;
}

std::vector<std::vector<cv::Point>> ImageOutHole::filterPoly(std::vector<std::vector<cv::Point>>& contours, const std::vector<cv::Vec4i>& m,
	cv::RotatedRect roi, float edgeScale, float areaThreshold)
{
	edgeScale = std::min(0.49f, std::max(edgeScale, 0.0f));
	cv::RotatedRect roi2(roi.center, cv::Size(static_cast<int>(roi.size.width * (1 - edgeScale * 2)),
		static_cast<int>(roi.size.height * (1 - edgeScale * 2))), roi.angle);

	std::vector<cv::Point> vertices_roi1 = hg::getVertices(roi);
	std::vector<cv::Point> vertices_roi2 = hg::getVertices(roi2);

	std::vector<std::vector<cv::Point>> hole_contours;
	for (size_t i = 0, length = contours.size(); i < length; i++)
	{
		if (m[i][2] != -1) continue;

		cv::RotatedRect rrect = hg::getBoundingRect(contours[i]);
		if (rrect.size.width > areaThreshold || rrect.size.height > areaThreshold) continue;

		bool enabled = true;
		for (size_t j = 0, count = contours[i].size(); j < count; j++)
		{
			cv::Point p(contours[i][j]);
			double temp1 = pointPolygonTest(vertices_roi1, p, false);     //�ж��Ƿ���ֽ����      1���ڣ�0���ϣ�-1����
			double temp2 = pointPolygonTest(vertices_roi2, p, false);     //�ж��Ƿ��ڱ�Ե������  1���ڣ�0���ϣ�-1����
			//�����ֽ���⣬���߱�Ե�ڣ���Ϊ�ǿ׶�
			if (temp1 < 0 || temp2 > 0)
			{
				enabled = false;
				break;
			}
		}

		if (enabled)
			hole_contours.push_back(contours[i]);
	}
	return hole_contours;
}

cv::Scalar ImageOutHole::getBackGroudColor(const cv::Mat &image, const std::vector<cv::Point> pixelPoints)
{
	if (pixelPoints.empty()) return cv::Scalar(255, 255, 255);

	int channels = image.channels();

	int temp[3] = { 0 };
	for (size_t i = 0, length = pixelPoints.size(); i < length; ++i)
	{
		int x = cv::min(cv::max(0, pixelPoints[i].x), image.cols - 1);
		int y = cv::min(cv::max(0, pixelPoints[i].y), image.rows - 1);

		const unsigned char* ptr = image.ptr(y, x);
		for (int j = 0; j < channels; ++j)
			temp[j] += ptr[j];
	}

	return cv::Scalar(temp[0] / pixelPoints.size(), temp[1] / pixelPoints.size(), temp[2] / pixelPoints.size());
}

