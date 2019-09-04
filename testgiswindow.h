#ifndef TESTGISWINDOW_H
#define TESTGISWINDOW_H

#include <QWidget>

namespace Ui {
class TestGISWindow;
}

class TestGISWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TestGISWindow(QWidget *parent = nullptr);
    ~TestGISWindow();

signals:
    void accpet_result(int result);

public slots:
    void on_checkBox_1_clicked(bool checked);

    void on_checkBox_2_clicked(bool checked);

    void on_checkBox_3_clicked(bool checked);

    void on_checkBox_4_clicked(bool checked);

    void on_checkBox_5_clicked(bool checked);

    void on_checkBox_6_clicked(bool checked);

    void on_checkBox_7_clicked(bool checked);

    void on_checkBox_8_clicked(bool checked);

    void on_checkBox_9_clicked(bool checked);

    void on_checkBox_10_clicked(bool checked);

    void on_checkBox_11_clicked(bool checked);

    void on_checkBox_12_clicked(bool checked);

    void on_accept_clicked();

private:
    Ui::TestGISWindow *ui;
};

#endif // TESTGISWINDOW_H
