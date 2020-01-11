#include "ImageApplyRotation.h"
#ifdef USE_TESSERCAT
#include "../HG_OCR4/include/hg_ocr4.h"
#endif

CImageApplyRotation::CImageApplyRotation(RotationType rotation, bool isBackTransposed, int dpi, const char* tessdataPath)
	: m_rotation(rotation)
	, m_backTranspose(isBackTransposed)
	, m_dpi(dpi)
	, osd(nullptr)
{
	if (rotation == AutoTextOrientation)
	{
#ifdef USE_TESSERCAT
		osd = new HG_OCR4();
		reinterpret_cast<HG_OCR4*>(osd)->init(tessdataPath, HG_OCR4::Orientation);
#endif
	}
}


CImageApplyRotation::~CImageApplyRotation()
{
#ifdef USE_TESSERCAT
	if (osd) delete reinterpret_cast<HG_OCR4*>(osd);
#endif
}

void CImageApplyRotation::apply(cv::Mat & pDib, int side)
{
	if (m_rotation == AutoTextOrientation)	//自动文本方向识别
	{
#ifdef USE_TESSERCAT
        if (osd)
        {
            cv::Mat temp;
            double scale = 150.0 / static_cast<double>(m_dpi);
            cv::resize(pDib, temp, cv::Size(), scale, scale);
            if (temp.channels() == 3)
                cv::cvtColor(temp, temp, cv::COLOR_BGR2GRAY);
            cv::threshold(temp, temp, 127, 255, cv::THRESH_OTSU);
            HG_OCR4* ptr_osd = reinterpret_cast<HG_OCR4*>(osd);
            int orientation = ptr_osd->getOrientation(temp.data, temp.cols, temp.rows, temp.channels(), temp.step);

            switch (orientation)
            {
            case 1:
                transpose(pDib, pDib);
                flip(pDib, pDib, 0);
                break;
            case 2:
                flip(pDib, pDib, 0);
                flip(pDib, pDib, 1);
                break;
            case 3:
                transpose(pDib, pDib);
                flip(pDib, pDib, 1);
                break;
            default:
                break;
            }
        }
#endif
	}
	else if (m_backTranspose && side == Rotate_90_clockwise)	//背面旋转180
	{
		if (m_rotation != Rotate_180)	//旋转180度
		{
			if (m_rotation == Rotate_90_clockwise || m_rotation == Rotate_90_anti_clockwise)	//90°  -90°
			{
				transpose(pDib, pDib);
				flip(pDib, pDib, m_rotation == Rotate_90_clockwise ? 0 : 1);
			}
			else
			{
				flip(pDib, pDib, 0);
				flip(pDib, pDib, 1);
			}
		}
	}
	else //zh
	{
		if (m_rotation == Rotate_90_clockwise || m_rotation == Rotate_90_anti_clockwise)	//90°  -90°
		{
			transpose(pDib, pDib);
			flip(pDib, pDib, m_rotation == Rotate_90_clockwise ? 1 : 0);
		}
		else if (m_rotation == Rotate_180)
		{
			flip(pDib, pDib, 0);
			flip(pDib, pDib, 1);
		}
	}
}
