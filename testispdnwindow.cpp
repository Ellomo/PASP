#include "testispdnwindow.h"
#include "ui_testispdnwindow.h"

TestISPDnWindow::TestISPDnWindow(QWidget *parent) : QWidget(parent),
    ui(new Ui::TestISPDnWindow)
{
    ui->setupUi(this);
}

TestISPDnWindow::~TestISPDnWindow()
{
    delete ui;
}

void TestISPDnWindow::on_checkBox_5_clicked(bool checked)
{
    if (checked){
        ui->checkBox_7->setChecked(false);
    }
}

void TestISPDnWindow::on_checkBox_6_clicked(bool checked)
{
    if (checked){
        ui->checkBox_7->setChecked(false);
    }
}

void TestISPDnWindow::on_checkBox_7_clicked(bool checked)
{
    if (checked){
        ui->checkBox_5->setChecked(false);
        ui->checkBox_6->setChecked(false);
    }
}

void TestISPDnWindow::on_comboBox_4_currentIndexChanged(int index)
{
    if (this->focusWidget()==ui->comboBox_4){
        ui->lineEdit_4->setText("");
    }
}

void TestISPDnWindow::on_lineEdit_4_editingFinished()
{
    if((ui->lineEdit_4->text().toLongLong()<0)){
        ui->comboBox_4->setCurrentIndex(0);
    }
    if((ui->lineEdit_4->text().toLongLong()>-1)&&(ui->lineEdit_4->text().toLongLong()<=99999)){
        ui->comboBox_4->setCurrentIndex(1);
    }
    if((ui->lineEdit_4->text().toLongLong()>99999)){
        ui->comboBox_4->setCurrentIndex(2);
    }
}

void TestISPDnWindow::on_accept_clicked()
{
    int result = 5;

    if (ui->checkBox_1->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_8->isChecked()){ // проверка категории субъектов
            if(ui->comboBox_4->currentIndex() == 2){ // проверка количества субъектов
                if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
                    result = 1;
                }else {
                    if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                        result = 1;
                    }else {
                        if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                            if (result > 2) {result = 2;}
                        }
                    }
                }
            }
        }
    }

    if (ui->checkBox_1->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_8->isChecked()){ // проверка категории субъектов
            if(ui->comboBox_4->currentIndex() == 1){ // проверка количества субъектов
                if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
                    result = 1;
                }else {
                    if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                        if (result > 2) {result = 2;}
                    }else {
                        if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                            if (result > 3) {result = 3;}
                        }
                    }
                }
            }
        }
    }

    if (ui->checkBox_1->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_9->isChecked()){ // проверка категории субъектов
            if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
                result = 1;
            }else {
                if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                    if (result > 2) {result = 2;}
                }else {
                    if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                        if (result > 3) {result = 3;}
                    }
                }
            }
        }
    }

    if (ui->checkBox_2->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
            result = 1;
        }else {
            if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                if (result > 2) {result = 2;}
            }else {
                if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                    if (result > 3) {result = 3;}
                }
            }
        }
    }

    if (ui->checkBox_3->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_8->isChecked()){ // проверка категории субъектов
            if(ui->comboBox_4->currentIndex() == 2){ // проверка количества субъектов
                if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
                    result = 1;
                }else {
                    if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                        if (result > 2) {result = 2;}
                    }else {
                        if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                            if (result > 3) {result = 3;}
                        }
                    }
                }
            }
        }
    }

    if (ui->checkBox_3->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_8->isChecked()){ // проверка категории субъектов
            if(ui->comboBox_4->currentIndex() == 1){ // проверка количества субъектов
                if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
                    result = 1;
                }else {
                    if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                        if (result > 3) {result = 3;}
                    }else {
                        if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                            if (result > 4) {result = 4;}
                        }
                    }
                }
            }
        }
    }

    if (ui->checkBox_3->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_9->isChecked()){ // проверка категории субъектов
            if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
                result = 1;
            }else {
                if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                    if (result > 3) {result = 3;}
                }else {
                    if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                        if (result > 4) {result = 4;}
                    }
                }
            }
        }
    }

    if (ui->checkBox_4->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_8->isChecked()){ // проверка категории субъектов
            if(ui->comboBox_4->currentIndex() == 2){ // проверка количества субъектов
                if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
                    if (result > 2) {result = 2;}
                }else {
                    if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                        if (result > 2) {result = 2;}
                    }else {
                        if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                            if (result > 4) {result = 4;}
                        }
                    }
                }
            }
        }
    }

    if (ui->checkBox_4->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_8->isChecked()){ // проверка категории субъектов
            if(ui->comboBox_4->currentIndex() == 1){ // проверка количества субъектов
                if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
                    if (result > 2) {result = 2;}
                }else {
                    if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                        if (result > 3) {result = 3;}
                    }else {
                        if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                            if (result > 4) {result = 4;}
                        }
                    }
                }
            }
        }
    }


    if (ui->checkBox_4->isChecked()){ // проверка категории ПДн
        if (ui->checkBox_9->isChecked()){ // проверка категории субъектов
            if (ui->checkBox_5->isChecked()){  // проверка актуальности угроз 1 типа
                if (result > 2) {result = 2;}
            }else {
                if (ui->checkBox_6->isChecked()){ // проверка актуальности угроз 2 типа
                    if (result > 3) {result = 3;}
                }else {
                    if (ui->checkBox_7->isChecked()){ // проверка актуальности угроз 3 типа
                        if (result > 4) {result = 4;}
                    }
                }
            }
        }
    }
    if(result == 5){result = 0;}
    accpet_result(result);
}
