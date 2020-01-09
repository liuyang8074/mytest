#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QSpinBox>
#include <QSlider>
#include <Windows.h>

//CpuUsage usage;
//double cpuUsage;

DWORD WINAPI WatchItThreadProc(LPVOID lpParam);
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("ImgProcess TestTool");
    ui->tabWidget->setTabText(0,"基本功能");
    ui->tabWidget->setTabText(1,"色彩调节");
    ui->tabWidget->setTabText(2,"图像处理");
    ui->tabWidget->setTabText(3,"送纸");

    process_thread = new procthread(this);
//    CreateThread(NULL,0,WatchItThreadProc,NULL,0,NULL);

    connect(ui->checkBox, SIGNAL(toggled(bool)), process_thread, SLOT(setIsSave(bool)));
    connect(process_thread, SIGNAL(processNum(int)), this, SLOT(processNum(int)));

    connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),ui->spinBox_2,SLOT(setValue(int)));
    connect(ui->spinBox_2,SIGNAL(valueChanged(int)),ui->horizontalSlider_2,SLOT(setValue(int)));
    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotDoubleSpinbox_slider()));
    connect(ui->horizontalSlider_3, SIGNAL(valueChanged(int)), this, SLOT(slotslider_DoubleSpinBox()));

    ui->horizontalSlider->setEnabled(false);
    ui->horizontalSlider_2->setEnabled(false);
    ui->horizontalSlider_3->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->spinBox_2->setEnabled(false);
    ui->doubleSpinBox->setEnabled(false);
    ui->spinBox_3->setEnabled(false);
    ui->spinBox_4->setEnabled(false);
    ui->spinBox_5->setEnabled(false);
    ui->spinBox_6->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
    ui->radioButton_4->setEnabled(false);
    ui->radioButton_5->setEnabled(false);
    ui->doubleSpinBox_2->setEnabled(false);
    ui->doubleSpinBox_4->setEnabled(false);
    ui->spinBox_7->setEnabled(false);
    ui->spinBox_8->setEnabled(false);
}

//DWORD WINAPI WatchItThreadProc(LPVOID lpParam) {
//    while (true) {
//        cpuUsage = usage.GetUsage();
//        qDebug()<<cpuUsage;
//        Sleep(1000);
//    }
//}
Widget::~Widget()
{
    delete ui;
}
void Widget::slotDoubleSpinbox_slider()
{
    ui->horizontalSlider_3->setValue(static_cast<int>(ui->doubleSpinBox->value()*100));
}
void Widget::slotslider_DoubleSpinBox()
{
    ui->doubleSpinBox->setValue(static_cast<double>(ui->horizontalSlider_3->value())/100);
}
void Widget::on_pushButton_clicked()//选择原图路径
{
    QString dir_src = QFileDialog::getExistingDirectory(this,tr("选择原图路径"),"D:/");
    if (dir_src.isEmpty())
        return;
    ui->lineEdit->setText(dir_src);

    QStringList filenames;
    QDir dir(dir_src);
    dir.setFilter(QDir::Files);

    QStringList filters;
    filters << "*.bmp" << "*.jpg" << "*.jpeg" << "*.png";
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0, length = list.size(); i < length; i++)
    {
        QFileInfo fileInfo = list.at(i);
        filenames.append(fileInfo.absoluteFilePath());
        qDebug() << fileInfo.absoluteFilePath();
    }

    process_thread->setFileNames(filenames);
}

void Widget::on_pushButton_2_clicked()//选择保存路径
{
    QString dir_dst = QFileDialog::getExistingDirectory(this,tr("选择保存路径"),"D:/");
    if (dir_dst.isEmpty())
        return;
    ui->lineEdit_2->setText(dir_dst);

    process_thread->setSavePath(dir_dst);
}

void Widget::on_pushButton_3_clicked()//批处理
{
    process_thread->startProcess();

}

void Widget::on_radioButton_toggled(bool checked)//彩色
{
    if (checked)    process_thread->setReadMode(procthread::READ_COLOR);
}

void Widget::on_radioButton_2_toggled(bool checked)//灰度
{
    if (checked)    process_thread->setReadMode(procthread::READ_GRAYSCALE);
    ui->checkBox_5->setChecked(false);
    ui->checkBox_5->setEnabled(!checked);

}
void Widget::on_radioButton_3_toggled(bool checked)//任意
{
    if (checked)    process_thread->setReadMode(procthread::READ_ANYCOLOR);
}

void Widget::on_checkBox_2_toggled(bool checked)//discard blank
{
    process_thread->setProcessEnabled(0, checked);
}

void Widget::on_checkBox_8_toggled(bool checked)//adjustcolor
{
    process_thread->setProcessEnabled(1, checked);

    ui->horizontalSlider->setEnabled(checked);
    ui->horizontalSlider_2->setEnabled(checked);
    ui->horizontalSlider_3->setEnabled(checked);
    ui->spinBox->setEnabled(checked);
    ui->spinBox_2->setEnabled(checked);
    ui->doubleSpinBox->setEnabled(checked);
}

void Widget::on_checkBox_3_toggled(bool checked)//crop
{
    process_thread->setProcessEnabled(2, checked);

    ui->spinBox_3->setEnabled(checked);
    ui->spinBox_4->setEnabled(checked);
    ui->spinBox_5->setEnabled(checked);
    ui->spinBox_6->setEnabled(checked);

    if(!checked)
    {
        ui->spinBox_3->setValue(0);
        ui->spinBox_4->setValue(0);
        ui->spinBox_5->setValue(0);
        ui->spinBox_6->setValue(0);
    }
}

