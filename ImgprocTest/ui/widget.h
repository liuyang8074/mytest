#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "procthread.h"
namespace Ui {
class Widget;
}
class procthread;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

    void on_checkBox_2_toggled(bool checked);

    void on_checkBox_3_toggled(bool checked);

    void on_checkBox_8_toggled(bool checked);

    void on_checkBox_4_toggled(bool checked);

    void on_checkBox_5_toggled(bool checked);

    void on_checkBox_6_toggled(bool checked);

    void on_checkBox_7_toggled(bool checked);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

    void slotDoubleSpinbox_slider();

    void slotslider_DoubleSpinBox();

    void processNum(int num);
    void on_spinBox_3_valueChanged(int arg1);

    void on_spinBox_4_valueChanged(int arg1);

    void on_spinBox_5_valueChanged(int arg1);

    void on_spinBox_6_valueChanged(int arg1);

    void on_radioButton_4_toggled(bool checked);

    void on_radioButton_5_toggled(bool checked);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_4_valueChanged(double arg1);

    void on_spinBox_7_valueChanged(int arg1);

    void on_spinBox_8_valueChanged(int arg1);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_currentIndexChanged(int index);

    void on_radioButton_3_toggled(bool checked);

    void on_checkBox_9_toggled(bool checked);

    void on_checkBox_10_toggled(bool checked);

    void on_checkBox_12_toggled(bool checked);

    void on_checkBox_11_toggled(bool checked);

private:
    Ui::Widget *ui;
    procthread *process_thread;
};

#endif // WIDGET_H
