#include "testzokiiwindow.h"
#include "ui_testzokiiwindow.h"

TestZOKIIWindow::TestZOKIIWindow(QWidget *parent) : QWidget(parent),
    ui(new Ui::TestZOKIIWindow)
{
    ui->setupUi(this);
}

TestZOKIIWindow::~TestZOKIIWindow()
{
    delete ui;
}

void TestZOKIIWindow::on_comboBox_1_currentIndexChanged(int index)
{
    if(index>0){
        ui->label_1->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_1->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_1){
        ui->lineEdit_1->setText("");
    }
}


void TestZOKIIWindow::on_lineEdit_1_editingFinished()
{
    if((ui->lineEdit_1->text().toLongLong()<1)){
        ui->comboBox_1->setCurrentIndex(0);
    }
    if((ui->lineEdit_1->text().toLongLong()>0)&&(ui->lineEdit_1->text().toLongLong()<=50)){
        ui->comboBox_1->setCurrentIndex(1);
    }
    if((ui->lineEdit_1->text().toLongLong()>50)&&(ui->lineEdit_1->text().toLongLong()<=500)){
        ui->comboBox_1->setCurrentIndex(2);
    }
    if((ui->lineEdit_1->text().toLongLong()>500)){
        ui->comboBox_1->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_2_currentIndexChanged(int index)
{
    if(index>0){
        ui->label_2->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_2->setText("Значение: ");
    }
}

void TestZOKIIWindow::on_comboBox_3_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_3->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_3->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_3){
        ui->lineEdit_3->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_3_editingFinished()
{
    if((ui->lineEdit_3->text().toLongLong()<2)){
        ui->comboBox_3->setCurrentIndex(0);
    }
    if((ui->lineEdit_3->text().toLongLong()>1)&&(ui->lineEdit_3->text().toLongLong()<=999)){
        ui->comboBox_3->setCurrentIndex(1);
    }
    if((ui->lineEdit_3->text().toLongLong()>999)&&(ui->lineEdit_3->text().toLongLong()<=4999)){
        ui->comboBox_3->setCurrentIndex(2);
    }
    if((ui->lineEdit_3->text().toLongLong()>4999)){
        ui->comboBox_3->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_4_currentIndexChanged(int index)
{
    if(index>0){
        ui->label_4->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_4->setText("Значение: ");
    }
}

void TestZOKIIWindow::on_comboBox_5_currentIndexChanged(int index)
{
    if(index>0){
        ui->label_5->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_5->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_5){
        ui->lineEdit_5->setText("");
    }
}


void TestZOKIIWindow::on_lineEdit_5_editingFinished()
{
    if((ui->lineEdit_5->text().toLongLong()<2)){
        ui->comboBox_5->setCurrentIndex(0);
    }
    if((ui->lineEdit_5->text().toLongLong()>1)&&(ui->lineEdit_5->text().toLongLong()<=999)){
        ui->comboBox_5->setCurrentIndex(1);
    }
    if((ui->lineEdit_5->text().toLongLong()>999)&&(ui->lineEdit_5->text().toLongLong()<=4999)){
        ui->comboBox_5->setCurrentIndex(2);
    }
    if((ui->lineEdit_5->text().toLongLong()>4999)){
        ui->comboBox_5->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_6_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_6->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_6->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_6){
        ui->lineEdit_6->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_6_editingFinished()
{
    if((ui->lineEdit_6->text().toLongLong()<3)){
        ui->comboBox_6->setCurrentIndex(0);
    }
    if((ui->lineEdit_6->text().toLongLong()>2)&&(ui->lineEdit_6->text().toLongLong()<=999)){
        ui->comboBox_6->setCurrentIndex(1);
    }
    if((ui->lineEdit_6->text().toLongLong()>999)&&(ui->lineEdit_6->text().toLongLong()<=4999)){
        ui->comboBox_6->setCurrentIndex(2);
    }
    if((ui->lineEdit_6->text().toLongLong()>4999)){
        ui->comboBox_6->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_7_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_7->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_7->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_7){
        ui->lineEdit_7->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_7_editingFinished()
{
    if((ui->lineEdit_7->text().toLongLong()>24)){
        ui->comboBox_7->setCurrentIndex(0);
    }
    if((ui->lineEdit_7->text().toLongLong()>12)&&(ui->lineEdit_7->text().toLongLong()<=24)){
        ui->comboBox_7->setCurrentIndex(1);
    }
    if((ui->lineEdit_7->text().toLongLong()>6)&&(ui->lineEdit_7->text().toLongLong()<=12)){
        ui->comboBox_7->setCurrentIndex(2);
    }
    if((ui->lineEdit_7->text().toLongLong()<7)){
        ui->comboBox_7->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_8_currentIndexChanged(int index)
{
    if(index>0){
        ui->label_8->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_8->setText("Значение: ");
    }
}

void TestZOKIIWindow::on_comboBox_9_currentIndexChanged(int index)
{
    if(index>0){
        ui->label_9->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_9->setText("Значение: ");
    }
}

void TestZOKIIWindow::on_comboBox_10_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_10->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_10->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_10){
        ui->lineEdit_10->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_10_editingFinished()
{
    if((ui->lineEdit_10->text().toLongLong()<1)){
        ui->comboBox_10->setCurrentIndex(0);
    }
    if((ui->lineEdit_10->text().toLongLong()>0)&&(ui->lineEdit_10->text().toLongLong()<=10)){
        ui->comboBox_10->setCurrentIndex(1);
    }
    if((ui->lineEdit_10->text().toLongLong()>10)&&(ui->lineEdit_10->text().toLongLong()<=20)){
        ui->comboBox_10->setCurrentIndex(2);
    }
    if((ui->lineEdit_10->text().toLongLong()>20)){
        ui->comboBox_10->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_11_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_11->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_11->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_11){
        ui->lineEdit_11->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_11_editingFinished()
{
    if((ui->lineEdit_11->text().toFloat()<(0.001))){
        ui->comboBox_11->setCurrentIndex(0);
    }
    if((ui->lineEdit_11->text().toFloat()>(0.001))&&(ui->lineEdit_11->text().toFloat()<=(0.05))){
        ui->comboBox_11->setCurrentIndex(1);
    }
    if((ui->lineEdit_11->text().toFloat()>(0.05))&&(ui->lineEdit_11->text().toFloat()<=(0.1))){
        ui->comboBox_11->setCurrentIndex(2);
    }
    if((ui->lineEdit_11->text().toFloat()>(0.1))){
        ui->comboBox_11->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_12_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_12->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_12->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_12){
        ui->lineEdit_12->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_12_editingFinished()
{
    if((ui->lineEdit_12->text().toLongLong()<4)){
        ui->comboBox_12->setCurrentIndex(0);
    }
    if((ui->lineEdit_12->text().toLongLong()>3)&&(ui->lineEdit_12->text().toLongLong()<=70)){
        ui->comboBox_12->setCurrentIndex(1);
    }
    if((ui->lineEdit_12->text().toLongLong()>70)&&(ui->lineEdit_12->text().toLongLong()<=120)){
        ui->comboBox_12->setCurrentIndex(2);
    }
    if((ui->lineEdit_12->text().toLongLong()>120)){
        ui->comboBox_12->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_13_currentIndexChanged(int index)
{
    if(index>0){
        ui->label_13->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_13->setText("Значение: ");
    }
}

void TestZOKIIWindow::on_comboBox_14_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_14->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_14->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_14){
        ui->lineEdit_14->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_14_editingFinished()
{
    if((ui->lineEdit_14->text().toLongLong()<2)){
        ui->comboBox_14->setCurrentIndex(0);
    }
    if((ui->lineEdit_14->text().toLongLong()>1)&&(ui->lineEdit_14->text().toLongLong()<=999)){
        ui->comboBox_14->setCurrentIndex(1);
    }
    if((ui->lineEdit_14->text().toLongLong()>999)&&(ui->lineEdit_14->text().toLongLong()<=4999)){
        ui->comboBox_14->setCurrentIndex(2);
    }
    if((ui->lineEdit_14->text().toLongLong()>4999)){
        ui->comboBox_14->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_15_currentIndexChanged(int index)
{
    if(index>0){
        ui->label_15->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_15->setText("Значение: ");
    }
}

void TestZOKIIWindow::on_comboBox_16_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_16->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_16->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_16){
        ui->lineEdit_16->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_16_editingFinished()
{
    if((ui->lineEdit_16->text().toLongLong()<0)){
        ui->comboBox_16->setCurrentIndex(0);
    }
    if((ui->lineEdit_16->text().toLongLong()>0)&&(ui->lineEdit_16->text().toLongLong()<=10)){
        ui->comboBox_16->setCurrentIndex(1);
    }
    if((ui->lineEdit_16->text().toLongLong()>10)&&(ui->lineEdit_16->text().toLongLong()<=15)){
        ui->comboBox_16->setCurrentIndex(2);
    }
    if((ui->lineEdit_16->text().toLongLong()>15)){
        ui->comboBox_16->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_17_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_17->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_17->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_17){
        ui->lineEdit_17->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_17_editingFinished()
{
    if((ui->lineEdit_17->text().toLongLong()<0)){
        ui->comboBox_17->setCurrentIndex(0);
    }
    if((ui->lineEdit_17->text().toLongLong()>0)&&(ui->lineEdit_17->text().toLongLong()<=10)){
        ui->comboBox_17->setCurrentIndex(1);
    }
    if((ui->lineEdit_17->text().toLongLong()>10)&&(ui->lineEdit_17->text().toLongLong()<=40)){
        ui->comboBox_17->setCurrentIndex(2);
    }
    if((ui->lineEdit_17->text().toLongLong()>40)){
        ui->comboBox_17->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_comboBox_18_currentIndexChanged(int index)
{

    if(index>0){
        ui->label_18->setText("Значение: " + QString::number(4-index));
    }else {
        ui->label_18->setText("Значение: ");
    }
    if (this->focusWidget()==ui->comboBox_18){
        ui->lineEdit_18->setText("");
    }
}

void TestZOKIIWindow::on_lineEdit_18_editingFinished()
{
    if((ui->lineEdit_18->text().toLongLong()>4)){
        ui->comboBox_18->setCurrentIndex(0);
    }
    if((ui->lineEdit_18->text().toLongLong()>2)&&(ui->lineEdit_18->text().toLongLong()<=4)){
        ui->comboBox_18->setCurrentIndex(1);
    }
    if((ui->lineEdit_18->text().toLongLong()>1)&&(ui->lineEdit_18->text().toLongLong()<=2)){
        ui->comboBox_18->setCurrentIndex(2);
    }
    if((ui->lineEdit_18->text().toLongLong()<2)){
        ui->comboBox_18->setCurrentIndex(3);
    }
}

void TestZOKIIWindow::on_accept_clicked()
{
    int result = 0;
    if    (ui->comboBox_1->currentIndex() > result) {result = ui->comboBox_1->currentIndex();}
    if    (ui->comboBox_2->currentIndex() > result) {result = ui->comboBox_2->currentIndex();}
    if    (ui->comboBox_3->currentIndex() > result) {result = ui->comboBox_3->currentIndex();}
    if    (ui->comboBox_4->currentIndex() > result) {result = ui->comboBox_4->currentIndex();}
    if    (ui->comboBox_5->currentIndex() > result) {result = ui->comboBox_5->currentIndex();}
    if    (ui->comboBox_6->currentIndex() > result) {result = ui->comboBox_6->currentIndex();}
    if    (ui->comboBox_7->currentIndex() > result) {result = ui->comboBox_7->currentIndex();}
    if    (ui->comboBox_8->currentIndex() > result) {result = ui->comboBox_8->currentIndex();}
    if    (ui->comboBox_9->currentIndex() > result) {result = ui->comboBox_9->currentIndex();}
    if    (ui->comboBox_10->currentIndex() > result) {result = ui->comboBox_10->currentIndex();}
    if    (ui->comboBox_11->currentIndex() > result) {result = ui->comboBox_11->currentIndex();}
    if    (ui->comboBox_12->currentIndex() > result) {result = ui->comboBox_12->currentIndex();}
    if    (ui->comboBox_13->currentIndex() > result) {result = ui->comboBox_13->currentIndex();}
    if    (ui->comboBox_14->currentIndex() > result) {result = ui->comboBox_14->currentIndex();}
    if    (ui->comboBox_15->currentIndex() > result) {result = ui->comboBox_15->currentIndex();}
    if    (ui->comboBox_16->currentIndex() > result) {result = ui->comboBox_16->currentIndex();}
    if    (ui->comboBox_17->currentIndex() > result) {result = ui->comboBox_17->currentIndex();}
    if    (ui->comboBox_18->currentIndex() > result) {result = ui->comboBox_18->currentIndex();}
    if(result > 0){result = 4-result;}
    accpet_result(result);
}
