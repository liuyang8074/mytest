#include "ImageApplyAutoCrop.h"
#include "ImageProcess_Public.h"

#define RE 8

CImageApplyAutoCrop::CImageApplyAutoCrop()
    : m_isCrop(false) //自动裁剪
    , m_isFillBlank(false) //填充空白
    , m_isDesaskew(false)   //自动纠偏
    , m_threshold(40) //二值化时阈值 >40 -->255 <40 -->0
    , m_noise(7)    //噪点
    , m_indent(5)//填充背景时覆盖边缘过渡色
{
}

CImageApplyAutoCrop::CImageApplyAutoCrop(bool isCrop, bool isDesaskew, bool isFillBlank, const cv::Size& fixedSize, double threshold, int noise, int indent)
	: m_isCrop(isCrop)
	, m_isFillBlank(isFillBlank)
	, m_isDesaskew(isDesaskew)
	, m_threshold(threshold)
	, m_noise(noise)
	, m_indent(indent)
    , m_fixedSize(fixedSize)//原图尺寸
{
}

CImageApplyAutoCrop::~CImageApplyAutoCrop()
{
}

void CImageApplyAutoCrop::apply(cv::Mat& pDib, int side)
{
#ifdef LOG
	FileTools::write_log("imgprc.txt", "enter CImageApplyAutoCrop apply");
#endif // LOG
	// if (!m_isCrop && !m_isFillBlank && !m_isDesaskew) return;
	cv::Mat src = pDib;
	cv::Mat dst;
	cv::Mat src_resize;
	cv::resize(src, src_resize, cv::Size(src.cols / RE, src.rows / RE));
	cv::Mat scale_mat;
	cv::Mat thre(src_resize.size(), CV_8UC1);
	hg::threshold_Mat(src_resize, thre, m_threshold);

	cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(m_noise, m_noise));
	cv::morphologyEx(thre, thre, cv::MORPH_OPEN, element);

	std::vector<cv::Vec4i> hierarchy;
	std::vector<std::vector<cv::Point>> contours;

	hg::findContours(thre, contours, hierarchy, cv::RETR_EXTERNAL);
	std::vector<cv::Point> maxContour = hg::getMaxContour(contours, hierarchy);

	for (cv::Point& item : maxContour)
	{
		item.x = item.x * RE + RE / 2;
		item.y = item.y * RE + RE / 2;
	}

	if (maxContour.size() == 0)
	{
		thre.release();
		return;
	}
	thre.release();
	dst.release();

	cv::RotatedRect rect = hg::getBoundingRect(maxContour);

	hg::convexHull(maxContour, maxContour);

	cv::Rect bounding_rect = rect.boundingRect();

	if (!(m_isCrop || m_isDesaskew || m_isFillBlank))
	{
#if 0
		dst = cv::Mat::zeros(m_fixedSize, src.type());
		cv::Mat temp = pDib(bounding_rect);

		cv::Rect crop_roi = getCenterRoi(temp.size(), m_fixedSize);
		temp(crop_roi).copyTo(dst(getCenterRoi(dst.size(), crop_roi.size())));
#else
		cv::Point center(bounding_rect.x + bounding_rect.width / 2, bounding_rect.y + bounding_rect.height / 2);
		cv::Rect roi;

		if (center.x < m_fixedSize.width / 2)
			roi.x = 0;
		else if (center.x > (pDib.cols - m_fixedSize.width / 2))
			roi.x = pDib.cols - m_fixedSize.width;
		else
			roi.x = center.x - m_fixedSize.width / 2 + 1;
		roi.width = m_fixedSize.width;

		if (center.y < m_fixedSize.height / 2)
			roi.y = 0;
		else if (center.y > pDib.rows - m_fixedSize.height / 2)
			roi.y = pDib.rows - m_fixedSize.height;
		else
			roi.y = center.y - m_fixedSize.height / 2 + 1;
		roi.height = m_fixedSize.height;

		cv::Mat dst = pDib(roi).clone();

#endif
		pDib.release();
		pDib = dst;
		return;
	}

	if (m_isCrop)
		if (m_isDesaskew)
			dst = cv::Mat::zeros(cv::Size(rect.size), src.type());
		else
		{
			cv::Rect bounding_rect_clone = bounding_rect;
			bounding_rect.x = cv::max(0, bounding_rect_clone.x);
			bounding_rect.y = cv::max(0, bounding_rect_clone.y);
			bounding_rect.width = cv::min(src.cols, bounding_rect_clone.x + bounding_rect_clone.width) - bounding_rect.x;
			bounding_rect.height = cv::min(src.rows, bounding_rect_clone.y + bounding_rect_clone.height) - bounding_rect.y;
			dst = src(bounding_rect).clone();
		}
	else
		if (m_isFillBlank)
			dst = src.clone();
		else
			dst = cv::Mat(src.rows, src.cols, src.type());

	cv::Mat warp_mat;
	if (m_isDesaskew)
	{
		cv::Point2f dstTri[3];
		cv::Point2f srcTri[4];
		rect.points(srcTri);

		if (m_isCrop)
		{
			dstTri[0] = cv::Point2f(0, rect.size.height - 1);
			dstTri[1] = cv::Point2f(0, 0);
			dstTri[2] = cv::Point2f(rect.size.width - 1, 0);
		}
		else
		{
			float left = (src.cols - rect.size.width) / 2;
			float right = left + rect.size.width - 1;
			float top = (src.rows - rect.size.height) / 2;
			float bottom = top + rect.size.height - 1;
			dstTri[0] = cv::Point2f(left, bottom);
			dstTri[1] = cv::Point2f(left, top);
			dstTri[2] = cv::Point2f(right, top);
		}

		cv::Size dSize = m_isCrop ? cv::Size(static_cast<int>(rect.size.width),
			static_cast<int>(rect.size.height)) : dst.size();
		bounding_rect.width = dSize.width;
		bounding_rect.height = dSize.height;

		warp_mat = cv::getAffineTransform(srcTri, dstTri);
		cv::warpAffine(src, dst, warp_mat, dSize);
	}

	if (m_isFillBlank)
	{
		if (m_isDesaskew)
			for (cv::Point& item : maxContour)
				item = hg::warpPoint(item, warp_mat);
		else
		{
			if (m_isCrop)
			{
				cv::Point offset = bounding_rect.tl();
				for (cv::Point& item : maxContour)
					item -= offset;
			}
		}

		hg::polyIndent(maxContour, m_indent);
		hg::fillBlackBackGround(dst, maxContour);
	}

	if (!m_isCrop)
		dst = dst(getCenterRoi(dst.size(), m_fixedSize));

	pDib.release();
	pDib = dst.clone();
	dst.release();
#ifdef LOG
	FileTools::write_log("imgprc.txt", "exit CImageApplyAutoCrop apply");
#endif // LOG
}

cv::Rect CImageApplyAutoCrop::getCenterRoi(const cv::Size& origin, const cv::Size& dst)
{
	cv::Rect center_roi;
	if (origin.width < dst.width)
	{
		center_roi.x = 0;
		center_roi.width = origin.width;
	}
	else
	{
		center_roi.x = (origin.width - dst.width) / 2;
		center_roi.width = dst.width;
	}

	if (origin.height < dst.height)
	{
		center_roi.y = 0;
		center_roi.height = origin.height;
	}
	else
	{
		center_roi.y = (origin.height - dst.height) / 2;
		center_roi.height = dst.height;
	}
	return center_roi;
}
