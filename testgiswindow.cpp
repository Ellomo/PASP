#include "testgiswindow.h"
#include "ui_testgiswindow.h"

TestGISWindow::TestGISWindow(QWidget *parent) : QWidget(parent),
    ui(new Ui::TestGISWindow)
{
    ui->setupUi(this);
}

TestGISWindow::~TestGISWindow()
{
    delete ui;
}

void TestGISWindow::on_checkBox_1_clicked(bool checked)
{
    if (checked){
        ui->checkBox_2->setChecked(false);
        ui->checkBox_3->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_2_clicked(bool checked)
{
    if (checked){
        ui->checkBox_1->setChecked(false);
        ui->checkBox_3->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_3_clicked(bool checked)
{
    if (checked){
        ui->checkBox_1->setChecked(false);
        ui->checkBox_2->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_4_clicked(bool checked)
{
    if (checked){
        ui->checkBox_5->setChecked(false);
        ui->checkBox_6->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_5_clicked(bool checked)
{
    if (checked){
        ui->checkBox_4->setChecked(false);
        ui->checkBox_6->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_6_clicked(bool checked)
{
    if (checked){
        ui->checkBox_4->setChecked(false);
        ui->checkBox_5->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_7_clicked(bool checked)
{
    if (checked){
        ui->checkBox_8->setChecked(false);
        ui->checkBox_9->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_8_clicked(bool checked)
{
    if (checked){
        ui->checkBox_7->setChecked(false);
        ui->checkBox_9->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_9_clicked(bool checked)
{
    if (checked){
        ui->checkBox_7->setChecked(false);
        ui->checkBox_8->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_10_clicked(bool checked)
{
    if (checked){
        ui->checkBox_11->setChecked(false);
        ui->checkBox_12->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_11_clicked(bool checked)
{
    if (checked){
        ui->checkBox_10->setChecked(false);
        ui->checkBox_12->setChecked(false);
    }
}

void TestGISWindow::on_checkBox_12_clicked(bool checked)
{
    if (checked){
        ui->checkBox_10->setChecked(false);
        ui->checkBox_11->setChecked(false);
    }
}

void TestGISWindow::on_accept_clicked()
{
    int YZi = 4;

    if (ui->checkBox_1->isChecked() || ui->checkBox_4->isChecked() || ui->checkBox_7->isChecked()){
        YZi = 1;
    } else {
        if(ui->checkBox_2->isChecked() || ui->checkBox_5->isChecked() || ui->checkBox_8->isChecked()){
            YZi = 2;
        } else {
            if(ui->checkBox_3->isChecked() || ui->checkBox_6->isChecked() || ui->checkBox_9->isChecked()){
                YZi = 3;
            }
        }
    }

    int result = 5;

    if (YZi == 1){
        result = 1;
    }

    if (YZi == 2){
        if (ui->checkBox_10->isChecked()){
            result = 1;
        } else {
            if (ui->checkBox_11->isChecked() || ui->checkBox_12->isChecked()){
                if (result > 2) {result = 2;}
            }
        }
    }

    if (YZi == 3){
        if (ui->checkBox_10->isChecked()){
            if (result > 2) {result = 2;}
        } else {
            if (ui->checkBox_11->isChecked() || ui->checkBox_12->isChecked()){
                if (result > 3) {result = 3;}
            }
        }
    }

    if (YZi == 4){
        if (ui->checkBox_10->isChecked() || ui->checkBox_11->isChecked() ){
            if (result > 3) {result = 3;}
        } else {
            if (ui->checkBox_12->isChecked() ){
                if (result > 4) {result = 4;}
            }
        }
    }

    if(result == 5){result = 0;}

    accpet_result(result);
}
