#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProxyStyle>
#include <QStyleOptionTab>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QList>
#include <QIcon>
#include <QMessageBox>
#include <QObject>
#include <QTextEdit>

#include <ActiveQt/qaxobject.h>
#include <ActiveQt/qaxbase.h>

#include <QStyleOptionTab>
#include "customtabstyle.h"

#include "object.h"
#include "organization.h"
#include "testzokiiwindow.h"
#include "testispdnwindow.h"
#include "testgiswindow.h"
#include "widgetforcategory.h"
#include "qcustomplot.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:



    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private slots:

    void on_action_exit_triggered();

    void on_action_new_triggered();

    void on_action_open_triggered();

    void on_action_save_triggered();

    void on_action_saveAs_triggered();

    void on_toolButton_add_org_clicked();

    void on_toolButton_cancel_org_clicked();

    void on_lineEdit_orgName_textChanged(const QString &arg1);

    void on_toolButton_accept_org_clicked();

    void on_listWidget_org_itemClicked(QListWidgetItem *item);

    void on_toolButton_del_org_clicked();

    void on_listWidget_org_itemSelectionChanged();

    void on_toolButton_change_org_clicked();

    void on_toolButton_add_ob_clicked();

    void test_ZOKII();

    void close_test_ZOKII(int category);

    void test_ISPDn();

    void close_test_ISPDn(int category);

    void test_GIS();

    void close_test_GIS(int category);

    void on_toolButton_cancel_ob_clicked();

    void on_toolButton_accept_ob_clicked();

    void on_tableWidget_ob_itemSelectionChanged();

    void on_toolButton_del_ob_clicked();

    void on_toolButton_change_ob_clicked();

    void on_toolButton_add_arr_clicked();

    void on_toolButton_cancel_arr_clicked();

    void on_toolButton_accept_arr_clicked();

    void on_tableWidget_arr_itemSelectionChanged();

    void on_toolButton_del_arr_clicked();

    void on_toolButton_change_arr_clicked();

    void on_toolButton_ask_arr_clicked();

    void on_tableWidget_arr_itemDoubleClicked(QTableWidgetItem *item);

    void on_tableWidget_ob_itemDoubleClicked(QTableWidgetItem *item);

    void on_listWidget_org_itemDoubleClicked(QListWidgetItem *item);

    void notificationButton_clicked();

    void on_evaluation_1_valueChanged(const QString &arg1);

    void on_evaluation_2_valueChanged(const QString &arg1);

    void on_evaluation_3_valueChanged(const QString &arg1);

    void on_evaluation_4_valueChanged(const QString &arg1);

    void on_evaluation_5_valueChanged(const QString &arg1);

    void on_evaluation_6_valueChanged(const QString &arg1);

    void on_evaluation_7_valueChanged(const QString &arg1);

    void on_evaluation_8_valueChanged(const QString &arg1);

    void on_allowable_reduction_valueChanged(const QString &arg1);

    void on_information_security_status_valueChanged(const QString &arg1);

    void on_required_reliability_valueChanged(const QString &arg1);

private:

    QList<Organization> listOrg;
    QList<Object> listOb;

    WidgetForCategory* widgetForCategoryZOKII;
    WidgetForCategory* widgetForCategoryISPDn;
    WidgetForCategory* widgetForCategoryGIS;

    TestZOKIIWindow * testZOKIIWindow;
    TestISPDnWindow * testISPDnWindow;
    TestGISWindow * testGISWindow;

    int chengedObjetRow;
    int chengedObjet;
    int chengedArrangementRow;
    int chengedArrangement;

    QWidget* notificationWidget;
    QLabel* notificationLabel;
    QPushButton* notificationButton;
    QHBoxLayout* notificationHLayout;
    QString notificationMessage;

    void insertArr(Object object, QString name, QString form, QString classSZI, QString lvlSZI, QString classSVT, QString doc, QString comment);

    void checkStatus();

    void refreshNotification();

    void refreshEvaluatioResult();

    void refreshGraphic();

    void clearAllOrgObArr();

    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
