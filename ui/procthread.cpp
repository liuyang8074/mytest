#include "procthread.h"
#include <ImageApply.h>
#include <ImageApplyAdjustColors.h>
#include <ImageApplyAutoCrop.h>
#include <ImageApplyBWBinaray.h>
#include <ImageApplyChannel.h>
#include <ImageApplyCrop.h>
#include <ImageApplyDiscardBlank.h>
#include <ImageApplyResize.h>
#include <ImageApplyRotation.h>
#include <opencv2/opencv.hpp>
#include "StopWatch.h"
#include "filetools.h"
#include <QDebug>
#include <Windows.h>
procthread::procthread(QObject* parent):QThread (parent)
  ,m_mode(READ_ANYCOLOR)
  ,m_isSave(false)
  ,m_isRunning(false)
  ,m_processNum(0)
{
    ci_adjustColor = new CImageApplyAdjustColors(0, 0, 1.0);
    ci_discardBlank = new CImageApplyDiscardBlank(false);
    ci_crop = new CImageApplyCrop();
    ci_resize = new CImageApplyResize();
    ci_channel = new CImageApplyChannel();
    ci_rotation = new CImageApplyRotation(CImageApplyRotation::Invalid);
    ci_binaray = new CImageApplyBWBinaray();
    ci_autoCrop = new CImageApplyAutoCrop(false,false,false,cv::Size(2448,3584));//?

    m_map_apply.insert(ci_discardBlank, false);
    m_map_apply.insert(ci_adjustColor, false);
    m_map_apply.insert(ci_crop, false);
    m_map_apply.insert(ci_resize, false);
    m_map_apply.insert(ci_channel, false);
    m_map_apply.insert(ci_rotation, false);
    m_map_apply.insert(ci_binaray, false);
    m_map_apply.insert(ci_autoCrop,false);
}

procthread::~procthread()
{
     stopProcess();

     delete ci_discardBlank;
     delete ci_adjustColor;
     delete ci_crop;
     delete ci_resize;
     delete ci_channel;
     delete ci_binaray;
     delete ci_rotation;
     delete ci_autoCrop;
}

void procthread::startProcess()
{
    m_isRunning = true;
    start();
}

void procthread::stopProcess()
{
    m_isRunning = false;
}

void procthread::setIsSave(bool isSave)
{
    m_isSave = isSave;
}
void procthread::setFileNames(const QStringList &filenames)
{
    m_filenames = QStringList(filenames);
}
void procthread::setSavePath(const QString &path)
{
    m_path_output = path;
}
void procthread::setReadMode(READ_MODES mode)
{
    m_mode = mode;
}
void procthread::setProcessEnabled(int index, bool enabled)
{
    switch (index)
        {
        case 0:
            m_map_apply[ci_discardBlank] = enabled;
            break;
        case 1:
            m_map_apply[ci_adjustColor] = enabled;
            break;
        case 2:
            m_map_apply[ci_crop] = enabled;
            break;
        case 3:
            m_map_apply[ci_resize] = enabled;
            break;
        case 4:
            m_map_apply[ci_channel] = enabled;
            break;
        case 5:
            m_map_apply[ci_rotation] = enabled;
            break;
        case 6:
            m_map_apply[ci_binaray] = enabled;
            break;
        case 7:
            m_map_apply[ci_autoCrop] = enabled;
        }
}

void procthread::setAdjustColorsParms(int Brightness, int Contrast, float Gamma)
{
    ci_adjustColor->setAdjustColors(Brightness, Contrast, Gamma);
}
void procthread::setCropRect(int x, int y, int w, int h)
{
    ci_crop->setROI(cv::Rect(x, y, w, h));
}
void procthread::setResizeType(int type)
{
    if (type == 1)
        ci_resize->setType(CImageApplyResize::RATIO);
    else
        ci_resize->setType(CImageApplyResize::DSIZE);
}
void procthread::setResizeScale(int size_x, int size_y)//按尺寸
{
    ci_resize->setDSize(cv::Size(size_x,size_y));
}
void procthread::setResizeScale(double ratio_x,double ratio_y)//按比例
{
    ci_resize->setFX(ratio_x);
    ci_resize->setFY(ratio_y);
}
void procthread::setChannel(int channel)
{
    switch (channel)
    {
    case 0:
        ci_channel->setChannel(CImageApplyChannel::Invalid);
        break;
    case 1:
        ci_channel->setChannel(CImageApplyChannel::Red);
        break;
    case 2:
        ci_channel->setChannel(CImageApplyChannel::Green);
        break;
    case 3:
        ci_channel->setChannel(CImageApplyChannel::Blue);
        break;
    case 4:
        ci_channel->setChannel(CImageApplyChannel::Except_Red);
        break;
    case 5:
        ci_channel->setChannel(CImageApplyChannel::Except_Green);
        break;
    case 6:
        ci_channel->setChannel(CImageApplyChannel::Except_Blue);
        break;
    }
}

void procthread::setRotation(int flip)
{
    switch (flip)
    {
    case 0:
        ci_rotation->setRotationType(CImageApplyRotation::Invalid);
        break;
    case 1:
        ci_rotation->setRotationType(CImageApplyRotation::Rotate_90_clockwise);
        break;
    case 2:
        ci_rotation->setRotationType(CImageApplyRotation::Rotate_180);
        break;
    case 3:
        ci_rotation->setRotationType(CImageApplyRotation::Rotate_90_anti_clockwise);
        break;
    case 4:
        ci_rotation->setRotationType(CImageApplyRotation::AutoTextOrientation);
        break;
    }
}

void procthread::setAutoCrop(bool enabled)
{
    ci_autoCrop->setAutoCrop(enabled);
}
void procthread::setFillBlank(bool enabled)
{
    ci_autoCrop->setFillBlank(enabled);
}
void procthread::setDesaskew(bool enabled)
{
    ci_autoCrop->setDesaskew(enabled);
}

void procthread::run()
{
    if(m_isRunning)
    {

        for (const QString& filename : m_filenames)
        {
            bool flag = false;

            if (!m_isRunning) break;
            cv::Mat image = cv::imread(filename.toStdString(), static_cast<int>(m_mode));

            StopWatch *watch = new StopWatch();//计时开始

            for (QMap<CImageApply*, bool>::iterator iterator = m_map_apply.begin(); iterator != m_map_apply.end(); iterator++)
            {
                if (iterator.value())
                {
                    iterator.key()->apply(image, 0);

                    if (!image.data)//跳过空白页
                    {
                        flag = true;
                        break;
                    }
                }
            }

            if (flag)   continue;

            double watch_ms = watch->elapsed_ms();//计算耗时
            std::string data = filename.toStdString()+"   "+QString::number(watch_ms).toStdString() + " ms";
            FileTools::write_log("record.txt",data);//向文本写入耗时数据

            if (m_isSave)
            {

                QDir saveDir(m_path_output);
                if (saveDir.exists())
                {
                    QString savePath = m_path_output + "/" + filename.right(filename.length() - filename.lastIndexOf('/') - 1);
                    cv::imwrite(savePath.toStdString(), image);
                }
                else {
                    cv::imwrite(filename.right(filename.length() - filename.lastIndexOf('/') - 1).toStdString(), image);
                }
            }
            m_processNum++;
            emit processNum(m_processNum);
        }
        m_processNum = 0;
    }

}

void procthread::getCPUusage()
{

}
