#include "ImageProcess_Public.h"

typedef unsigned int uint;

namespace hg
{
	void convexHull(const std::vector<cv::Point>& src, std::vector<cv::Point>& dst, bool clockwise)
	{
		CvMemStorage* storage = cvCreateMemStorage();	//
		CvSeq* ptseq = cvCreateSeq(CV_SEQ_KIND_GENERIC | CV_32SC2, sizeof(CvContour), sizeof(CvPoint), storage);	//ptseqstorage

		//
		for (const cv::Point& item : src)
		{
			CvPoint p;
			p.x = item.x;
			p.y = item.y;
			cvSeqPush(ptseq, &p);
		}

		//¦Ìhullstorage
		CvSeq* hull = cvConvexHull2(ptseq, nullptr, clockwise ? CV_CLOCKWISE : CV_COUNTER_CLOCKWISE, 0);

		//dst
		dst.clear();
		int hullCount = hull->total;
		for (int i = 0; i < hullCount; i++)
			dst.push_back(**CV_GET_SEQ_ELEM(CvPoint*, hull, i));

		//storage
		cvReleaseMemStorage(&storage);
	}

#define R_COLOR 255
	void fillBlackBackGround(cv::Mat& src, std::vector<cv::Point> points)
	{
		uint index_top = 0;
		uint index_bottom = 0;
		for (size_t i = 0, length = points.size(); i < length; i++)
		{
			if (points[i].y < points[index_top].y)
				index_top = i;
			if (points[i].y > points[index_bottom].y)
				index_bottom = i;
		}

		std::vector<cv::Point> edge_left;
		uint temp = index_top;
		while (temp != index_bottom)
		{
			edge_left.push_back(points[temp]);
			temp = (temp + points.size() - 1) % points.size();
		}
		edge_left.push_back(points[index_bottom]);

		std::vector<cv::Point> edge_right;
		temp = index_top;
		while (temp != index_bottom)
		{
			edge_right.push_back(points[temp]);
			temp = (temp + points.size() + 1) % points.size();
		}
		edge_right.push_back(points[index_bottom]);

		std::vector<int> left_edge_x;
		std::vector<int> left_edge_y;
		for (size_t i = 0, length = edge_left.size() - 1; i < length; i++)
		{
			int y_top = edge_left[i].y;
			int x_top = edge_left[i].x;
			int y_bottom = edge_left[i + 1].y;
			int x_bottom = edge_left[i + 1].x;
			for (int y = y_top; y < y_bottom; y++)
				if (y >= 0 && y_top != y_bottom && y < src.rows)
				{
					left_edge_x.push_back(((x_bottom - x_top) * y + x_top * y_bottom - x_bottom * y_top) / (y_bottom - y_top));
					left_edge_y.push_back(y);
				}
		}
		size_t step = src.step;
		unsigned char* ptr;
		ptr = src.data + static_cast<uint>(left_edge_y[0]) * step;
		for (size_t i = 0, length = left_edge_x.size(); i < length; i++)
		{
			int pix = left_edge_x[i];
			if (pix < src.cols - 1 && pix > 0)
				memset(ptr + i * step, R_COLOR, static_cast<size_t>((pix + 1) * src.channels()));
		}

		std::vector<int> right_edge_x;
		std::vector<int> right_edge_y;
		for (size_t i = 0, length = edge_right.size() - 1; i < length; i++)
		{
			int y_top = edge_right[i].y;
			int x_top = edge_right[i].x;
			int y_bottom = edge_right[i + 1].y;
			int x_bottom = edge_right[i + 1].x;
			for (int y = y_top; y < y_bottom; y++)
				if (y_top != y_bottom && y < src.rows && y >= 0)
				{
					right_edge_x.push_back(((x_bottom - x_top) * y + x_top * y_bottom - x_bottom * y_top) / (y_bottom - y_top));
					right_edge_y.push_back(y);
				}
		}

		ptr = src.data + static_cast<uint>(right_edge_y[0]) * step;
		for (size_t i = 0, length = right_edge_x.size(); i < length; i++)
		{
			int pix = right_edge_x[i];
			if (pix < src.cols - 1 && pix > 0)
				memset(ptr + i * step + pix * src.channels(), R_COLOR, step - static_cast<size_t>(pix * src.channels()));
		}

		if (edge_left[0].y > 0)
			memset(src.data, R_COLOR, static_cast<size_t>(edge_left[0].y) * step);

		if (edge_left.back().y < src.rows - 1)
			memset(src.data + static_cast<size_t>(edge_left.back().y) * step, R_COLOR,
				static_cast<size_t>(src.rows - edge_left.back().y) * step);
	}

	void fillPoly(cv::Mat & image, const std::vector<cv::Point>& contours, const cv::Scalar & color)
	{
		size_t count = contours.size();
		cv::Point * points = new cv::Point[count];
		for (size_t i = 0; i < count; i++)
			points[i] = contours[i];

		const cv::Point* pointss[1] = { points };
		int npts[1];
		npts[0] = static_cast<int>(count);

		cv::fillPoly(image, pointss, npts, 1, color);

		delete[] points;
	}

