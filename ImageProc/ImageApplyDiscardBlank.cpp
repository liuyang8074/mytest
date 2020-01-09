#include "ImageApplyDiscardBlank.h"

using namespace cv;
using namespace std;

int CImageApplyDiscardBlank::ProcessRectR(Mat & image, RotatedRect & rotatedRect, vector<Point>& maxContour, double scale, double thresh, int blobAreaSize)
{
	Mat gray;
	int blockCount = 0;
	if (image.channels() == 3)
	{
		if (scale != 1.0f)
		{
			Size ResImgSiz = Size(image.cols*scale, image.rows*scale);
			resize(image, gray, cv::Size(), scale, scale, 0);
			cvtColor(gray, gray, CV_BGR2GRAY);
		}
		else
		{
			cvtColor(image, gray, CV_BGR2GRAY);
		}
	}
	else
	{
		if (scale != 1.0f)
		{
			resize(image, gray, cv::Size(), scale, scale, 0);
		}
		else
		{
			gray = image;
		}
	}
	Mat threshold_img;
	threshold(gray, threshold_img, thresh, 255.0, CV_THRESH_BINARY);
	vector<vector<Point>> contours;
	std::vector<Vec4i> h1;
	GetContours(threshold_img, contours, h1, CV_CHAIN_APPROX_SIMPLE);
	threshold_img.release();

	if (contours.size() == 0)
	{
		return blockCount;
	}

	vector<Point2f> list_com;
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (area > blobAreaSize)
		{
			blockCount++;
			for (int j = 0; j < contours[i].size(); j++)
			{
				list_com.push_back(contours[i][j]);
			}
		}
	}

	if (list_com.size() == 0)
	{
		return blockCount;
	}

	rotatedRect = minAreaRect(list_com);
	rotatedRect.center.x /= (float)scale;
	rotatedRect.center.y /= (float)scale;
	rotatedRect.size.width /= (float)scale;
	rotatedRect.size.height /= (float)scale;

	if (rotatedRect.angle < -45.0f)
	{
		rotatedRect.angle += 90.0f;
		float l_temp = rotatedRect.size.width;
		rotatedRect.size.width = rotatedRect.size.height;
		rotatedRect.size.height = l_temp;
	}

	vector<int> hull(list_com.size());
	convexHull(list_com, hull);

	for (int i = 0; i < hull.size(); i++)
	{
		Point temp = list_com[hull[i]];
		int x = (int)(temp.x / scale);
		int y = (int)(temp.y / scale);
		maxContour.push_back(Point(x, y));
	}

	return blockCount;
}

  bool CImageApplyDiscardBlank:: Scalar_LE(cv::Scalar& val1, cv::Scalar& val2)
{
	for(int i = 0; i < 3; i++)
	{
		if(val1[i] > val2[i])
			return false;
	}
	return true;
}

  CImageApplyDiscardBlank::CImageApplyDiscardBlank(bool isnormal)
	: devTh (10, 10, 10, 10), dSize(200),isNormalDiscard(isnormal)
{
}


  CImageApplyDiscardBlank::~CImageApplyDiscardBlank(void)
{
}

void CImageApplyDiscardBlank::setIntensity(int val)
{
	val = max(min(20, val), 2); 
	devTh = cv::Scalar(val, val, val, val);
}

void CImageApplyDiscardBlank::setMinArea(int val)
{
	dSize = max(min(500, val), 100);
}

void CImageApplyDiscardBlank::GetContours(const Mat& src, vector<vector<Point>>& contours, vector<Vec4i>& hierarchy, int retr /*= RETR_CCOMP*/)
{
	CvMat c_image = src;
	MemStorage storage(cvCreateMemStorage());
	CvSeq* _ccontours = 0;
	cvFindContours(&c_image, storage, &_ccontours, sizeof(CvContour), retr, CHAIN_APPROX_SIMPLE);

	if (!_ccontours)
	{
		contours.clear();
		return;
	}
	Seq<CvSeq*> all_contours(cvTreeToNodeSeq(_ccontours, sizeof(CvSeq), storage));
	int total = (int)all_contours.size();
	contours.resize(total);

	SeqIterator<CvSeq*> it = all_contours.begin();
	for (int i = 0; i < total; i++, ++it)
	{
		CvSeq* c = *it;
		((CvContour*)c)->color = (int)i;
		int count = (int)c->total;
		int* data = new int[count * 2];
		cvCvtSeqToArray(c, data);
		for (int j = 0; j < count; j++)
		{
			contours[i].push_back(Point(data[j * 2], data[j * 2 + 1]));
		}
		delete[] data;
	}

	hierarchy.resize(total);
	it = all_contours.begin();
	for (int i = 0; i < total; i++, ++it)
	{
		CvSeq* c = *it;
		int h_next = c->h_next ? ((CvContour*)c->h_next)->color : -1;
		int h_prev = c->h_prev ? ((CvContour*)c->h_prev)->color : -1;
		int v_next = c->v_next ? ((CvContour*)c->v_next)->color : -1;
		int v_prev = c->v_prev ? ((CvContour*)c->v_prev)->color : -1;
		hierarchy[i] = Vec4i(h_next, h_prev, v_next, v_prev);
	}
}

cv::Mat CImageApplyDiscardBlank::getRoiMat(cv::Mat& image)
{
	int gap = 100;
	RotatedRect rect;
	vector<Point> contour;
	double scale = 0.25;
	double thresh = 50;
	int blobSize = 200;
	int edgeWidth = 10;
	ProcessRectR(image, rect, contour, scale, thresh, blobSize);
	cv::Rect rect2 = rect.boundingRect();
	cv::Rect inRect = rect2 & Rect(0, 0, image.cols, image.rows);
	gap = max(inRect.width - rect.size.width, inRect.height -rect.size.height)+100;
	inRect = cv::Rect(inRect.x+ gap, inRect.y + gap, inRect.width -gap*2, inRect.height-gap*2);
	return image(inRect);
}

void CImageApplyDiscardBlank::apply(cv::Mat& pDib,int side)
{
#ifdef LOG
	FileTools::write_log("imgprc.txt", "enter CImageApplyDiscardBlank apply");
#endif // LOG
	setIntensity(isNormalDiscard?8:20);
	setMinArea(isNormalDiscard?200:300);

	cv::Scalar mean;  
	cv::Scalar dev;  
	cv::Mat image = getRoiMat(pDib);
	cv::Rect rect;
	cv::Rect imRect(0, 0, image.cols, image.rows);
	for(int i = 0; i < image.cols; i+= dSize)
	{
		for(int j = 0; j < image.rows; j+= dSize)
		{
			rect = cv::Rect(i, j,dSize, dSize) & imRect; 
			if(rect != cv::Rect())
			{
				cv::meanStdDev (image(rect) , mean, dev);  	
				if(!Scalar_LE(dev, devTh))
				{
					m_res = false;
					return;
				}
			}
		}
	}
	m_res = true;
	if (m_res)
		pDib.release();
#ifdef LOG
	FileTools::write_log("imgprc.txt", "exit CImageApplyDiscardBlank apply");
#endif // LOG
}