void Widget::on_checkBox_4_toggled(bool checked)//resize
{
    process_thread->setProcessEnabled(3, checked);

    ui->radioButton_4->setEnabled(checked);
    ui->radioButton_5->setEnabled(checked);
    ui->doubleSpinBox_2->setEnabled(checked);
    ui->doubleSpinBox_4->setEnabled(checked);
    ui->spinBox_7->setEnabled(checked);
    ui->spinBox_8->setEnabled(checked);

    if(!checked)
    {
        ui->doubleSpinBox_2->setValue(0);
        ui->doubleSpinBox_4->setValue(0);
        ui->spinBox_7->setValue(0);
        ui->spinBox_8->setValue(0);
    }
}

void Widget::on_checkBox_5_toggled(bool checked)//channel
{
    process_thread->setProcessEnabled(4, checked);

    ui->comboBox->setEnabled(checked);
}

void Widget::on_checkBox_6_toggled(bool checked)//rotation
{
    process_thread->setProcessEnabled(5, checked);

    ui->comboBox_2->setEnabled(checked);
}

void Widget::on_checkBox_7_toggled(bool checked)//binaray
{
    process_thread->setProcessEnabled(6, checked);
}
void Widget::on_checkBox_9_toggled(bool checked)
{
    process_thread->setProcessEnabled(7, checked);
}

void Widget::on_spinBox_valueChanged(int arg1)//亮度
{
    (void)arg1;
    process_thread->setAdjustColorsParms(ui->spinBox->value(),ui->spinBox_2->value(),static_cast<float>(ui->doubleSpinBox->value()));

}

void Widget::on_spinBox_2_valueChanged(int arg1)//对比度
{
    (void)arg1;
    process_thread->setAdjustColorsParms(ui->spinBox->value(),ui->spinBox_2->value(),static_cast<float>(ui->doubleSpinBox->value()));
}

void Widget::on_doubleSpinBox_valueChanged(double arg1)//伽马值
{
    (void)arg1;
    process_thread->setAdjustColorsParms(ui->spinBox->value(),ui->spinBox_2->value(),static_cast<float>(ui->doubleSpinBox->value()));
}

void Widget::processNum(int num)
{
    this->setWindowTitle("已处理张数：" + QString::number(num));
}


void Widget::on_spinBox_3_valueChanged(int arg1)
{
    (void)arg1;
    process_thread->setCropRect(ui->spinBox_3->value(), ui->spinBox_4->value(), ui->spinBox_5->value(), ui->spinBox_6->value());
    qDebug()<<"set crop"<<ui->spinBox_3->value()<<","<< ui->spinBox_4->value()<<","<< ui->spinBox_5->value()<<","<< ui->spinBox_6->value();
}

void Widget::on_spinBox_4_valueChanged(int arg1)
{
    (void)arg1;
    process_thread->setCropRect(ui->spinBox_3->value(), ui->spinBox_4->value(), ui->spinBox_5->value(), ui->spinBox_6->value());
    qDebug()<<"set crop"<<ui->spinBox_3->value()<<","<< ui->spinBox_4->value()<<","<< ui->spinBox_5->value()<<","<< ui->spinBox_6->value();
}

void Widget::on_spinBox_5_valueChanged(int arg1)
{
    (void)arg1;
    process_thread->setCropRect(ui->spinBox_3->value(), ui->spinBox_4->value(), ui->spinBox_5->value(), ui->spinBox_6->value());
    qDebug()<<"set crop"<<ui->spinBox_3->value()<<","<< ui->spinBox_4->value()<<","<< ui->spinBox_5->value()<<","<< ui->spinBox_6->value();
}

void Widget::on_spinBox_6_valueChanged(int arg1)
{
    (void)arg1;
    process_thread->setCropRect(ui->spinBox_3->value(), ui->spinBox_4->value(), ui->spinBox_5->value(), ui->spinBox_6->value());
    qDebug()<<"set crop"<<ui->spinBox_3->value()<<","<< ui->spinBox_4->value()<<","<< ui->spinBox_5->value()<<","<< ui->spinBox_6->value();
}

void Widget::on_radioButton_4_toggled(bool checked)
{
    if (checked)    process_thread->setResizeType(1);//ratio
}

void Widget::on_radioButton_5_toggled(bool checked)
{
    if (checked)    process_thread->setResizeType(2);//dsize
}

void Widget::on_doubleSpinBox_2_valueChanged(double arg1)
{
    (void)arg1;
    process_thread->setResizeScale(ui->doubleSpinBox_2->value(),ui->doubleSpinBox_4->value());
}

void Widget::on_doubleSpinBox_4_valueChanged(double arg1)
{
    (void)arg1;
    process_thread->setResizeScale(ui->doubleSpinBox_2->value(),ui->doubleSpinBox_4->value());
}

void Widget::on_spinBox_7_valueChanged(int arg1)
{
    (void)arg1;
    process_thread->setResizeScale(ui->spinBox_7->value(),ui->spinBox_8->value());
}

void Widget::on_spinBox_8_valueChanged(int arg1)
{
    (void)arg1;
    process_thread->setResizeScale(ui->spinBox_7->value(),ui->spinBox_8->value());
}

void Widget::on_comboBox_2_currentIndexChanged(int index)
{
    process_thread->setRotation(index);
}

void Widget::on_comboBox_currentIndexChanged(int index)
{
    process_thread->setChannel(index);
}


void Widget::on_checkBox_10_toggled(bool checked)
{
    process_thread->setAutoCrop(checked);
    ui->checkBox_12->setEnabled(!checked);
}

void Widget::on_checkBox_12_toggled(bool checked)
{
    process_thread->setFillBlank(checked);
    ui->checkBox_10->setEnabled(!checked);
}

void Widget::on_checkBox_11_toggled(bool checked)
{
    process_thread->setDesaskew(checked);
}