	void findContours(const cv::Mat& src, std::vector<std::vector<cv::Point>>& contours, std::vector<cv::Vec4i>& hierarchy, int retr, int method, cv::Point offset)
	{
		CvMat c_image = src;
		cv::MemStorage storage(cvCreateMemStorage());
		CvSeq* _ccontours = nullptr;
		cvFindContours(&c_image, storage, &_ccontours, sizeof(CvContour), retr, method, CvPoint(offset));

		if (!_ccontours)
		{
			contours.clear();
			return;
		}
		cv::Seq<CvSeq*> all_contours(cvTreeToNodeSeq(_ccontours, sizeof(CvSeq), storage));
		size_t total = all_contours.size();
		contours.resize(total);

		cv::SeqIterator<CvSeq*> it = all_contours.begin();
		for (size_t i = 0; i < total; i++, ++it)
		{
			CvSeq* c = *it;
			reinterpret_cast<CvContour*>(c)->color = static_cast<int>(i);
			int count = c->total;
			int* data = new int[static_cast<size_t>(count * 2)];
			cvCvtSeqToArray(c, data);
			for (int j = 0; j < count; j++)
			{
				contours[i].push_back(cv::Point(data[j * 2], data[j * 2 + 1]));
			}
			delete[] data;
		}

		hierarchy.resize(total);
		it = all_contours.begin();
		for (size_t i = 0; i < total; i++, ++it)
		{
			CvSeq* c = *it;
			int h_next = c->h_next ? reinterpret_cast<CvContour*>(c->h_next)->color : -1;
			int h_prev = c->h_prev ? reinterpret_cast<CvContour*>(c->h_prev)->color : -1;
			int v_next = c->v_next ? reinterpret_cast<CvContour*>(c->v_next)->color : -1;
			int v_prev = c->v_prev ? reinterpret_cast<CvContour*>(c->v_prev)->color : -1;
			hierarchy[i] = cv::Vec4i(h_next, h_prev, v_next, v_prev);
		}

		storage.release();
	}

	cv::RotatedRect getBoundingRect(const std::vector<cv::Point>& contour)
	{
		if (contour.empty()) return {};

		cv::RotatedRect rect = minAreaRect(contour);
		if (rect.angle < -45)
		{
			rect.angle += 90;
			float temp = rect.size.width;
			rect.size.width = rect.size.height;
			rect.size.height = temp;
		}

		return rect;
	}

	std::vector<cv::Point> getMaxContour(const std::vector<std::vector<cv::Point>>& contours, const std::vector<cv::Vec4i>& hierarchy)
	{
		std::vector<cv::Point> maxContour;
		if (contours.size() < 1) return {};

		for (size_t i = 0, length = hierarchy.size(); i < length; i++)
			if (hierarchy[i][3] == -1)
				for (const auto &item : contours[i])
					maxContour.push_back(item);

		return maxContour;
	}

	std::vector<cv::Point> getVertices(const cv::RotatedRect& rect)
	{
		cv::Point2f box[4];
		rect.points(box);
		std::vector<cv::Point> points;
		for (int i = 0; i < 4; i++)
			points.push_back(cv::Point(box[i]));

		return points;
	}

	void polyIndent(std::vector<cv::Point>& points, float indent)
	{
		static cv::Point zero(0, 0);
		cv::Point center = getBoundingRect(points).center;
		for (cv::Point& item : points)
		{
			cv::Point vec = item - center;
			if (vec != zero)
			{
				int length = vec.x * vec.x + vec.y * vec.y;
				float x = cv::sqrt(static_cast<float>(vec.x * vec.x / length)) * indent;
				float y = cv::sqrt(static_cast<float>(vec.y * vec.y / length)) * indent;

				if (vec.x < 0) x *= -1.0f;
				if (vec.y < 0) y *= -1.0f;

				item.x -= static_cast<int>(x);
				item.y -= static_cast<int>(y);
			}
		}

		hg::convexHull(points, points);
	}

	cv::Mat transforColor(const cv::Mat& src)
	{
		if (src.channels() == 1) return src.clone();

		std::vector<cv::Mat> channels(3);
		cv::split(src, channels);

		cv::Mat temp, dst;
		bitwise_or(channels[0], channels[1], temp);
		bitwise_or(channels[2], temp, dst);
		temp.release();

		for (cv::Mat& index : channels)
			index.release();
		return dst;
	}

	void threshold_Mat(const cv::Mat& src, cv::Mat& dst, double thre)
	{
		if (src.channels() == 3)
		{
#ifdef USE_ONENCL
			if (cl_res.context)
				transforColor_threshold_opencl(src, dst, static_cast<uchar>(thre));
			else
#endif
			{
				cv::Mat gray = transforColor(src);
				cv::threshold(gray, dst, thre, 255, cv::THRESH_BINARY);
				gray.release();
			}
		}
		else
			cv::threshold(src, dst, thre, 255, cv::THRESH_BINARY);
	}

	cv::Point warpPoint(cv::Point p, const cv::Mat& warp_mat)
	{
		double src_data[3] = { static_cast<double>(p.x), static_cast<double>(p.y), 1 };
		cv::Mat src(3, 1, warp_mat.type(), src_data);	//warp_mat.type() == CV_64FC1

		cv::Mat dst = warp_mat * src;
		double* ptr = reinterpret_cast<double*>(dst.data);
		return cv::Point(static_cast<int>(ptr[0]), static_cast<int>(ptr[1]));
	}
}
