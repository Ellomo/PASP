#ifndef TESTISPDNWINDOW_H
#define TESTISPDNWINDOW_H

#include <QWidget>

namespace Ui {
class TestISPDnWindow;
}

class TestISPDnWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TestISPDnWindow(QWidget *parent = nullptr);
    ~TestISPDnWindow();

signals:
    void accpet_result(int result);

public slots:

private slots:
    void on_accept_clicked();

    void on_checkBox_5_clicked(bool checked);

    void on_checkBox_6_clicked(bool checked);

    void on_checkBox_7_clicked(bool checked);

    void on_comboBox_4_currentIndexChanged(int index);

    void on_lineEdit_4_editingFinished();


private:
    Ui::TestISPDnWindow *ui;
};

#endif // TESTISPDNWINDOW_H
