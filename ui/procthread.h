#ifndef PROCTHREAD_H
#define PROCTHREAD_H

#include <QThread>
#include <QMap>
#include <QDir>
#include <Windows.h>
#include "CpuUsage.h"
class CImageApply;
class CImageApplyAdjustColors;
class CImageApplyBWBinaray;
class CImageApplyChannel;
class CImageApplyCrop;
class CImageApplyDiscardBlank;
class CImageApplyResize;
class CImageApplyRotation;
class CImageApplyAutoCrop;

class procthread: public QThread
{
    Q_OBJECT
public:

    enum READ_MODES
    {
        READ_GRAYSCALE = 0,
        READ_COLOR = 1,
        READ_ANYCOLOR = 4//位深不变，通道不变
    };

    procthread(QObject* parent = nullptr);

    ~procthread();

    void startProcess();

    void stopProcess();

public slots:
    void setIsSave(bool isSave);

    void setFileNames(const QStringList &filenames);

    void setSavePath(const QString &path);

    void setReadMode(READ_MODES mode);

    void setProcessEnabled(int index,bool enabled);

    void setAdjustColorsParms(int Brightness, int Contrast, float Gamma);

    void setCropRect(int x, int y, int w, int h);

    void setResizeType(int type);

    void setResizeScale(int size_x, int size_y);

    void setResizeScale(double ratio_x,double ratio_y);

    void setChannel(int channel);

    void setRotation(int flip);

    void setAutoCrop(bool enabled);

    void setFillBlank(bool enabled);

    void setDesaskew(bool enabled);

    void getCPUusage();

signals:

    void processNum(int num);

protected:

    void run();

private:
    READ_MODES m_mode;
    bool m_isSave;
    bool m_isRunning;
    QStringList m_filenames;
    QString m_path_output;
    QMap<CImageApply*, bool> m_map_apply;

    int m_processNum;

    CImageApplyAdjustColors *ci_adjustColor;
    CImageApplyBWBinaray *ci_binaray;
    CImageApplyChannel *ci_channel;
    CImageApplyCrop *ci_crop;
    CImageApplyDiscardBlank *ci_discardBlank;
    CImageApplyResize *ci_resize;
    CImageApplyRotation *ci_rotation;
    CImageApplyAutoCrop *ci_autoCrop;

};

#endif // PROCTHREAD_H
