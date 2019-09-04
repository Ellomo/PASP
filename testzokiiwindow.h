#ifndef TESTZOKIIWINDOW_H
#define TESTZOKIIWINDOW_H

#include <QWidget>

namespace Ui {
class TestZOKIIWindow;
}

class TestZOKIIWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TestZOKIIWindow(QWidget *parent = nullptr);
    ~TestZOKIIWindow();

signals:
    void accpet_result(int result);

public slots:

private slots:
    void on_comboBox_1_currentIndexChanged(int index);

    void on_lineEdit_1_editingFinished();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_lineEdit_3_editingFinished();

    void on_comboBox_4_currentIndexChanged(int index);

    //void on_lineEdit_4_editingFinished();

    void on_comboBox_5_currentIndexChanged(int index);

    void on_lineEdit_5_editingFinished();

    void on_comboBox_6_currentIndexChanged(int index);

    void on_lineEdit_6_editingFinished();

    void on_comboBox_7_currentIndexChanged(int index);

    void on_lineEdit_7_editingFinished();

    void on_comboBox_8_currentIndexChanged(int index);

    //void on_lineEdit_8_editingFinished();

    void on_comboBox_9_currentIndexChanged(int index);

    //void on_lineEdit_9_editingFinished();

    void on_comboBox_10_currentIndexChanged(int index);

    void on_lineEdit_10_editingFinished();

    void on_comboBox_11_currentIndexChanged(int index);

    void on_lineEdit_11_editingFinished();

    void on_comboBox_12_currentIndexChanged(int index);

    void on_lineEdit_12_editingFinished();

    void on_comboBox_13_currentIndexChanged(int index);

    //void on_lineEdit_13_editingFinished();

    void on_comboBox_14_currentIndexChanged(int index);

    void on_lineEdit_14_editingFinished();

    void on_comboBox_15_currentIndexChanged(int index);

    //void on_lineEdit_15_editingFinished();

    void on_comboBox_16_currentIndexChanged(int index);

    void on_lineEdit_16_editingFinished();

    void on_comboBox_17_currentIndexChanged(int index);

    void on_lineEdit_17_editingFinished();

    void on_comboBox_18_currentIndexChanged(int index);

    void on_lineEdit_18_editingFinished();

    void on_accept_clicked();

private:
    Ui::TestZOKIIWindow *ui;

};

#endif // TESTZOKIIWINDOW_H
