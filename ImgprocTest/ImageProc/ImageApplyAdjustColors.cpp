#include "ImageApplyAdjustColors.h"

CImageApplyAdjustColors::CImageApplyAdjustColors(void)
	: m_brightness(0)
	, m_contrast(0)
	, m_gamma(1.0f)
	, lut(1, 256, CV_8UC1)
{
	update_lutData();
}

CImageApplyAdjustColors::CImageApplyAdjustColors(int brightness, int contrast, float gamma)
	: lut(1, 256, CV_8UC1)
{
	setAdjustColors(brightness, contrast, gamma);
}

CImageApplyAdjustColors::~CImageApplyAdjustColors(void)
{

}

void CImageApplyAdjustColors::apply(cv::Mat& pDib,int side)
{
#ifdef LOG
	FileTools::write_log("imgprc.txt", "enter CImageApplyAdjustColors apply");
#endif // LOG

	if (m_brightness != 0 || m_contrast != 0 || m_gamma != 1.0)
		cv::LUT(pDib, lut, pDib);
#ifdef LOG
	FileTools::write_log("imgprc.txt", "exit CImageApplyAdjustColors apply");
#endif // LOG
}

void CImageApplyAdjustColors::setAdjustColors(int brightness, int contrast, float gamma)
{
	setBrightness(brightness);
	setContrast(contrast);
	setGamma(gamma);
	update_lutData();
}

void CImageApplyAdjustColors::setBrightness(int brightness)
{
	m_brightness = cv::max(-255, cv::min(brightness, 255)); 
	update_lutData();
}

void CImageApplyAdjustColors::setContrast(int contrast)
{
	m_contrast = cv::max(-127, cv::min(contrast, 127));
	update_lutData();
}

void CImageApplyAdjustColors::setGamma(float gamma)
{
	m_gamma = cv::max(0.1f, cv::min(gamma, 5.0f));
	update_lutData();
}

void CImageApplyAdjustColors::update_lutData()
{
	unsigned char* ptr = lut.data;

	if (m_gamma != 1.0f)
	{
		float g = 1.0f / m_gamma;
		for (int i = 0; i < 256; i++)
			ptr[i] = static_cast<unsigned char>(cv::min(255, static_cast<int>(cv::pow(i / 255.0f, g) * 255.0f + 0.5f)));
	}
	else
	{
		for (int i = 0; i < 256; i++)
		{
			//update contrast
			if (i < 128)
				ptr[i] = static_cast<unsigned char>(cv::max(0, cv::min(i - m_contrast, 127)));
			else
				ptr[i] = static_cast<unsigned char>(cv::max(127, cv::min(i + m_contrast, 255)));

			//update brightness
			ptr[i] = static_cast<unsigned char>(cv::max(0, cv::min(ptr[i] + m_brightness, 255)));
		}
	}
}
