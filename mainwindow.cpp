#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //------------ боковой виджет

    notificationWidget = new QWidget(this);


    notificationLabel = new QLabel(notificationWidget);

    notificationLabel->setText("Уведомлений:");


    notificationButton = new QPushButton(notificationWidget);

    notificationButton->setText("0");

    notificationButton->setMaximumSize(QSize(30, 30));


    notificationHLayout = new QHBoxLayout(notificationWidget);

    notificationHLayout->addWidget(notificationLabel);

    notificationHLayout->addWidget(notificationButton);

    connect(notificationButton, SIGNAL(clicked()), this, SLOT(notificationButton_clicked()));

    ui->tabWidget->setCornerWidget(notificationWidget, Qt::BottomRightCorner);

    ui->tableWidget_ob->verticalHeader()->setVisible(1);

    ui->grafic->addGraph();
    ui->grafic->graph(0)->setPen(QPen(Qt::red));
    //
    ui->grafic->addGraph();
    ui->grafic->graph(1)->setPen(QPen(Qt::darkRed));
    refreshGraphic();
}

MainWindow::~MainWindow()
{
    ui->statusBar->showMessage("Выход");
    delete ui;
}

void MainWindow::on_action_exit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_new_triggered()
{
    ui->statusBar->showMessage("Создание нового проекта");
    if (!(QMessageBox::warning(0,
                               "Подтверждение действия",
                               "Выбранное вами действие приведет к потере несохраненного прогресса! Желаете продолжить?",
                               "Да",
                               "Нет",
                               QString(),
                               0,
                               1
                               ))){
        clearAllOrgObArr();
    }
}

void MainWindow::on_action_open_triggered()
{
    ui->statusBar->showMessage("Открытие проекта");

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QString::fromUtf8("Открыть файл"),
                                                    QDir::currentPath(),
                                                    "Microsoft Excel File (*.xls *.xlsx );;All files (*.*)");
    if (!fileName.isEmpty()){
        if (!(QMessageBox::warning(0,
                                   "Подтверждение действия",
                                   "Выбранное вами действие приведет к потере несохраненного прогресса! Желаете продолжить?",
                                   "Да",
                                   "Нет",
                                   QString(),
                                   0,
                                   1
                                   ))){
            clearAllOrgObArr();

            QAxObject *excel = new QAxObject( "Excel.Application",this);
            //excel->setProperty("Visible", true);                                            //видимость
            QAxObject *wbooks = excel->querySubObject( "Workbooks" );                       //книга
            QAxObject *shab = wbooks->querySubObject("Open(const QString&)", fileName );    //файл
            QAxObject *mSheets = shab->querySubObject("Sheets");                            //листы
            QAxObject *StatSheet = mSheets->querySubObject("Item(1)");                      //текущий лист
            StatSheet->dynamicCall("Select()");

            int IndexOrg = 2;

            QAxObject* cell_NameOrg = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 1);
            QAxObject* cell_TypeOrg = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 2);
            QAxObject* cell_INN = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 3);
            QAxObject* cell_ORGN = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 4);
            QAxObject* cell_YAdr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 5);
            QAxObject* cell_FAdr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 6);

            QString val_cell_NameOrg = cell_NameOrg->property("Value").toString();
            QString val_cell_TypeOrg = cell_TypeOrg->property("Value").toString();
            QString val_cell_INN = cell_INN->property("Value").toString();
            QString val_cell_ORGN = cell_ORGN->property("Value").toString();
            QString val_cell_YAdr = cell_YAdr->property("Value").toString();
            QString val_cell_FAdr = cell_FAdr->property("Value").toString();

            while ((!val_cell_NameOrg.isEmpty())||(!val_cell_TypeOrg.isEmpty())||(!val_cell_INN.isEmpty())||(!val_cell_ORGN.isEmpty())||(!val_cell_YAdr.isEmpty())||(!val_cell_FAdr.isEmpty())){
                ui->listWidget_org->insertItem(ui->listWidget_org->count(), val_cell_NameOrg);
                Organization *org = new Organization();

                org->Name = *ui->listWidget_org->item(ui->listWidget_org->count()-1);
                org->Type = val_cell_TypeOrg;
                org->INN = val_cell_INN;
                org->ORGN = val_cell_ORGN;
                org->YAdr = val_cell_YAdr;
                org->FAdr = val_cell_FAdr;
                listOrg.append(*org);

                IndexOrg++;

                cell_NameOrg = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 1);
                cell_TypeOrg = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 2);
                cell_INN = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 3);
                cell_ORGN = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 4);
                cell_YAdr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 5);
                cell_FAdr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOrg, 6);

                val_cell_NameOrg = cell_NameOrg->property("Value").toString();
                val_cell_TypeOrg = cell_TypeOrg->property("Value").toString();
                val_cell_INN = cell_INN->property("Value").toString();
                val_cell_ORGN = cell_ORGN->property("Value").toString();
                val_cell_YAdr = cell_YAdr->property("Value").toString();
                val_cell_FAdr = cell_FAdr->property("Value").toString();
            }

            // освобождение памяти
            delete cell_TypeOrg;
            delete cell_INN;
            delete cell_ORGN;
            delete cell_YAdr;
            delete cell_FAdr;

            StatSheet = mSheets->querySubObject("Item(2)");                      //текущий лист

            int IndexOb = 2;

            cell_NameOrg = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 1);
            QAxObject* cell_NameOb = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 2);
            QAxObject* cell_TypeOb = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 3);
            QAxObject* cell_ActivityOrg = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 4);
            QAxObject* cell_Status = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 5);
            QAxObject* cell_CatZOKII = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 6);
            QAxObject* cell_CatISPDn = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 7);
            QAxObject* cell_CatGIS = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 8);

            val_cell_NameOrg = cell_NameOrg->property("Formula").toString();
            QString val_cell_NameOb = cell_NameOb->property("Value").toString();
            QString val_cell_TypeOb = cell_TypeOb->property("Value").toString();
            QString val_cell_ActivityOrg = cell_ActivityOrg->property("Value").toString();
            QString val_cell_Status = cell_Status->property("Value").toString();
            QString val_cell_CatZOKII = cell_CatZOKII->property("Value").toString();
            QString val_cell_CatISPDn = cell_CatISPDn->property("Value").toString();
            QString val_cell_CatGIS = cell_CatGIS->property("Value").toString();

            ui->tableWidget_ob->setSortingEnabled(false);

            while ((!val_cell_NameOrg.isEmpty())||(!val_cell_NameOb.isEmpty())||(!val_cell_TypeOb.isEmpty())||(!val_cell_ActivityOrg.isEmpty())||(!val_cell_Status.isEmpty())||(!val_cell_CatZOKII.isEmpty())||(!val_cell_CatISPDn.isEmpty())||(!val_cell_CatGIS.isEmpty())){
                ui->tableWidget_ob->insertRow(ui->tableWidget_ob->rowCount());
                Object *ob = new Object();
                if (!val_cell_NameOrg.isEmpty()){

                    //val_cell_NameOrg = val_cell_NameOrg.replace("=Организации!$A$", "");
                    IndexOrg = val_cell_NameOrg.replace("=Организации!$A$", "").toInt()-2;
                    //IndexOrg = val_cell_NameOrg.replace("=Организации!R", "").replace("C1", "").toInt()-2;
                    ob->nameOrg = ui->listWidget_org->item(IndexOrg);
                    ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,0, new QTableWidgetItem(ob->nameOrg->text()));
                    ui->tableWidget_ob->item(ui->tableWidget_ob->rowCount()-1,0)->setIcon(ui->comboBox_orgType->itemIcon(ui->comboBox_orgType->findText(listOrg[IndexOrg].Type)));
                }else{
                    ob->nameOrg = NULL;
                    ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,0, new QTableWidgetItem("Укажите организацию"));
                    ui->tableWidget_ob->item(ui->tableWidget_ob->rowCount()-1,0)->setIcon(ui->comboBox_orgType->itemIcon(0));
                }
                ob->objectTableRow = ui->tableWidget_ob->item(ui->tableWidget_ob->rowCount()-1,0);
                ob->name = val_cell_NameOb;
                ob->type = val_cell_TypeOb;
                ob->activityOrg = val_cell_ActivityOrg;
                ob->status = val_cell_Status;
                ob->catZOKII = val_cell_CatZOKII;
                ob->catISPDn = val_cell_CatISPDn;
                ob->catGIS = val_cell_CatGIS;

                ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,1, new QTableWidgetItem(ob->name));
                ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,2, new QTableWidgetItem(ob->type));
                ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,3, new QTableWidgetItem(ob->activityOrg));
                ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,4, new QTableWidgetItem(ob->status));
                ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,5, new QTableWidgetItem(ob->catZOKII));
                ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,6, new QTableWidgetItem(ob->catISPDn));
                ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,7, new QTableWidgetItem(ob->catGIS));

                listOb.append(*ob);

                IndexOb++;

                cell_NameOrg = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 1);
                cell_NameOb = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 2);
                cell_TypeOb = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 3);
                cell_ActivityOrg = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 4);
                cell_Status = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 5);
                cell_CatZOKII = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 6);
                cell_CatISPDn = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 7);
                cell_CatGIS = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexOb, 8);

                val_cell_NameOrg = cell_NameOrg->property("Formula").toString();
                val_cell_NameOb = cell_NameOb->property("Value").toString();
                val_cell_TypeOb = cell_TypeOb->property("Value").toString();
                val_cell_ActivityOrg = cell_ActivityOrg->property("Value").toString();
                val_cell_Status = cell_Status->property("Value").toString();
                val_cell_CatZOKII = cell_CatZOKII->property("Value").toString();
                val_cell_CatISPDn = cell_CatISPDn->property("Value").toString();
                val_cell_CatGIS = cell_CatGIS->property("Value").toString();
            }



            // освобождение памяти
            delete cell_NameOrg;
            delete cell_NameOb;
            delete cell_TypeOb;
            delete cell_ActivityOrg;
            delete cell_Status;
            delete cell_CatZOKII;
            delete cell_CatISPDn;
            delete cell_CatGIS;

            int countAllItem = mSheets->property("Count").toInt();

            for (int countItem = 3 ; countItem < countAllItem + 1 ; countItem++){

                StatSheet = mSheets->querySubObject("Item(int)", countItem);                      //текущий лист

                QAxObject* cell_IndexOb = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 2);
                QString val_cell_IndexOb = cell_IndexOb->property("Formula").toString();
                IndexOb = val_cell_IndexOb.replace("=Объекты!$B$", "").toInt()-2;

                int IndexArr = 3;

                QAxObject* cell_NameArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 1);
                QAxObject* cell_FormArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 2);
                QAxObject* cell_ClassSZIArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 3);
                QAxObject* cell_LvlSZIArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 4);
                QAxObject* cell_ClassSVTArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 5);
                QAxObject* cell_DocArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 6);
                QAxObject* cell_CommentArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 7);


                QString val_cell_NameArr = cell_NameArr->property("Value").toString();
                QString val_cell_FormArr = cell_FormArr->property("Value").toString();
                QString val_cell_ClassSZIArr = cell_ClassSZIArr->property("Value").toString();
                QString val_cell_LvlSZIArr = cell_LvlSZIArr->property("Value").toString();
                QString val_cell_ClassSVTArr = cell_ClassSVTArr->property("Value").toString();
                QString val_cell_DocArr = cell_DocArr->property("Value").toString();
                QString val_cell_CommentArr = cell_CommentArr->property("Value").toString();

                while ((!val_cell_NameArr.isEmpty())||(!val_cell_FormArr.isEmpty())||(!val_cell_ClassSZIArr.isEmpty())||(!val_cell_LvlSZIArr.isEmpty())||(!val_cell_ClassSVTArr.isEmpty())||(!val_cell_DocArr.isEmpty())||(!val_cell_CommentArr.isEmpty())){

                    listOb[IndexOb].profile[0]->append(new QTableWidgetItem(val_cell_NameArr));
                    listOb[IndexOb].profile[1]->append(new QTableWidgetItem(val_cell_FormArr));
                    listOb[IndexOb].profile[2]->append(new QTableWidgetItem(val_cell_ClassSZIArr));
                    listOb[IndexOb].profile[3]->append(new QTableWidgetItem(val_cell_LvlSZIArr));
                    listOb[IndexOb].profile[4]->append(new QTableWidgetItem(val_cell_ClassSVTArr));
                    listOb[IndexOb].profile[5]->append(new QTableWidgetItem(val_cell_DocArr));
                    listOb[IndexOb].profile[6]->append(new QTableWidgetItem(val_cell_CommentArr));

                    IndexArr++;

                    cell_NameArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 1);
                    cell_FormArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 2);
                    cell_ClassSZIArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 3);
                    cell_LvlSZIArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 4);
                    cell_ClassSVTArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 5);
                    cell_DocArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 6);
                    cell_CommentArr = StatSheet->querySubObject("Cells(QVariant,QVariant)", IndexArr, 7);

                    val_cell_NameArr = cell_NameArr->property("Value").toString();
                    val_cell_FormArr = cell_FormArr->property("Value").toString();
                    val_cell_ClassSZIArr = cell_ClassSZIArr->property("Value").toString();
                    val_cell_LvlSZIArr = cell_LvlSZIArr->property("Value").toString();
                    val_cell_ClassSVTArr = cell_ClassSVTArr->property("Value").toString();
                    val_cell_DocArr = cell_DocArr->property("Value").toString();
                    val_cell_CommentArr = cell_CommentArr->property("Value").toString();

                }

                // освобождение памяти
                delete cell_IndexOb;
                delete cell_NameArr;
                delete cell_FormArr;
                delete cell_ClassSZIArr;
                delete cell_LvlSZIArr;
                delete cell_ClassSVTArr;
                delete cell_DocArr;
                delete cell_CommentArr;

            }

            //закрытие
            delete StatSheet;
            delete mSheets;
            delete shab;//wbook;
            delete wbooks;//wbooks;
            excel->dynamicCall("Quit()");
            delete excel;


            refreshNotification();
            ui->tableWidget_ob->setSortingEnabled(true);


        }
    }
}

void MainWindow::clearAllOrgObArr(){
    while (listOrg.count()!=0){
        listOrg.removeAt(0);
    }
    for (int i = ui->listWidget_org->count()-1 ; i >-1 ; i--){
        delete ui->listWidget_org->takeItem(i);
    }
    ui->lineEdit_orgName->setText("");
    ui->comboBox_orgType->setCurrentText("Укажите тип");
    ui->lineEdit_orgINN->setText("");
    ui->lineEdit_orgORGN->setText("");
    ui->lineEdit_orgYAdr->setText("");
    ui->lineEdit_orgFAdr->setText("");
    ui->toolButton_add_org->setEnabled(true);
    ui->toolButton_del_org->setEnabled(false);
    ui->toolButton_change_org->setEnabled(false);
    ui->toolButton_accept_org->setEnabled(false);
    ui->toolButton_cancel_org->setEnabled(false);
    ui->scrollAreaWidgetContents_org->setEnabled(false);
    ui->listWidget_org->setEnabled(true);
    ui->label_statusAction_org->setText("");

    while (listOb.count()!=0){
        listOb.removeAt(0);
    }
    for (int i = ui->tableWidget_ob->rowCount()-1 ; i>-1 ; i--){
        ui->tableWidget_ob->removeRow(i);
    }
    ui->toolButton_add_ob->setEnabled(true);
    ui->toolButton_del_ob->setEnabled(false);
    ui->toolButton_change_ob->setEnabled(false);
    ui->toolButton_accept_ob->setEnabled(false);
    ui->toolButton_cancel_ob->setEnabled(false);
    ui->tableWidget_ob->setSortingEnabled(true);
    ui->label_statusAction_ob->setText("");

    for (int i = ui->tableWidget_arr->rowCount()-1 ; i>-1 ; i--){
        ui->tableWidget_arr->removeRow(i);
    }
    ui->toolButton_add_arr->setEnabled(true);
    ui->toolButton_del_arr->setEnabled(false);
    ui->toolButton_change_arr->setEnabled(false);
    ui->toolButton_accept_arr->setEnabled(false);
    ui->toolButton_cancel_arr->setEnabled(false);
    ui->toolButton_ask_arr->setEnabled(true);
    ui->widget_ob->setEnabled(true);
    ui->label_statusAction_arr->setText("");
    notificationButton->setText("0");
}

void MainWindow::on_action_save_triggered()
{
    ui->statusBar->showMessage("Сохранение проекта проекта");
}

void MainWindow::on_action_saveAs_triggered()
{
    ui->statusBar->showMessage("Сохранение проекта проекта как...");

    QAxObject *excel = new QAxObject( "Excel.Application",this);
    QAxObject *wbooks = excel->querySubObject( "Workbooks" );                       //книга
    QAxObject *shab = wbooks->querySubObject("Add()");                              //файл
    QAxObject *mSheets = shab->querySubObject("Sheets");                            //листы
    mSheets->querySubObject("Add");
    for (int i = 0 ; i < listOb.count() ; i++){
        if(listOb[i].profile[0]->count()>0){
            mSheets->querySubObject("Add");
        }
    }
    QAxObject *StatSheet = mSheets->querySubObject("Item(1)");                      //текущий лист

    StatSheet->setProperty("Name", "Организации");
    StatSheet->dynamicCall("Select()");

    QAxObject *cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 1);
    cell->setProperty("Value", QVariant("Название организации"));
    cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 2);
    cell->setProperty("Value", QVariant("Тип организации"));
    cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 3);
    cell->setProperty("Value", QVariant("ИНН"));
    cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 4);
    cell->setProperty("Value", QVariant("ОРГН"));
    cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 5);
    cell->setProperty("Value", QVariant("Юридический адресс"));
    cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 6);
    cell->setProperty("Value", QVariant("Физический адресс"));
    for( int k=2; k < listOrg.count()+2 ; k++ )
    {
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 1);
        cell->setProperty("Value", QVariant(listOrg[k-2].Name.text()));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 2);
        cell->setProperty("Value", QVariant(listOrg[k-2].Type));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 3);
        cell->setProperty("Value", QVariant(listOrg[k-2].INN));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 4);
        cell->setProperty("Value", QVariant(listOrg[k-2].ORGN));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 5);
        cell->setProperty("Value", QVariant(listOrg[k-2].YAdr));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 6);
        cell->setProperty("Value", QVariant(listOrg[k-2].FAdr));
    }

    StatSheet = mSheets->querySubObject("Item(2)");
    StatSheet->setProperty("Name", "Объекты");

    for( int i = 1 ; i < ui->tableWidget_ob->horizontalHeader()->count()+1 ; i++ )
    {
        QVariant header = ui->tableWidget_ob->model()->headerData(i-1, Qt::Horizontal);
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, i);
        cell->setProperty("Value", QVariant(header));
    }


    for( int k=2; k < listOb.count()+2 ; k++ )
    {
        if (listOb[k-2].nameOrg != NULL){
            cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 1);
            int indexOrg = 0;
            for (int i = 0 ; i < ui->listWidget_org->count() ; i++){
                if (ui->listWidget_org->item(i) == listOb[k-2].nameOrg){
                    indexOrg = i;
                }
            }
            cell->setProperty("Value", QVariant("=Организации!R"+QString::number(indexOrg+2)+"C1"));
        }
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 2);
        cell->setProperty("Value", QVariant(listOb[k-2].name));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 3);
        cell->setProperty("Value", QVariant(listOb[k-2].type));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 4);
        cell->setProperty("Value", QVariant(listOb[k-2].activityOrg));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 5);
        cell->setProperty("Value", QVariant(listOb[k-2].status));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 6);
        cell->setProperty("Value", QVariant(listOb[k-2].catZOKII));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 7);
        cell->setProperty("Value", QVariant(listOb[k-2].catISPDn));
        cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", k, 8);
        cell->setProperty("Value", QVariant(listOb[k-2].catGIS));
    }
    int countItem = 3;
    for (int k = 0 ; k < listOb.count() ; k++){
        if(listOb[k].profile[0]->count()>0){

            StatSheet = mSheets->querySubObject("Item(int)", countItem);
            StatSheet->setProperty("Name", "Профиль мер №" + QString::number(countItem-1) + " объекта \""+ listOb[k].name + "\"");

            cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 1);
            cell->setProperty("Value", QVariant("Профиль мер объекта"));
            cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 1, 2);
            cell->setProperty("Value", QVariant("=Объекты!R"+QString::number(k+2)+"C2"));

            for( int i = 1 ; i < ui->tableWidget_arr->horizontalHeader()->count()+1 ; i++ )
            {
                QVariant header = ui->tableWidget_arr->model()->headerData(i-1, Qt::Horizontal);
                cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", 2, i);
                cell->setProperty("Value", QVariant(header));
            }

            for(int i=0; i < listOb[k].profile[0]->count() ;i++)//строки
            {
                for( int j = 0 ; j < listOb[k].profile.count() ; j++ )//столбцы
                {
                    cell = StatSheet->querySubObject("Cells(QVariant,QVariant)", i+3, j+1);
                    cell->setProperty("Value", QVariant(listOb[k].profile[j]->at(i)->text()));
                }

            }

            countItem++;
        }
    }

    // освобождение памяти
    delete cell;
    delete StatSheet;
    excel->setProperty("Visible", true);                                            //видимость
    mSheets->dynamicCall("Save()");
    delete mSheets;
    delete shab;//wbook;
    delete wbooks;//wbooks;
    excel->dynamicCall("Quit()");
    //ui->name_excel_file->setText("");
    delete excel;

}

void MainWindow::on_toolButton_add_org_clicked()
{
    ui->lineEdit_orgName->setText("");
    ui->comboBox_orgType->setCurrentText("Укажите тип");
    ui->lineEdit_orgINN->setText("");
    ui->lineEdit_orgORGN->setText("");
    ui->lineEdit_orgYAdr->setText("");
    ui->lineEdit_orgFAdr->setText("");

    ui->toolButton_add_org->setEnabled(false);
    ui->toolButton_del_org->setEnabled(false);
    ui->toolButton_change_org->setEnabled(false);
    ui->toolButton_accept_org->setEnabled(true);
    ui->toolButton_cancel_org->setEnabled(true);
    ui->label_statusAction_org->setText("Добавление новой организации");
    ui->scrollAreaWidgetContents_org->setEnabled(true);
    ui->listWidget_org->setEnabled(false);

    for (int i = 0; i<ui->listWidget_org->count(); i++) {
        ui->listWidget_org->item(i)->setSelected(false);
    }


    QListWidgetItem* new_org_item = new QListWidgetItem("Новая организация");
    ui->listWidget_org->addItem(new_org_item);
    ui->listWidget_org->setCurrentItem(new_org_item);
}

void MainWindow::on_toolButton_cancel_org_clicked()
{
    if (ui->label_statusAction_org->text() == "Добавление новой организации"){

        ui->lineEdit_orgName->setText("");
        ui->comboBox_orgType->setCurrentText("Укажите тип");
        ui->lineEdit_orgINN->setText("");
        ui->lineEdit_orgORGN->setText("");
        ui->lineEdit_orgYAdr->setText("");
        ui->lineEdit_orgFAdr->setText("");

        ui->listWidget_org->removeItemWidget(ui->listWidget_org->takeItem(ui->listWidget_org->currentRow()));
        ui->toolButton_add_org->setEnabled(true);
        ui->toolButton_del_org->setEnabled(false);
        ui->toolButton_change_org->setEnabled(false);
        ui->toolButton_accept_org->setEnabled(false);
        ui->toolButton_cancel_org->setEnabled(false);
        ui->label_statusAction_org->setText("");
        ui->scrollAreaWidgetContents_org->setEnabled(false);
        ui->listWidget_org->setEnabled(true);

    }
    if (ui->label_statusAction_org->text() == "Изменение данных об организации"){

        ui->label_statusAction_org->setText("");
        ui->lineEdit_orgName->setText(listOrg[ui->listWidget_org->currentRow()].Name.text());
        ui->listWidget_org->currentItem()->setText(listOrg[ui->listWidget_org->currentRow()].Name.text());
        ui->comboBox_orgType->setCurrentText(listOrg[ui->listWidget_org->currentRow()].Type);
        ui->lineEdit_orgINN->setText(listOrg[ui->listWidget_org->currentRow()].INN);
        ui->lineEdit_orgORGN->setText(listOrg[ui->listWidget_org->currentRow()].ORGN);
        ui->lineEdit_orgYAdr->setText(listOrg[ui->listWidget_org->currentRow()].YAdr);
        ui->lineEdit_orgFAdr->setText(listOrg[ui->listWidget_org->currentRow()].FAdr);

        ui->toolButton_add_org->setEnabled(true);
        ui->toolButton_del_org->setEnabled(true);
        ui->toolButton_change_org->setEnabled(true);
        ui->toolButton_accept_org->setEnabled(false);
        ui->toolButton_cancel_org->setEnabled(false);
        ui->scrollAreaWidgetContents_org->setEnabled(false);
        ui->listWidget_org->setEnabled(true);


    }
}

void MainWindow::on_lineEdit_orgName_textChanged(const QString &arg1)
{
    if (ui->label_statusAction_org->text() == "Добавление новой организации" || ui->label_statusAction_org->text() == "Изменение данных об организации"){
        ui->listWidget_org->currentItem()->setText(arg1);
    }
}

void MainWindow::on_toolButton_accept_org_clicked()
{
    if (ui->label_statusAction_org->text() == "Добавление новой организации"){

        ui->label_statusAction_org->setText("");

        Organization *org = new Organization();

        org->Name = *ui->listWidget_org->currentItem();
        ui->lineEdit_orgName->setText("");

        org->Type = ui->comboBox_orgType->currentText();
        ui->comboBox_orgType->setCurrentText("Укажите тип");

        org->INN = ui->lineEdit_orgINN->text();
        ui->lineEdit_orgINN->setText("");

        org->ORGN = ui->lineEdit_orgORGN->text();
        ui->lineEdit_orgORGN->setText("");

        org->YAdr = ui->lineEdit_orgYAdr->text();
        ui->lineEdit_orgYAdr->setText("");

        org->FAdr = ui->lineEdit_orgFAdr->text();
        ui->lineEdit_orgFAdr->setText("");

        listOrg.append(*org);

        ui->toolButton_add_org->setEnabled(true);
        ui->toolButton_del_org->setEnabled(false);
        ui->toolButton_change_org->setEnabled(false);
        ui->toolButton_accept_org->setEnabled(false);
        ui->toolButton_cancel_org->setEnabled(false);
        ui->scrollAreaWidgetContents_org->setEnabled(false);
        ui->listWidget_org->setEnabled(true);

        ui->listWidget_org->currentItem()->setSelected(false);
    }
    if (ui->label_statusAction_org->text() == "Изменение данных об организации"){

        ui->label_statusAction_org->setText("");

        listOrg[ui->listWidget_org->currentRow()].Name.setText(ui->lineEdit_orgName->text());
        listOrg[ui->listWidget_org->currentRow()].Type = ui->comboBox_orgType->currentText();
        listOrg[ui->listWidget_org->currentRow()].INN = ui->lineEdit_orgINN->text();
        listOrg[ui->listWidget_org->currentRow()].ORGN = ui->lineEdit_orgORGN->text();
        listOrg[ui->listWidget_org->currentRow()].YAdr = ui->lineEdit_orgYAdr->text();
        listOrg[ui->listWidget_org->currentRow()].FAdr = ui->lineEdit_orgFAdr->text();

        for (int i = 0 ; i < listOb.count() ; i++) {
            if (ui->listWidget_org->currentItem() == listOb[i].nameOrg){
                for (int j = 0 ; j < ui->tableWidget_ob->rowCount() ; j++) {
                    if (listOb[i].objectTableRow == ui->tableWidget_ob->item(j,0)){
                        QTableWidgetItem* item = ui->tableWidget_ob->item(j,0);
                        item->setIcon(ui->comboBox_orgType->itemIcon(ui->comboBox_orgType->currentIndex()));
                        item->setText(ui->lineEdit_orgName->text());
                    }
                }
            }
        }

        ui->toolButton_add_org->setEnabled(true);
        ui->toolButton_del_org->setEnabled(true);
        ui->toolButton_change_org->setEnabled(true);
        ui->toolButton_accept_org->setEnabled(false);
        ui->toolButton_cancel_org->setEnabled(false);
        ui->scrollAreaWidgetContents_org->setEnabled(false);
        ui->listWidget_org->setEnabled(true);

    }
    if (ui->label_statusAction_ob->text() == "Добавление нового объекта"){
        ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,0);
        ui->tableWidget_ob->setCellWidget(ui->tableWidget_ob->rowCount()-1,0, new QComboBox);
        QComboBox* nameOrg = (QComboBox*)(ui->tableWidget_ob->cellWidget(ui->tableWidget_ob->rowCount()-1,0));
        nameOrg->setMinimumWidth(49);
        nameOrg->resize(125, nameOrg->height());
        nameOrg->addItem("Укажите организацию");
        nameOrg->setItemIcon(0, ui->comboBox_orgType->itemIcon(0));
        for (int i = 0 ; i < listOrg.count() ; i++) {
            nameOrg->addItem(listOrg[i].Name.text());
            nameOrg->setItemIcon(i+1, ui->comboBox_orgType->itemIcon(ui->comboBox_orgType->findText(listOrg[i].Type))); // i+1 потому что 0-я строка "Укажите тип"
        }
    }
    if (ui->label_statusAction_ob->text() == "Изменение данных об объекте"){
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,0);
        QComboBox* nameOrg = new QComboBox;
        ui->tableWidget_ob->setCellWidget(chengedObjetRow,0, nameOrg);
        nameOrg->setMinimumWidth(49);
        nameOrg->resize(125, nameOrg->height());
        nameOrg->addItem("Укажите организацию");
        nameOrg->setItemIcon(0, ui->comboBox_orgType->itemIcon(0));
        for (int i = 0 ; i < listOrg.count() ; i++) {
            nameOrg->addItem(listOrg[i].Name.text());
            nameOrg->setItemIcon(i+1, ui->comboBox_orgType->itemIcon(ui->comboBox_orgType->findText(listOrg[i].Type))); // i+1 потому что 0-я строка "Укажите тип"
        }

        ui->tableWidget_ob->item(chengedObjetRow,0)->setIcon(QIcon());

    }
}

void MainWindow::on_listWidget_org_itemClicked(QListWidgetItem *item)
{


    ui->lineEdit_orgName->setText(listOrg[ui->listWidget_org->row(item)].Name.text());
    ui->comboBox_orgType->setCurrentText(listOrg[ui->listWidget_org->row(item)].Type);
    ui->lineEdit_orgINN->setText(listOrg[ui->listWidget_org->row(item)].INN);
    ui->lineEdit_orgORGN->setText(listOrg[ui->listWidget_org->row(item)].ORGN);
    ui->lineEdit_orgYAdr->setText(listOrg[ui->listWidget_org->row(item)].YAdr);
    ui->lineEdit_orgFAdr->setText(listOrg[ui->listWidget_org->row(item)].FAdr);

}

void MainWindow::on_toolButton_del_org_clicked()
{
    QString headerMes;
    QString mes;
    if(ui->listWidget_org->selectedItems().count()==1){
        headerMes="Удаление элемента";
        mes="Вы действительно хотите удалить выбранный элемент?";
    }else {
        headerMes="Удаление элементов";
        mes="Вы действительно хотите удалить "+ QString::number(ui->listWidget_org->selectedItems().count())+" выбранных(-е) элемента(-ы)?";
    }
    if (!(QMessageBox::warning(0,
                               headerMes,
                               mes,
                               "Да",
                               "Нет",
                               QString(),
                               0,
                               1
                               ))){
        foreach(QListWidgetItem * item, ui->listWidget_org->selectedItems())
        {

            int n = ui->listWidget_org->row(item);
            for (int i = 0 ; i < listOb.count(); i++) {
                if (listOb[i].nameOrg == item){//qobject_cast(listOb[i].nameOrg)){
                    for (int j = 0 ; j < ui->tableWidget_ob->rowCount() ; j++) {
                        if (listOb[i].objectTableRow == ui->tableWidget_ob->item(j,0)){
                            ui->tableWidget_ob->item(j,0)->setText("Укажите организацию");
                            ui->tableWidget_ob->item(j,0)->setIcon(QIcon(":icon/img/noneType.png"));
                        }
                    }
                    listOb[i].nameOrg = NULL;
                }
            }
            delete ui->listWidget_org->takeItem(n);
            listOrg.removeAt(n);

        }
        if(ui->label_statusAction_ob->text() == "Изменение данных об объекте"){
            ui->tableWidget_ob->item(chengedObjetRow,0)->setIcon(QIcon());
        }

        ui->lineEdit_orgName->setText("");
        ui->comboBox_orgType->setCurrentText("Укажите тип");
        ui->lineEdit_orgINN->setText("");
        ui->lineEdit_orgORGN->setText("");
        ui->lineEdit_orgYAdr->setText("");
        ui->lineEdit_orgFAdr->setText("");

        if (ui->label_statusAction_ob->text() == "Добавление нового объекта"){
            ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,0);
            ui->tableWidget_ob->setCellWidget(ui->tableWidget_ob->rowCount()-1,0, new QComboBox);
            QComboBox* nameOrg = (QComboBox*)(ui->tableWidget_ob->cellWidget(ui->tableWidget_ob->rowCount()-1,0));
            nameOrg->setMinimumWidth(49);
            nameOrg->resize(125, nameOrg->height());
            nameOrg->addItem("Укажите организацию");
            nameOrg->setItemIcon(0, ui->comboBox_orgType->itemIcon(0));
            for (int i = 0 ; i < listOrg.count() ; i++) {
                nameOrg->addItem(listOrg[i].Name.text());
                nameOrg->setItemIcon(i+1, ui->comboBox_orgType->itemIcon(ui->comboBox_orgType->findText(listOrg[i].Type))); // i+1 потому что 0-я строка "Укажите тип"
            }
        }
        if (ui->label_statusAction_ob->text() == "Изменение данных об объекте"){
            ui->tableWidget_ob->removeCellWidget(chengedObjetRow,0);
            QComboBox* nameOrg = new QComboBox;
            ui->tableWidget_ob->setCellWidget(chengedObjetRow,0, nameOrg);
            nameOrg->setMinimumWidth(49);
            nameOrg->resize(125, nameOrg->height());
            nameOrg->addItem("Укажите организацию");
            nameOrg->setItemIcon(0, ui->comboBox_orgType->itemIcon(0));
            for (int i = 0 ; i < listOrg.count() ; i++) {
                nameOrg->addItem(listOrg[i].Name.text());
                nameOrg->setItemIcon(i+1, ui->comboBox_orgType->itemIcon(ui->comboBox_orgType->findText(listOrg[i].Type))); // i+1 потому что 0-я строка "Укажите тип"
            }
        }
    }
}

void MainWindow::on_listWidget_org_itemSelectionChanged()
{
    if (ui->label_statusAction_org->text()==""){
        if(ui->listWidget_org->selectedItems().count()>0){
            ui->toolButton_del_org->setEnabled(true);
        }else {
            ui->toolButton_del_org->setEnabled(false);
        }
        if(ui->listWidget_org->selectedItems().count()==1){
            ui->toolButton_change_org->setEnabled(true);
        }else {
            ui->toolButton_change_org->setEnabled(false);
        }
    }
}

void MainWindow::on_toolButton_change_org_clicked()
{

    ui->toolButton_add_org->setEnabled(false);
    ui->toolButton_del_org->setEnabled(false);
    ui->toolButton_change_org->setEnabled(false);
    ui->toolButton_accept_org->setEnabled(true);
    ui->toolButton_cancel_org->setEnabled(true);
    ui->label_statusAction_org->setText("Изменение данных об организации");
    ui->scrollAreaWidgetContents_org->setEnabled(true);
    ui->listWidget_org->setEnabled(false);
}

//-----------------------------     окно объектов
//-----------------------------

void MainWindow::on_toolButton_add_ob_clicked()
{

    ui->widget_arr->setEnabled(false);
    ui->tableWidget_ob->setSortingEnabled(false);
    ui->toolButton_add_ob->setEnabled(false);
    ui->toolButton_del_ob->setEnabled(false);
    ui->toolButton_change_ob->setEnabled(false);
    ui->toolButton_accept_ob->setEnabled(true);
    ui->toolButton_cancel_ob->setEnabled(true);
    ui->label_statusAction_ob->setText("Добавление нового объекта");


    ui->tableWidget_ob->insertRow(ui->tableWidget_ob->rowCount());

    ui->tableWidget_ob->setCellWidget(ui->tableWidget_ob->rowCount()-1,0, new QComboBox);
    QComboBox* nameOrg = (QComboBox*)(ui->tableWidget_ob->cellWidget(ui->tableWidget_ob->rowCount()-1,0));
    nameOrg->setMinimumWidth(49);
    nameOrg->resize(125, nameOrg->height());
    nameOrg->addItem("Укажите организацию");
    nameOrg->setItemIcon(0, ui->comboBox_orgType->itemIcon(0));
    for (int i = 0 ; i < listOrg.count() ; i++) {
        nameOrg->addItem(listOrg[i].Name.text());
        nameOrg->setItemIcon(i+1, ui->comboBox_orgType->itemIcon(ui->comboBox_orgType->findText(listOrg[i].Type)));// i+1 потому что 0-я строка "Укажите организацию"
    }

    ui->tableWidget_ob->setCellWidget(ui->tableWidget_ob->rowCount()-1,1, new QLineEdit);

    ui->tableWidget_ob->setCellWidget(ui->tableWidget_ob->rowCount()-1,2, new QComboBox);
    QComboBox* typeOb = (QComboBox*)(ui->tableWidget_ob->cellWidget(ui->tableWidget_ob->rowCount()-1,2));
    typeOb->setMinimumWidth(49);
    typeOb->resize(125, typeOb->height());
    typeOb->addItem("ИС");
    typeOb->addItem("ИТКС");
    typeOb->addItem("АСУ");

    ui->tableWidget_ob->setCellWidget(ui->tableWidget_ob->rowCount()-1,3, new QComboBox);
    QComboBox* activityOrg = (QComboBox*)(ui->tableWidget_ob->cellWidget(ui->tableWidget_ob->rowCount()-1,3));
    activityOrg->setMinimumWidth(49);
    activityOrg->resize(125, activityOrg->height());
    activityOrg->addItem("Здравоохранение");
    activityOrg->addItem("Наука");
    activityOrg->addItem("Транспорт");
    activityOrg->addItem("Связь");
    activityOrg->addItem("Энергетика");
    activityOrg->addItem("Банк./Фин. рынок");
    activityOrg->addItem("Атомная энергетика");
    activityOrg->addItem("Оборонная пром.");
    activityOrg->addItem("Ракетно-космическая пром.");
    activityOrg->addItem("Горнодобывающая пром.");
    activityOrg->addItem("Металлургическая пром.");
    activityOrg->addItem("Химическая пром.");

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setText("Новый");
    ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,4,item);



    widgetForCategoryZOKII = new WidgetForCategory;//---------- Виджет ячейки для категорий ЗОКИИ

    connect(widgetForCategoryZOKII->catButton, SIGNAL(clicked()), this, SLOT(test_ZOKII()));

    ui->tableWidget_ob->setCellWidget(ui->tableWidget_ob->rowCount()-1,5, widgetForCategoryZOKII);

    //----------
    widgetForCategoryISPDn = new WidgetForCategory;//---------- Виджет ячейки для категорий ИСПДн

    widgetForCategoryISPDn->catBox->addItem("4");

    connect(widgetForCategoryISPDn->catButton, SIGNAL(clicked()), this, SLOT(test_ISPDn()));

    ui->tableWidget_ob->setCellWidget(ui->tableWidget_ob->rowCount()-1,6, widgetForCategoryISPDn);


    //----------
    widgetForCategoryGIS = new WidgetForCategory;//---------- Виджет ячейки для категорий ГИС

    connect(widgetForCategoryGIS->catButton, SIGNAL(clicked()), this, SLOT(test_GIS()));

    ui->tableWidget_ob->setCellWidget(ui->tableWidget_ob->rowCount()-1,7, widgetForCategoryGIS);

    //-------

}

void MainWindow::test_ZOKII(){
    ui->statusBar->showMessage("Определение категории объекта как ЗОКИИ");
    testZOKIIWindow = new TestZOKIIWindow;
    testZOKIIWindow->setAttribute(Qt::WA_DeleteOnClose);

    connect(testZOKIIWindow, &TestZOKIIWindow::accpet_result, this, &MainWindow::close_test_ZOKII);

    testZOKIIWindow->setWindowModality(Qt::ApplicationModal);
    testZOKIIWindow->show();

}

void MainWindow::close_test_ZOKII(int category){
    disconnect(testZOKIIWindow, &TestZOKIIWindow::accpet_result, this, &MainWindow::close_test_ZOKII);
    testZOKIIWindow->close();
    widgetForCategoryZOKII->catBox->setCurrentIndex(category+1);
    if (category == 0){ui->statusBar->showMessage("Объект не нуждается в присвоении  категории ЗОКИИ");}else{
        ui->statusBar->showMessage("Объекту присвоена " + QString::number(category) + " категория ЗОКИИ");
    }
}

void MainWindow::test_ISPDn(){
    ui->statusBar->showMessage("Определение категории объекта как ИСПДн");
    testISPDnWindow = new TestISPDnWindow;
    testISPDnWindow->setAttribute(Qt::WA_DeleteOnClose);

    connect(testISPDnWindow, &TestISPDnWindow::accpet_result, this, &MainWindow::close_test_ISPDn);

    testISPDnWindow->setWindowModality(Qt::ApplicationModal);
    testISPDnWindow->show();
}

void MainWindow::close_test_ISPDn(int category){
    disconnect(testISPDnWindow, &TestISPDnWindow::accpet_result, this, &MainWindow::close_test_ISPDn);
    testISPDnWindow->close();
    widgetForCategoryISPDn->catBox->setCurrentIndex(category+1);
    if (category == 0){ui->statusBar->showMessage("Объект не нуждается в присвоении  категории ИСПДн");}else{
        ui->statusBar->showMessage("Объекту присвоена " + QString::number(category) + " категория ИСПДн");
    }
}

void MainWindow::test_GIS(){
    ui->statusBar->showMessage("Определение категории объекта как ГИС");
    testGISWindow = new TestGISWindow;
    testGISWindow->setAttribute(Qt::WA_DeleteOnClose);

    connect(testGISWindow, &TestGISWindow::accpet_result, this, &MainWindow::close_test_GIS);

    testGISWindow->setWindowModality(Qt::ApplicationModal);
    testGISWindow->show();
}

void MainWindow::close_test_GIS(int category){
    disconnect(testGISWindow, &TestGISWindow::accpet_result, this, &MainWindow::close_test_GIS);
    testGISWindow->close();
    widgetForCategoryGIS->catBox->setCurrentIndex(category+1);
    if (category == 0){ui->statusBar->showMessage("Объект не нуждается в присвоении  категории ГИС");}else{
        ui->statusBar->showMessage("Объекту присвоена " + QString::number(category) + " категория ГИС");
    }
}

void MainWindow::on_toolButton_cancel_ob_clicked()
{

    if (ui->label_statusAction_ob->text() == "Добавление нового объекта"){

        ui->tableWidget_ob->removeRow(ui->tableWidget_ob->rowCount()-1);
        ui->toolButton_add_ob->setEnabled(true);
        ui->toolButton_del_ob->setEnabled(false);
        ui->toolButton_change_ob->setEnabled(false);
        ui->toolButton_accept_ob->setEnabled(false);
        ui->toolButton_cancel_ob->setEnabled(false);
        ui->label_statusAction_ob->setText("");
        //ui->widget_arr->setEnabled(true);
    }
    if (ui->label_statusAction_ob->text() == "Изменение данных об объекте"){

        QComboBox* nameOrg = (QComboBox*)(ui->tableWidget_ob->cellWidget(chengedObjetRow,0));
        ui->tableWidget_ob->item(chengedObjetRow,0)->setIcon(nameOrg->itemIcon(nameOrg->findText(ui->tableWidget_ob->item(chengedObjetRow,0)->text())));
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,0);
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,1);
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,2);
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,3);
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,5);
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,6);
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,7);

        ui->toolButton_add_ob->setEnabled(true);
        ui->toolButton_del_ob->setEnabled(false);
        ui->toolButton_change_ob->setEnabled(false);
        ui->toolButton_accept_ob->setEnabled(false);
        ui->toolButton_cancel_ob->setEnabled(false);
        ui->label_statusAction_ob->setText("");
    }
    ui->tableWidget_ob->setSortingEnabled(true);
}

void MainWindow::on_toolButton_accept_ob_clicked()
{

    if (ui->label_statusAction_ob->text() == "Добавление нового объекта"){

        ui->label_statusAction_ob->setText("");

        Object *ob = new Object();

        QComboBox* nameOrg = (QComboBox*)(ui->tableWidget_ob->cellWidget(ui->tableWidget_ob->rowCount()-1,0));



        if (nameOrg->currentIndex()>0){
            ob->nameOrg = ui->listWidget_org->item(nameOrg->currentIndex()-1);
            ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,0);
            ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,0, new QTableWidgetItem(ob->nameOrg->text()));
        } else {
            ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,0);
            ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,0, new QTableWidgetItem("Укажите организацию"));
        }
        ob->objectTableRow = ui->tableWidget_ob->item(ui->tableWidget_ob->rowCount()-1,0);
        ui->tableWidget_ob->item(ui->tableWidget_ob->rowCount()-1,0)->setIcon(nameOrg->itemIcon(nameOrg->currentIndex()));

        QLineEdit* name = (QLineEdit*)(ui->tableWidget_ob->cellWidget(ui->tableWidget_ob->rowCount()-1,1));
        ob->name = name->text();

        QComboBox* typeOb = (QComboBox*)(ui->tableWidget_ob->cellWidget(ui->tableWidget_ob->rowCount()-1,2));
        ob->type = typeOb->currentText();

        QComboBox* activityOrg = (QComboBox*)(ui->tableWidget_ob->cellWidget(ui->tableWidget_ob->rowCount()-1,3));
        ob->activityOrg = activityOrg->currentText();

        ob->status = "Новый";

        ob->catZOKII = widgetForCategoryZOKII->catBox->currentText();

        ob->catISPDn = widgetForCategoryISPDn->catBox->currentText();

        ob->catGIS = widgetForCategoryGIS->catBox->currentText();

        ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,1);
        ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,1, new QTableWidgetItem(ob->name));

        ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,2);
        ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,2, new QTableWidgetItem(ob->type));

        ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,3);
        ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,3, new QTableWidgetItem(ob->activityOrg));

        ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,5);
        ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,5, new QTableWidgetItem(ob->catZOKII));

        ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,6);
        ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,6, new QTableWidgetItem(ob->catISPDn));

        ui->tableWidget_ob->removeCellWidget(ui->tableWidget_ob->rowCount()-1,7);
        ui->tableWidget_ob->setItem(ui->tableWidget_ob->rowCount()-1,7, new QTableWidgetItem(ob->catGIS));

        listOb.append(*ob);

        ui->toolButton_add_ob->setEnabled(true);
        ui->toolButton_del_ob->setEnabled(false);
        ui->toolButton_change_ob->setEnabled(false);
        ui->toolButton_accept_ob->setEnabled(false);
        ui->toolButton_cancel_ob->setEnabled(false);
        //ui->widget_arr->setEnabled(true);
    }

    if (ui->label_statusAction_ob->text() == "Изменение данных об объекте"){

        QComboBox* nameOrg = (QComboBox*)(ui->tableWidget_ob->cellWidget(chengedObjetRow,0));
        ui->tableWidget_ob->item(chengedObjetRow,0)->setText(nameOrg->currentText());
        ui->tableWidget_ob->item(chengedObjetRow,0)->setIcon(nameOrg->itemIcon(nameOrg->currentIndex()));
        if (nameOrg->currentIndex()>0){
            ui->listWidget_org->item(nameOrg->currentIndex()-1);
            ui->tableWidget_ob->removeCellWidget(chengedObjetRow,0);
        } else {
            listOb[chengedObjet].nameOrg = NULL;
            ui->tableWidget_ob->removeCellWidget(chengedObjetRow,0);
        }
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,0);


        QLineEdit* name = (QLineEdit*)(ui->tableWidget_ob->cellWidget(chengedObjetRow,1));
        ui->tableWidget_ob->item(chengedObjetRow,1)->setText(name->text());
        listOb[chengedObjet].name = name->text();
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,1);

        QComboBox* typeOb = (QComboBox*)(ui->tableWidget_ob->cellWidget(chengedObjetRow,2));
        ui->tableWidget_ob->item(chengedObjetRow,2)->setText(typeOb->currentText());
        listOb[chengedObjet].type = typeOb->currentText();
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,2);

        QComboBox* activityOrg = (QComboBox*)(ui->tableWidget_ob->cellWidget(chengedObjetRow,3));
        ui->tableWidget_ob->item(chengedObjetRow,3)->setText(activityOrg->currentText());
        listOb[chengedObjet].activityOrg = activityOrg->currentText();
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,3);

        listOb[chengedObjet].catZOKII = widgetForCategoryZOKII->catBox->currentText();
        ui->tableWidget_ob->item(chengedObjetRow,5)->setText(widgetForCategoryZOKII->catBox->currentText());
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,5);

        listOb[chengedObjet].catISPDn = widgetForCategoryISPDn->catBox->currentText();
        ui->tableWidget_ob->item(chengedObjetRow,6)->setText(widgetForCategoryISPDn->catBox->currentText());
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,6);

        listOb[chengedObjet].catGIS = widgetForCategoryGIS->catBox->currentText();
        ui->tableWidget_ob->item(chengedObjetRow,7)->setText(listOb[chengedObjet].catGIS);
        ui->tableWidget_ob->removeCellWidget(chengedObjetRow,7);

        checkStatus();

        ui->toolButton_add_ob->setEnabled(true);
        ui->toolButton_del_ob->setEnabled(false);
        ui->toolButton_change_ob->setEnabled(false);
        ui->toolButton_accept_ob->setEnabled(false);
        ui->toolButton_cancel_ob->setEnabled(false);
        ui->label_statusAction_ob->setText("");
        //ui->widget_arr->setEnabled(true);
    }


    ui->tableWidget_ob->setSortingEnabled(true);
}

void MainWindow::on_tableWidget_ob_itemSelectionChanged()
{
    if (ui->label_statusAction_ob->text() == ""){
        QList<int> numberSelectedRow;
        for (int i = 0 ; i < ui->tableWidget_ob->selectedItems().count() ; i++){
            if(!numberSelectedRow.contains(ui->tableWidget_ob->selectedItems()[i]->row()))
            {
                numberSelectedRow.append(ui->tableWidget_ob->selectedItems()[i]->row());
            }
        }
        for ( int i = ui->tableWidget_arr->rowCount()-1 ; i > -1 ; i--) {
            ui->tableWidget_arr->removeRow(i);
        }
        if(numberSelectedRow.count()>0){
            ui->toolButton_del_ob->setEnabled(true);
        } else {
            ui->toolButton_del_ob->setEnabled(false);
        }
        if(numberSelectedRow.count()==1){
            ui->widget_arr->setEnabled(true);
            for (int i = 0 ; i < listOb.count() ; i++) {
                if (ui->tableWidget_ob->item(numberSelectedRow[0],0) == listOb[i].objectTableRow){
                    for (int j = 0 ; j < listOb[i].profile[0]->count() ; j++) {

                        ui->tableWidget_arr->insertRow(ui->tableWidget_arr->rowCount());

                        QTableWidgetItem* name = new QTableWidgetItem;
                        name->setText(listOb[i].profile[0]->at(j)->text());
                        ui->tableWidget_arr->setItem(j,0, name);
                        QTableWidgetItem* form = new QTableWidgetItem;
                        form->setText(listOb[i].profile[1]->at(j)->text());
                        ui->tableWidget_arr->setItem(j,1, form);
                        QTableWidgetItem* classSZI = new QTableWidgetItem;
                        classSZI->setText(listOb[i].profile[2]->at(j)->text());
                        ui->tableWidget_arr->setItem(j,2, classSZI);
                        QTableWidgetItem* lvlSZI = new QTableWidgetItem;
                        lvlSZI->setText(listOb[i].profile[3]->at(j)->text());
                        ui->tableWidget_arr->setItem(j,3, lvlSZI);
                        QTableWidgetItem* classSVT = new QTableWidgetItem;
                        classSVT->setText(listOb[i].profile[4]->at(j)->text());
                        ui->tableWidget_arr->setItem(j,4, classSVT);
                        QTableWidgetItem* doc = new QTableWidgetItem;
                        doc->setText(listOb[i].profile[5]->at(j)->text());
                        ui->tableWidget_arr->setItem(j,5, doc);
                        QTableWidgetItem* comment = new QTableWidgetItem;
                        comment->setText(listOb[i].profile[6]->at(j)->text());
                        ui->tableWidget_arr->setItem(j,6, comment);
                    }

                    checkStatus();
                }
            }
            ui->toolButton_change_ob->setEnabled(true);
        }else {
            ui->widget_arr->setEnabled(false);

            ui->toolButton_change_ob->setEnabled(false);
        }
    }
}

void MainWindow::on_toolButton_del_ob_clicked()
{
    ui->tableWidget_ob->setSortingEnabled(false);
    QString headerMes;
    QString mes;
    QList<int> numberSelectedRow;
    for (int i = 0 ; i < ui->tableWidget_ob->selectedItems().count() ; i++){
        if(!numberSelectedRow.contains(ui->tableWidget_ob->selectedItems()[i]->row()))
        {
            numberSelectedRow.append(ui->tableWidget_ob->selectedItems()[i]->row());
        }
    }
    if(numberSelectedRow.count()==1){
        headerMes="Удаление элемента";
        mes="Вы действительно хотите удалить выбранный элемент?";
    }else {
        headerMes="Удаление элементов";
        mes="Вы действительно хотите удалить "+ QString::number(numberSelectedRow.count())+" выбранных(-е) элемента(-ы)?";
    }
    if (!(QMessageBox::warning(0,
                               headerMes,
                               mes,
                               "Да",
                               "Нет",
                               QString(),
                               0,
                               1
                               ))){
        QList<int> numberSelectedRowReversed;
        numberSelectedRowReversed.reserve( numberSelectedRow.size() ); // reserve is new in Qt 4.7
        std::reverse_copy( numberSelectedRow.begin(), numberSelectedRow.end(), std::back_inserter( numberSelectedRowReversed ) );
        foreach(int n, numberSelectedRowReversed)
        {
            for (int i = 0 ; i < listOb.count(); i++) {
                if (ui->tableWidget_ob->item(n,0) == listOb[i].objectTableRow){//qobject_cast(listOb[i].nameOrg)){
                    listOb.removeAt(i);
                }
            }
            ui->tableWidget_ob->removeRow(n);
        }
    }
    ui->tableWidget_ob->setSortingEnabled(true);
}

void MainWindow::on_toolButton_change_ob_clicked()
{
    ui->tableWidget_ob->setSortingEnabled(false);
    ui->widget_arr->setEnabled(false);
    ui->toolButton_add_ob->setEnabled(false);
    ui->toolButton_del_ob->setEnabled(false);
    ui->toolButton_change_ob->setEnabled(false);
    ui->toolButton_accept_ob->setEnabled(true);
    ui->toolButton_cancel_ob->setEnabled(true);
    ui->label_statusAction_ob->setText("Изменение данных об объекте");

    int numberSelectedRow = ui->tableWidget_ob->selectedItems()[0]->row();
    int numberSelectedObject = -1;
    for(int i = 0 ; i < listOb.count() ; i++){
        if (listOb[i].objectTableRow == ui->tableWidget_ob->item(numberSelectedRow,0)){
            numberSelectedObject = i;
        }
    }

    chengedObjetRow = numberSelectedRow;
    chengedObjet = numberSelectedObject;

    QComboBox* nameOrg = new QComboBox;
    ui->tableWidget_ob->setCellWidget(numberSelectedRow,0, nameOrg);
    nameOrg->setMinimumWidth(49);
    nameOrg->resize(125, nameOrg->height());
    nameOrg->addItem("Укажите организацию");
    nameOrg->setItemIcon(0, ui->comboBox_orgType->itemIcon(0));
    for (int i = 0 ; i < listOrg.count() ; i++) {
        nameOrg->addItem(listOrg[i].Name.text());
        nameOrg->setItemIcon(i+1, ui->comboBox_orgType->itemIcon(ui->comboBox_orgType->findText(listOrg[i].Type)));// i+1 потому что 0-я строка "Укажите тип"
    }
    nameOrg->setCurrentText(ui->tableWidget_ob->item(numberSelectedRow,0)->text());
    ui->tableWidget_ob->item(numberSelectedRow,0)->setIcon(QIcon());

    ui->tableWidget_ob->setCellWidget(numberSelectedRow,1, new QLineEdit(ui->tableWidget_ob->item(numberSelectedRow,1)->text()));

    QComboBox* typeOb = new QComboBox;
    ui->tableWidget_ob->setCellWidget(numberSelectedRow,2, typeOb);
    typeOb->setMinimumWidth(49);
    typeOb->resize(125, typeOb->height());
    typeOb->addItem("ИС");
    typeOb->addItem("ИТКС");
    typeOb->addItem("АСУ");
    typeOb->setCurrentText(ui->tableWidget_ob->item(numberSelectedRow,2)->text());

    QComboBox* activityOrg = new QComboBox;
    ui->tableWidget_ob->setCellWidget(numberSelectedRow,3, activityOrg);
    activityOrg->setMinimumWidth(49);
    activityOrg->resize(125, activityOrg->height());
    activityOrg->addItem("Здравоохранение");
    activityOrg->addItem("Наука");
    activityOrg->addItem("Транспорт");
    activityOrg->addItem("Связь");
    activityOrg->addItem("Энергетика");
    activityOrg->addItem("Банк./Фин. рынок");
    activityOrg->addItem("Атомная энергетика");
    activityOrg->addItem("Оборонная пром.");
    activityOrg->addItem("Ракетно-космическая пром.");
    activityOrg->addItem("Горнодобывающая пром.");
    activityOrg->addItem("Металлургическая пром.");
    activityOrg->addItem("Химическая пром.");
    activityOrg->setCurrentText(ui->tableWidget_ob->item(numberSelectedRow,3)->text());


    widgetForCategoryZOKII = new WidgetForCategory;//---------- Виджет ячейки для категорий ЗОКИИ

    connect(widgetForCategoryZOKII->catButton, SIGNAL(clicked()), this, SLOT(test_ZOKII()));

    ui->tableWidget_ob->setCellWidget(numberSelectedRow,5, widgetForCategoryZOKII);

    widgetForCategoryZOKII->catBox->setCurrentText(ui->tableWidget_ob->item(numberSelectedRow,5)->text());

    //----------
    widgetForCategoryISPDn = new WidgetForCategory;//---------- Виджет ячейки для категорий ИСПДн

    widgetForCategoryISPDn->catBox->addItem("4");

    connect(widgetForCategoryISPDn->catButton, SIGNAL(clicked()), this, SLOT(test_ISPDn()));

    ui->tableWidget_ob->setCellWidget(numberSelectedRow,6, widgetForCategoryISPDn);

    widgetForCategoryISPDn->catBox->setCurrentText(ui->tableWidget_ob->item(numberSelectedRow,6)->text());

    //----------
    widgetForCategoryGIS = new WidgetForCategory;//---------- Виджет ячейки для категорий ГИС

    connect(widgetForCategoryGIS->catButton, SIGNAL(clicked()), this, SLOT(test_GIS()));

    ui->tableWidget_ob->setCellWidget(numberSelectedRow,7, widgetForCategoryGIS);

    widgetForCategoryGIS->catBox->setCurrentText(ui->tableWidget_ob->item(numberSelectedRow,7)->text());

    //-------

}



void MainWindow::on_toolButton_add_arr_clicked()
{
    ui->widget_ob->setEnabled(false);
    ui->toolButton_add_arr->setEnabled(false);
    ui->toolButton_del_arr->setEnabled(false);
    ui->toolButton_change_arr->setEnabled(false);
    ui->toolButton_accept_arr->setEnabled(true);
    ui->toolButton_cancel_arr->setEnabled(true);
    ui->toolButton_ask_arr->setEnabled(false);
    ui->label_statusAction_arr->setText("Добавление новой меры");

    ui->tableWidget_arr->insertRow(ui->tableWidget_arr->rowCount());

    ui->tableWidget_arr->setCellWidget(ui->tableWidget_arr->rowCount()-1,0, new QLineEdit);

    ui->tableWidget_arr->setCellWidget(ui->tableWidget_arr->rowCount()-1,1, new QTextEdit);

    ui->tableWidget_arr->setCellWidget(ui->tableWidget_arr->rowCount()-1,2, new QComboBox);
    QComboBox* classSZI = (QComboBox*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,2));
    classSZI->addItem("Без СЗИ");
    classSZI->addItem("1");
    classSZI->addItem("2");
    classSZI->addItem("3");
    classSZI->addItem("4");
    classSZI->addItem("5");
    classSZI->addItem("6");

    ui->tableWidget_arr->setCellWidget(ui->tableWidget_arr->rowCount()-1,3, new QComboBox);
    QComboBox* lvlSZI = (QComboBox*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,3));
    lvlSZI->addItem("Без СЗИ");
    lvlSZI->addItem("1");
    lvlSZI->addItem("2");
    lvlSZI->addItem("3");
    lvlSZI->addItem("4");
    lvlSZI->addItem("5");
    lvlSZI->addItem("6");

    ui->tableWidget_arr->setCellWidget(ui->tableWidget_arr->rowCount()-1,4, new QComboBox);
    QComboBox* classSVT = (QComboBox*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,4));
    classSVT->addItem("Без СВТ");
    classSVT->addItem("1");
    classSVT->addItem("2");
    classSVT->addItem("3");
    classSVT->addItem("4");
    classSVT->addItem("5");
    classSVT->addItem("6");
    classSVT->addItem("7");

    ui->tableWidget_arr->setCellWidget(ui->tableWidget_arr->rowCount()-1,5, new QTextEdit);

    ui->tableWidget_arr->setCellWidget(ui->tableWidget_arr->rowCount()-1,6, new QTextEdit);
}

void MainWindow::on_toolButton_cancel_arr_clicked()
{
    if (ui->label_statusAction_arr->text() == "Добавление новой меры"){

        ui->tableWidget_arr->removeRow(ui->tableWidget_arr->rowCount()-1);
        ui->toolButton_add_arr->setEnabled(true);
        ui->toolButton_del_arr->setEnabled(false);
        ui->toolButton_change_arr->setEnabled(false);
        ui->toolButton_accept_arr->setEnabled(false);
        ui->toolButton_cancel_arr->setEnabled(false);
        ui->toolButton_ask_arr->setEnabled(true);
        ui->label_statusAction_arr->setText("");
        ui->widget_ob->setEnabled(true);

    }
    if (ui->label_statusAction_arr->text() == "Изменение данных о мере"){

        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,0);
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,1);
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,2);
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,3);
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,4);
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,5);
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,6);

        ui->toolButton_add_arr->setEnabled(true);
        ui->toolButton_del_arr->setEnabled(false);
        ui->toolButton_change_arr->setEnabled(false);
        ui->toolButton_accept_arr->setEnabled(false);
        ui->toolButton_cancel_arr->setEnabled(false);
        ui->toolButton_ask_arr->setEnabled(true);
        ui->label_statusAction_arr->setText("");
        ui->widget_ob->setEnabled(true);
    }

    //ui->tableWidget_ob->setSortingEnabled(true);
}

void MainWindow::on_toolButton_accept_arr_clicked()
{
    if (ui->label_statusAction_arr->text() == "Добавление новой меры"){

        int numberSelectedRow = ui->tableWidget_ob->selectedItems()[0]->row();
        int numberSelectedObject = -1;
        for(int i = 0 ; i < listOb.count() ; i++){
            if (listOb[i].objectTableRow == ui->tableWidget_ob->item(numberSelectedRow,0)){
                numberSelectedObject = i;
            }
        }

        ui->tableWidget_arr->setItem(ui->tableWidget_arr->rowCount()-1,0, new QTableWidgetItem); // в ячеку таблицы новый итем
        QTableWidgetItem* name = new QTableWidgetItem; // и дубликат в профиль, так как если делать без дубликата, то лист итемов будет ссылатся на табличные
        QLineEdit* nameLineEdit = (QLineEdit*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,0));
        ui->tableWidget_arr->item(ui->tableWidget_arr->rowCount()-1,0)->setText(nameLineEdit->text());
        name->setText(nameLineEdit->text());
        ui->tableWidget_arr->removeCellWidget(ui->tableWidget_arr->rowCount()-1,0);
        listOb[numberSelectedObject].profile[0]->append(name);

        ui->tableWidget_arr->setItem(ui->tableWidget_arr->rowCount()-1,1, new QTableWidgetItem);
        QTableWidgetItem* form = new QTableWidgetItem;
        QTextEdit* formTextEdit = (QTextEdit*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,1));
        ui->tableWidget_arr->item(ui->tableWidget_arr->rowCount()-1,1)->setText(formTextEdit->toPlainText());
        form->setText(formTextEdit->toPlainText());
        ui->tableWidget_arr->removeCellWidget(ui->tableWidget_arr->rowCount()-1,1);
        listOb[numberSelectedObject].profile[1]->append(form);

        ui->tableWidget_arr->setItem(ui->tableWidget_arr->rowCount()-1,2, new QTableWidgetItem);
        QTableWidgetItem* classSZI = new QTableWidgetItem;
        QComboBox* classSZIComboBox = (QComboBox*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,2));
        ui->tableWidget_arr->item(ui->tableWidget_arr->rowCount()-1,2)->setText(classSZIComboBox->currentText());
        classSZI->setText(classSZIComboBox->currentText());
        ui->tableWidget_arr->removeCellWidget(ui->tableWidget_arr->rowCount()-1,2);
        listOb[numberSelectedObject].profile[2]->append(classSZI);

        ui->tableWidget_arr->setItem(ui->tableWidget_arr->rowCount()-1,3, new QTableWidgetItem);
        QTableWidgetItem* lvlSZI = new QTableWidgetItem;
        QComboBox* lvlSZIComboBox = (QComboBox*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,3));
        ui->tableWidget_arr->item(ui->tableWidget_arr->rowCount()-1,3)->setText(lvlSZIComboBox->currentText());
        lvlSZI->setText(lvlSZIComboBox->currentText());
        ui->tableWidget_arr->removeCellWidget(ui->tableWidget_arr->rowCount()-1,3);
        listOb[numberSelectedObject].profile[3]->append(lvlSZI);

        ui->tableWidget_arr->setItem(ui->tableWidget_arr->rowCount()-1,4, new QTableWidgetItem);
        QTableWidgetItem* classSVT = new QTableWidgetItem;
        QComboBox* classSVTComboBox = (QComboBox*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,4));
        ui->tableWidget_arr->item(ui->tableWidget_arr->rowCount()-1,4)->setText(classSVTComboBox->currentText());
        classSVT->setText(classSVTComboBox->currentText());
        ui->tableWidget_arr->removeCellWidget(ui->tableWidget_arr->rowCount()-1,4);
        listOb[numberSelectedObject].profile[4]->append(classSVT);

        ui->tableWidget_arr->setItem(ui->tableWidget_arr->rowCount()-1,5, new QTableWidgetItem);
        QTableWidgetItem* doc = new QTableWidgetItem;
        QTextEdit* docTextEdit = (QTextEdit*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,5));
        ui->tableWidget_arr->item(ui->tableWidget_arr->rowCount()-1,5)->setText(docTextEdit->toPlainText());
        doc->setText(docTextEdit->toPlainText());
        ui->tableWidget_arr->removeCellWidget(ui->tableWidget_arr->rowCount()-1,5);
        listOb[numberSelectedObject].profile[5]->append(doc);

        ui->tableWidget_arr->setItem(ui->tableWidget_arr->rowCount()-1,6, new QTableWidgetItem);
        QTableWidgetItem* comment = new QTableWidgetItem;
        QTextEdit* commentTextEdit = (QTextEdit*)(ui->tableWidget_arr->cellWidget(ui->tableWidget_arr->rowCount()-1,6));
        ui->tableWidget_arr->item(ui->tableWidget_arr->rowCount()-1,6)->setText(commentTextEdit->toPlainText());
        comment->setText(commentTextEdit->toPlainText());
        ui->tableWidget_arr->removeCellWidget(ui->tableWidget_arr->rowCount()-1,6);
        listOb[numberSelectedObject].profile[6]->append(comment);

        checkStatus();

        ui->label_statusAction_arr->setText("");
        ui->toolButton_add_arr->setEnabled(true);
        ui->toolButton_del_arr->setEnabled(false);
        ui->toolButton_change_arr->setEnabled(false);
        ui->toolButton_accept_arr->setEnabled(false);
        ui->toolButton_cancel_arr->setEnabled(false);
        ui->toolButton_ask_arr->setEnabled(true);
        ui->widget_ob->setEnabled(true);

    }

    if (ui->label_statusAction_arr->text() == "Изменение данных о мере"){

        int numberSelectedRow = ui->tableWidget_ob->selectedItems()[0]->row();
        int numberSelectedObject = -1;
        for(int i = 0 ; i < listOb.count() ; i++){
            if (listOb[i].objectTableRow == ui->tableWidget_ob->item(numberSelectedRow,0)){
                numberSelectedObject = i;
            }
        }

        QLineEdit* name = (QLineEdit*)(ui->tableWidget_arr->cellWidget(chengedArrangementRow,0));
        ui->tableWidget_arr->item(chengedArrangementRow,0)->setText(name->text());
        listOb[numberSelectedObject].profile[0]->at(chengedArrangementRow)->setText(name->text());
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,0);

        QTextEdit* form = (QTextEdit*)(ui->tableWidget_arr->cellWidget(chengedArrangementRow,1));
        ui->tableWidget_arr->item(chengedArrangementRow,1)->setText(form->toPlainText());
        listOb[numberSelectedObject].profile[1]->at(chengedArrangementRow)->setText(form->toPlainText());
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,1);

        QComboBox* classSZI = (QComboBox*)(ui->tableWidget_arr->cellWidget(chengedArrangementRow,2));
        ui->tableWidget_arr->item(chengedArrangementRow,2)->setText(classSZI->currentText());
        listOb[numberSelectedObject].profile[2]->at(chengedArrangementRow)->setText(classSZI->currentText());
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,2);

        QComboBox* lvlSZI = (QComboBox*)(ui->tableWidget_arr->cellWidget(chengedArrangementRow,3));
        ui->tableWidget_arr->item(chengedArrangementRow,3)->setText(lvlSZI->currentText());
        listOb[numberSelectedObject].profile[3]->at(chengedArrangementRow)->setText(lvlSZI->currentText());
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,3);

        QComboBox* classSVT = (QComboBox*)(ui->tableWidget_arr->cellWidget(chengedArrangementRow,4));
        ui->tableWidget_arr->item(chengedArrangementRow,4)->setText(classSVT->currentText());
        listOb[numberSelectedObject].profile[4]->at(chengedArrangementRow)->setText(classSVT->currentText());
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,4);

        QTextEdit* doc = (QTextEdit*)(ui->tableWidget_arr->cellWidget(chengedArrangementRow,5));
        ui->tableWidget_arr->item(chengedArrangementRow,5)->setText(doc->toPlainText());
        listOb[numberSelectedObject].profile[5]->at(chengedArrangementRow)->setText(doc->toPlainText());
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,5);

        QTextEdit* comment = (QTextEdit*)(ui->tableWidget_arr->cellWidget(chengedArrangementRow,6));
        ui->tableWidget_arr->item(chengedArrangementRow,6)->setText(comment->toPlainText());
        listOb[numberSelectedObject].profile[6]->at(chengedArrangementRow)->setText(comment->toPlainText());
        ui->tableWidget_arr->removeCellWidget(chengedArrangementRow,6);

        checkStatus();

        ui->label_statusAction_arr->setText("");
        ui->toolButton_add_arr->setEnabled(true);
        ui->toolButton_del_arr->setEnabled(false);
        ui->toolButton_change_arr->setEnabled(false);
        ui->toolButton_accept_arr->setEnabled(false);
        ui->toolButton_cancel_arr->setEnabled(false);
        ui->toolButton_ask_arr->setEnabled(true);
        ui->widget_ob->setEnabled(true);
    }
}



void MainWindow::on_tableWidget_arr_itemSelectionChanged()
{
    if (ui->label_statusAction_arr->text() == ""){
        QList<int> numberSelectedRow;
        for (int i = 0 ; i < ui->tableWidget_arr->selectedItems().count() ; i++){
            if(!numberSelectedRow.contains(ui->tableWidget_arr->selectedItems()[i]->row()))
            {
                numberSelectedRow.append(ui->tableWidget_arr->selectedItems()[i]->row());
            }
        }
        if(numberSelectedRow.count()>0){
            ui->toolButton_del_arr->setEnabled(true);
        } else {
            ui->toolButton_del_arr->setEnabled(false);
        }
        if(numberSelectedRow.count()==1){
            ui->toolButton_change_arr->setEnabled(true);
        }else {
            ui->toolButton_change_arr->setEnabled(false);
        }
    }
}

void MainWindow::on_toolButton_del_arr_clicked()
{
    QString headerMes;
    QString mes;
    QList<int> numberSelectedRow;
    for (int i = 0 ; i < ui->tableWidget_arr->selectedItems().count() ; i++){
        if(!numberSelectedRow.contains(ui->tableWidget_arr->selectedItems()[i]->row()))
        {
            numberSelectedRow.append(ui->tableWidget_arr->selectedItems()[i]->row());
        }
    }
    if(numberSelectedRow.count()==1){
        headerMes="Удаление элемента";
        mes="Вы действительно хотите удалить выбранный элемент?";
    }else {
        headerMes="Удаление элементов";
        mes="Вы действительно хотите удалить "+ QString::number(numberSelectedRow.count())+" выбранных(-е) элемента(-ы)?";
    }
    if (!(QMessageBox::warning(0,
                               headerMes,
                               mes,
                               "Да",
                               "Нет",
                               QString(),
                               0,
                               1
                               ))){
        QList<int> numberSelectedRowReversed;
        numberSelectedRowReversed.reserve( numberSelectedRow.size() ); // reserve is new in Qt 4.7
        std::reverse_copy( numberSelectedRow.begin(), numberSelectedRow.end(), std::back_inserter( numberSelectedRowReversed ) );
        foreach(int n, numberSelectedRowReversed)
        {
            int numberSelectedObject;
            for ( int i = 0 ; i < listOb.count() ; i++ ) {
                if (listOb[i].objectTableRow == ui->tableWidget_ob->item(ui->tableWidget_ob->selectedItems()[0]->row(),0)){
                    numberSelectedObject = i;
                }
            }
            listOb[numberSelectedObject].profile[0]->removeAt(n);
            listOb[numberSelectedObject].profile[1]->removeAt(n);
            listOb[numberSelectedObject].profile[2]->removeAt(n);
            listOb[numberSelectedObject].profile[3]->removeAt(n);
            listOb[numberSelectedObject].profile[4]->removeAt(n);
            listOb[numberSelectedObject].profile[5]->removeAt(n);
            listOb[numberSelectedObject].profile[6]->removeAt(n);
            ui->tableWidget_arr->removeRow(n);
        }

        checkStatus();
    }
}

void MainWindow::on_toolButton_change_arr_clicked()
{
    ui->widget_ob->setEnabled(false);
    ui->toolButton_add_arr->setEnabled(false);
    ui->toolButton_del_arr->setEnabled(false);
    ui->toolButton_change_arr->setEnabled(false);
    ui->toolButton_accept_arr->setEnabled(true);
    ui->toolButton_cancel_arr->setEnabled(true);
    ui->toolButton_ask_arr->setEnabled(false);
    ui->label_statusAction_arr->setText("Изменение данных о мере");

    int numberSelectedObjectRow = ui->tableWidget_ob->selectedItems()[0]->row();
    int numberSelectedObject = -1;
    for(int i = 0 ; i < listOb.count() ; i++){
        if (listOb[i].objectTableRow == ui->tableWidget_ob->item(numberSelectedObjectRow,0)){
            numberSelectedObject = i;
        }
    }

    int numberSelectedArrangementRow = ui->tableWidget_arr->selectedItems()[0]->row();
    int numberSelectedArrangement = -1;
    for(int i = 0 ; i < listOb[numberSelectedObject].profile[0]->count() ; i++){
        if (listOb[numberSelectedObject].profile[0]->at(i) == ui->tableWidget_arr->item(numberSelectedArrangementRow,0)){ // не имеет смысла надо удалть
            numberSelectedArrangement = i; // останется равным -1 !!!!!!!!!!!!!!!
        }
    }

    chengedArrangementRow = numberSelectedArrangementRow;
    chengedArrangement = numberSelectedArrangement; // оно -1 и тут!!!!!!!!!!!!!!!

    ui->tableWidget_arr->setCellWidget(numberSelectedArrangementRow,0, new QLineEdit(ui->tableWidget_arr->item(numberSelectedArrangementRow,0)->text()));
    ui->tableWidget_arr->setCellWidget(numberSelectedArrangementRow,1, new QTextEdit(ui->tableWidget_arr->item(numberSelectedArrangementRow,1)->text()));

    QComboBox* classSZI = new QComboBox;
    ui->tableWidget_arr->setCellWidget(numberSelectedArrangementRow,2, classSZI);
    classSZI->addItem("Без СЗИ");
    classSZI->addItem("1");
    classSZI->addItem("2");
    classSZI->addItem("3");
    classSZI->addItem("4");
    classSZI->addItem("5");
    classSZI->addItem("6");
    classSZI->addItem("7");
    classSZI->setCurrentText(ui->tableWidget_arr->item(numberSelectedArrangementRow,2)->text());

    QComboBox* lvlSZI = new QComboBox;
    ui->tableWidget_arr->setCellWidget(numberSelectedArrangementRow,3, lvlSZI);
    lvlSZI->addItem("Без СЗИ");
    lvlSZI->addItem("1");
    lvlSZI->addItem("2");
    lvlSZI->addItem("3");
    lvlSZI->addItem("4");
    lvlSZI->addItem("5");
    lvlSZI->addItem("6");
    lvlSZI->setCurrentText(ui->tableWidget_arr->item(numberSelectedArrangementRow,3)->text());

    QComboBox* classSVT = new QComboBox;
    ui->tableWidget_arr->setCellWidget(numberSelectedArrangementRow,4, classSVT);
    classSVT->addItem("Без СВТ");
    classSVT->addItem("1");
    classSVT->addItem("2");
    classSVT->addItem("3");
    classSVT->addItem("4");
    classSVT->addItem("5");
    classSVT->addItem("6");
    classSVT->setCurrentText(ui->tableWidget_arr->item(numberSelectedArrangementRow,4)->text());

    ui->tableWidget_arr->setCellWidget(numberSelectedArrangementRow,5, new QTextEdit(ui->tableWidget_arr->item(numberSelectedArrangementRow,5)->text()));
    ui->tableWidget_arr->setCellWidget(numberSelectedArrangementRow,6, new QTextEdit(ui->tableWidget_arr->item(numberSelectedArrangementRow,6)->text()));
}

void MainWindow::on_toolButton_ask_arr_clicked()
{
    QString headerMes;
    QString mes;
    int numberSelectedObjectRow = ui->tableWidget_ob->selectedItems()[0]->row();
    int numberSelectedObject = -1;
    for(int i = 0 ; i < listOb.count() ; i++){
        if (listOb[i].objectTableRow == ui->tableWidget_ob->item(numberSelectedObjectRow,0)){
            numberSelectedObject = i;
        }
    }

    switch (ui->tableWidget_arr->rowCount()//listOb[numberSelectedObject].profile[0]->count()
            ) {

    default  :{
        if(ui->tableWidget_arr->rowCount()==1){
            headerMes="Изменение состава мер";
            mes="Изменение состава мер на минимально требуемый руководящими документами удалит имеющийся элемент. Вы действительно хотите выполнить это действие?";
        }else {
            headerMes="Изменение состава мер";
            mes="Изменение состава мер на минимально требуемый руководящими документами удалит имеющиеся "+ QString::number(ui->tableWidget_arr->rowCount())+" элемента(-ы). Вы действительно хотите выполнить это действие?";
        }
        if (!(QMessageBox::warning(0,
                                   headerMes,
                                   mes,
                                   "Да",
                                   "Нет",
                                   QString(),
                                   0,
                                   1
                                   ))){
            for ( int i = ui->tableWidget_arr->rowCount()-1 ; i > -1 ; i--) {
                listOb[numberSelectedObject].profile[0]->removeAt(i);
                listOb[numberSelectedObject].profile[1]->removeAt(i);
                listOb[numberSelectedObject].profile[2]->removeAt(i);
                listOb[numberSelectedObject].profile[3]->removeAt(i);
                listOb[numberSelectedObject].profile[4]->removeAt(i);
                listOb[numberSelectedObject].profile[5]->removeAt(i);
                listOb[numberSelectedObject].profile[6]->removeAt(i);
                ui->tableWidget_arr->removeRow(i);
            }
        } else {
            break;
        }
    }

    case 0 :{


        int catZOKII=4;
        int catISPDn=5;
        int catGIS=4;
        int minClassSZI=7;
        int minLvlSZI=7;
        int minClassSVT=8;
        if(!((listOb[numberSelectedObject].catZOKII == "Укажите кат.")||(listOb[numberSelectedObject].catZOKII == "-"))){
            catZOKII = listOb[numberSelectedObject].catZOKII.toInt();
            if ((listOb[numberSelectedObject].catZOKII.toInt() + 3)<minClassSZI){
                minClassSZI = listOb[numberSelectedObject].catZOKII.toInt() + 3;
            }
            if ((listOb[numberSelectedObject].catZOKII.toInt() + 3)<minLvlSZI){
                minLvlSZI = listOb[numberSelectedObject].catZOKII.toInt() + 3;
            }
            if (5<minClassSVT){
                minClassSVT = 5;
            }
        }
        if(!((listOb[numberSelectedObject].catISPDn == "Укажите кат.")||(listOb[numberSelectedObject].catISPDn == "-"))){
            catISPDn = listOb[numberSelectedObject].catISPDn.toInt();
            switch (listOb[numberSelectedObject].catISPDn.toInt()){
            case 4 :{
                if (6<minClassSZI){
                    minClassSZI = 6;
                }
                if (6<minClassSVT){
                    minClassSVT = 6;
                }
                break;
            }
            default  :{
                if ((listOb[numberSelectedObject].catISPDn.toInt() + 3)<minClassSZI){
                    minClassSZI = listOb[numberSelectedObject].catISPDn.toInt() + 3;
                }
                if (5<minClassSVT){
                    minClassSVT = 5;
                }
            }
            }
        }
        if(!((listOb[numberSelectedObject].catGIS == "Укажите кат.")||(listOb[numberSelectedObject].catGIS == "-"))){
            catGIS = listOb[numberSelectedObject].catGIS.toInt();
            if ((listOb[numberSelectedObject].catGIS.toInt() + 3)<minClassSZI){
                minClassSZI = listOb[numberSelectedObject].catGIS.toInt() + 3;
            }
            if ( ((listOb[numberSelectedObject].catGIS.toInt() == 1) || (listOb[numberSelectedObject].catGIS.toInt() == 2)) && (4<minLvlSZI) ){
                minLvlSZI = 4;
            }
            if (5<minClassSVT){
                minClassSVT = 5;
            }
        }


        // x-категория какого либо типа системы/объекта
        // x<0-отсудствие в приказе фстек меры
        // x<1-мера в приказе присудствует, но не обязательная для всех категорий
        // x<2-мера в приказе присудствует, но обязательная для 1 категории
        // x<3-мера в приказе присудствует, но обязательная для 1 и 2 категорий
        // x<4-мера в приказе присудствует, но обязательная для 1,2 и 3 категории в контексте ИСПДн; и мера в приказе присудствует и обязательна для всех категорий в контексте ЗОКИИ и ГИС
        // x<5-мера в приказе присудствует и обязательна для всех категорий ИСПДн


        //I. Идентификация и аутентификация (ИАФ)
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур идентификации и аутентификации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Идентификация и аутентификация пользователей и инициируемых ими процессов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Идентификация и аутентификация устройств",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление идентификаторами",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление средствами аутентификации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Идентификация и аутентификация внешних пользователей",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<1)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Защита обратной связи при вводе аутентификационной информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Защита аутентификационной информации при передаче",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //II. Управление доступом (УПД)
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур управления доступом",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление учетными записями пользователей",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Реализация модели управления доступом",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Доверенная загрузка",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<1)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление информационными потоками между устройствами, сегментами информационной системы, а также между информационными системами",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Разделение полномочий (ролей) пользователей",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Назначение минимально необходимых прав и привилегий",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Ограничение неуспешных попыток доступа в информационную (автоматизированную) систему",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<2)||(catISPDn<1)||(catGIS<2)){
            insertArr(listOb[numberSelectedObject],  "Ограничение числа параллельных сеансов доступа",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Блокирование сеанса доступа пользователя при неактивности",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление действиями пользователей до идентификации и аутентификации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Реализация защищенного удаленного доступа",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Контроль доступа из внешних информационных (автоматизированных) систем",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<1)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Регламентация и контроль использования в информационной системе технологий беспроводного доступа",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<1)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Регламентация и контроль использования в информационной системе мобильных технических средств",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //III. Ограничение программной среды (ОПС)
        if ((catZOKII<3)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур ограничения программной среды",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<2)||(catISPDn<1)||(catGIS<2)){
            insertArr(listOb[numberSelectedObject],  "Управление запуском (обращениями) компонентов программного обеспечения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Управление установкой (инсталляцией) компонентов программного обеспечения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<2)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Установка (инсталляция) только разрешенного к использованию программного обеспечения и (или) его компонентов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //IV. Защита машинных носителей информации (ЗНИ)
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур защиты машинных носителей информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Учет машинных носителей информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление физическим доступом к машинным носителям информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Контроль использования интерфейсов ввода (вывода) информации на съемные машинные носители информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<2)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Контроль ввода (вывода) информации на съемные машинные носители информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Контроль подключения съемных машинных носителей информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Уничтожение (стирание) информации на машинных носителях информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //V. Аудит безопасности (АУД) по 239 или V. Регистрация событий безопасности (РСБ) по 17 и 21
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур аудита безопасности",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Инвентаризация информационных ресурсов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Определение событий безопасности, подлежащих регистрации, и сроков их хранения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Определение состава и содержания информации о событиях безопасности, подлежащих регистрации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Анализ уязвимостей и их устранение",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Генерирование временных меток и (или) синхронизация системного времени",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Сбор, запись и хранение информации о событиях безопасности в течение установленного времени хранения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<2)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Контроль и анализ сетевого трафика",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Реагирование на сбои при регистрации событий безопасности, в том числе аппаратные и программные ошибки, сбои в механизмах сбора информации и достижение предела или переполнения объема (емкости) памяти",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Защита информации о событиях безопасности",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Мониторинг (просмотр, анализ) результатов регистрации событий безопасности и реагирование на них",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<2)||(catISPDn<0)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Анализ действий отдельных пользователей",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Проведение внутренних аудитов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //VI. Антивирусная защита (АВЗ)
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур антивирусной защиты",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Реализация антивирусной защиты",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Антивирусная защита электронной почты и иных сервисов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<2)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Контроль использования архивных, исполняемых и зашифрованных файлов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Обновление базы данных признаков вредоносных компьютерных программ (вирусов)",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<2)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Использование средств антивирусной защиты различных производителей",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //VII. Предотвращение вторжений (компьютерных атак) (СОВ)
        if ((catZOKII<3)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур предотвращения вторжений (компьютерных атак)",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Обнаружение и предотвращение компьютерных атак",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Обновление базы решающих правил",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //VIII. Контроль (анализ) защищенности персональных данных (АНЗ) по 17 или VIII. Контроль (анализ) защищенности информации (АНЗ) по 21, в 239 такого нет
        if ((catZOKII<0)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Выявление, анализ уязвимостей информационной системы и оперативное устранение вновь выявленных уязвимостей",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Контроль установки обновлений программного обеспечения, включая обновление программного обеспечения средств защиты информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Контроль работоспособности, параметров настройки и правильности функционирования программного обеспечения и средств защиты информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Контроль состава технических средств, программного обеспечения и средств защиты информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<3)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Контроль правил генерации и смены паролей пользователей, заведения и удаления учетных записей пользователей, реализации правил разграничения доступа, полномочий пользователей в информационной системе",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //VIII. Обеспечение целостности (ОЦЛ) по 239 или IХ.Обеспечение целостности информационной системы и персональных данных (ОЦЛ) по 17 или IX. Обеспечение целостности информационной системы и информации (ОЦЛ) по 21
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур обеспечения целостности",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Контроль целостности программного обеспечения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<2)||(catISPDn<1)||(catGIS<2)){
            insertArr(listOb[numberSelectedObject],  "Ограничения по вводу информации в информационную (автоматизированную) систему",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<3)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Обеспечение возможности восстановления программного обеспечения, включая программное обеспечение средств защиты информации, при возникновении нештатных ситуаций",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Контроль данных, вводимых в информационную (автоматизированную) систему",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Обнаружение и реагирование на поступление в информационную систему незапрашиваемых электронных сообщений (писем, документов) и иной информации, не относящихся к функционированию информационной системы (защита от спама)",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Контроль ошибочных действий пользователей по вводу и (или) передаче информации и предупреждение пользователей об ошибочных действиях",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //IX. Обеспечение доступности (ОДТ) по 239 или X. Обеспечение доступности персональных данных (ОДТ) по 17 или X. Обеспечение доступности информации (ОДТ) по 21
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур обеспечения доступности",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<1)||(catGIS<2)){
            insertArr(listOb[numberSelectedObject],  "Использование отказоустойчивых технических средств",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<1)||(catGIS<2)){
            insertArr(listOb[numberSelectedObject],  "Резервирование средств и систем",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<2)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Контроль безотказного функционирования средств и систем",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Резервное копирование информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Обеспечение возможности восстановления информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Обеспечение возможности восстановления программного обеспечения при нештатных ситуациях",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Контроль предоставляемых вычислительных ресурсов и каналов связи",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //XI. Защита среды виртуализации (ЗСВ) по 17 и 21, в 239 такого нет
        if ((catZOKII<0)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Идентификация и аутентификация субъектов доступа и объектов доступа в виртуальной инфраструктуре, в том числе администраторов управления средствами виртуализации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление доступом субъектов доступа к объектам доступа в виртуальной инфраструктуре, в том числе внутри виртуальных машин",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Регистрация событий безопасности в виртуальной инфраструктуре",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<1)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Управление (фильтрация, маршрутизация, контроль соединения, однонаправленная передача) потоками информации между компонентами виртуальной инфраструктуры, а также по периметру виртуальной инфраструктуры",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Управление перемещением виртуальных машин (контейнеров) и обрабатываемых на них данных",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Контроль целостности виртуальной инфраструктуры и ее конфигураций",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Резервное копирование данных, резервирование технических средств, программного обеспечения виртуальной инфраструктуры, а также каналов связи внутри виртуальной инфраструктуры",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Реализация и управление антивирусной защитой в виртуальной инфраструктуре",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Разбиение виртуальной инфраструктуры на сегменты (сегментирование виртуальной инфраструктуры) для обработки персональных данных отдельным пользователем и (или) группой пользователей",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //X. Защита технических средств и систем (ЗТС) по 239 или XII. Защита технических средств (ЗТС) по 17 и 21
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур защиты технических средств и систем",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Организация контролируемой зоны",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление физическим доступом",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Размещение устройств вывода (отображения) информации, исключающее ее несанкционированный просмотр",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<2)){
            insertArr(listOb[numberSelectedObject],  "Защита от внешних воздействий",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //XI. Защита информационной (автоматизированной) системы и ее компонентов (ЗИС) по 239 или XIII. Защита информационной системы, ее средств, систем связи и передачи данных (ЗИС) по 17 и 21
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур защиты информационной (автоматизированной) системы и ее компонентов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<2)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Разделение функций по управлению (администрированию) информационной (автоматизированной) системой с иными функциями",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Защита периметра информационной (автоматизированной) системы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Эшелонированная защита информационной (автоматизированной) системы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Обеспечение защиты информации от раскрытия, модификации и навязывания (ввода ложной информации) при ее передаче (подготовке к передаче) по каналам связи, имеющим выход за пределы контролируемой зоны, в том числе беспроводным каналам связи",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Защита информации при ее передаче по каналам связи  ",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Сегментирование информационной (автоматизированной) системы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Организация демилитаризованной зоны",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Запрет несанкционированной удаленной активации видеокамер, микрофонов и иных периферийных устройств, которые могут активироваться удаленно, и оповещение пользователей об активации таких устройств",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление сетевыми потоками",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "Подразумевается в рамках меры УПД.3 17 и 21 приказов ФСТЭК");
        }
        if ((catZOKII<1)||(catISPDn<1)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Контроль использования мобильного кода",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<1)||(catISPDn<1)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Контроль передачи речевой информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<1)||(catISPDn<1)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Контроль передачи видеоинформации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Сокрытие архитектуры и конфигурации информационной (автоматизированной) системы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Обеспечение подлинности сетевых соединений (сеансов взаимодействия), в том числе для защиты от подмены сетевых устройств и сервисов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<1)||(catISPDn<1)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Исключение возможности отрицания отправки информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<1)||(catISPDn<1)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Исключение возможности отрицания получения информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<3)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Защита архивных файлов, параметров настройки средств защиты информации и программного обеспечения и иных данных, не подлежащих изменению в процессе обработки информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<4)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Защита беспроводных соединений",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<2)||(catISPDn<0)||(catGIS<2)){
            insertArr(listOb[numberSelectedObject],  "Исключение доступа через общие ресурсы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Защита от угроз отказа в обслуживании (DOS, DDOS-атак)",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<5)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Управление сетевыми соединениями",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "Подразумевается в рамках меры УПД.3 17 и 21 приказов ФСТЭК");
        }
        if ((catZOKII<0)||(catISPDn<0)||(catGIS<3)){
            insertArr(listOb[numberSelectedObject],  "Прекращение сетевых соединений по их завершении или по истечении заданного оператором временного интервала неактивности сетевого соединения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<4)){
            insertArr(listOb[numberSelectedObject],  "Защита применяемых мобильных технических средств",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<1)||(catGIS<1)){
            insertArr(listOb[numberSelectedObject],  "Обеспечение доверенных канала, маршрута",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //XII. Реагирование на компьютерные инциденты (ИНЦ) по 239 или XIV. Выявление инцидентов и реагирование на них (ИНЦ) по 17, в 21 такого нет
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур реагирования на компьютерные инциденты",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Выявление компьютерных инцидентов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Информирование о компьютерных инцидентах",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Анализ компьютерных инцидентов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Устранение последствий компьютерных инцидентов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<3)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Принятие мер по предотвращению повторного возникновения компьютерных инцидентов",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Хранение и защита информации о компьютерных инцидентах",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<3)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Определение лиц, ответственных за выявление инцидентов и реагирование на них",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //XIII. Управление конфигурацией (УКФ) по 239 или XV. Управление конфигурацией информационной системы и системы защиты персональных данных (УКФ) по 17, в 21 такого нет
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур управления конфигурацией информационной (автоматизированной) системы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<4)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Управление изменениями",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<4)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Определение лиц, которым разрешены действия по внесению изменений в конфигурацию информационной системы и системы защиты персональных данных",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<0)||(catISPDn<4)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Анализ потенциального воздействия планируемых изменений в конфигурации информационной системы и системы защиты персональных данных на обеспечение защиты персональных данных и согласование изменений в конфигурации информационной системы с должностным лицом (работником), ответственным за обеспечение безопасности персональных данных",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<1)||(catISPDn<4)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Документирование информации (данных) об изменениях в конфигурации информационной системы и системы защиты персональных данных",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //XIV. Управление обновлениями программного обеспечения (ОПО) по 239, в 17 и 21 такого нет
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур управления обновлениями программного обеспечения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Поиск, получение обновлений программного обеспечения от доверенного источника",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Контроль целостности обновлений программного обеспечения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Тестирование обновлений программного обеспечения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Установка обновлений программного обеспечения",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //XV. Планирование мероприятий по обеспечению безопасности (ПЛН) по 239, в 17 и 21 такого нет
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур планирования мероприятий по обеспечению защиты информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Разработка, утверждение и актуализация плана мероприятий по обеспечению защиты информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Контроль выполнения мероприятий по обеспечению защиты информации",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //XVI. Обеспечение действий в нештатных  ситуациях (ДНС) по 239, в 17 и 21 такого нет
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур обеспечения действий в нештатных  ситуациях",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Разработка плана действий в нештатных ситуациях",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Обучение и отработка действий персонала в нештатных ситуациях",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Создание альтернативных мест хранения и обработки информации на случай возникновения нештатных ситуаций",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Резервирование программного обеспечения, технических средств, каналов связи на случай возникновения нештатных  ситуаций",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Обеспечение возможности восстановления информационной (автоматизированной) системы в случае возникновения нештатных ситуаций",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        //XVII. Информирование и обучение персонала (ИПО) по 239, в 17 и 21 такого нет
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Регламентация правил и процедур информирования и обучения персонала",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Информирование персонала об угрозах безопасности информации и о правилах безопасной работы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Обучение персонала правилам безопасной работы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<3)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Проведение практических занятий с персоналом по правилам безопасной работы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }
        if ((catZOKII<4)||(catISPDn<0)||(catGIS<0)){
            insertArr(listOb[numberSelectedObject],  "Контроль осведомленности персонала об угрозах безопасности информации и о правилах безопасной работы",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }

        /*if ((catZOKII<)||(catISPDn<)||(catGIS<)){
            insertArr(listOb[numberSelectedObject],  "",  "",  "Без СЗИ",  "Без СЗИ",  "Без СВТ",  "",  "");
        }*/

        QString minClassSZI2text;
        QString minLvlSZI2text;
        QString minClassSVT2text;
        if (minClassSZI<7){
            minClassSZI2text = QString::number(minClassSZI);
        } else {
            minClassSZI2text = "Без СЗИ";
        }
        if (minLvlSZI<7){
            minLvlSZI2text = QString::number(minLvlSZI);
        } else {
            minLvlSZI2text = "Без СЗИ";
        }
        if (minClassSVT<8){
            minClassSVT2text = QString::number(minClassSVT);
        } else {
            minClassSVT2text = "Без СВТ";
        }
        if ((!(minClassSZI2text == "Без СЗИ"))||(!(minLvlSZI2text == "Без СЗИ"))){
            insertArr(listOb[numberSelectedObject],  "Минимальный класс/ур. доверия СЗИ",  "",  minClassSZI2text,  minLvlSZI2text,  "Без СВТ",  "",  "");
        }
        if (!(minClassSVT2text == "Без СВТ")){
            insertArr(listOb[numberSelectedObject],  "Минимальный класс СВТ",  "",  "Без СЗИ",  "Без СЗИ",  minClassSVT2text,  "",  "");
        }


        for (int j = 0 ; j < listOb[numberSelectedObject].profile[0]->count() ; j++) {

            ui->tableWidget_arr->insertRow(ui->tableWidget_arr->rowCount());

            QTableWidgetItem* name = new QTableWidgetItem;
            name->setText(listOb[numberSelectedObject].profile[0]->at(j)->text());
            ui->tableWidget_arr->setItem(j,0, name);
            QTableWidgetItem* form = new QTableWidgetItem;
            form->setText(listOb[numberSelectedObject].profile[1]->at(j)->text());
            ui->tableWidget_arr->setItem(j,1, form);
            QTableWidgetItem* classSZI = new QTableWidgetItem;
            classSZI->setText(listOb[numberSelectedObject].profile[2]->at(j)->text());
            ui->tableWidget_arr->setItem(j,2, classSZI);
            QTableWidgetItem* lvlSZI = new QTableWidgetItem;
            lvlSZI->setText(listOb[numberSelectedObject].profile[3]->at(j)->text());
            ui->tableWidget_arr->setItem(j,3, lvlSZI);
            QTableWidgetItem* classSVT = new QTableWidgetItem;
            classSVT->setText(listOb[numberSelectedObject].profile[4]->at(j)->text());
            ui->tableWidget_arr->setItem(j,4, classSVT);
            QTableWidgetItem* doc = new QTableWidgetItem;
            doc->setText(listOb[numberSelectedObject].profile[5]->at(j)->text());
            ui->tableWidget_arr->setItem(j,5, doc);
            QTableWidgetItem* comment = new QTableWidgetItem;
            comment->setText(listOb[numberSelectedObject].profile[6]->at(j)->text());
            ui->tableWidget_arr->setItem(j,6, comment);
        }

        checkStatus();

    }
    }

}

void MainWindow::insertArr(Object object, QString name, QString form, QString classSZI, QString lvlSZI, QString classSVT, QString doc, QString comment){
    object.profile[0]->append(new QTableWidgetItem(name));
    object.profile[1]->append(new QTableWidgetItem(form));
    object.profile[2]->append(new QTableWidgetItem(classSZI));
    object.profile[3]->append(new QTableWidgetItem(lvlSZI));
    object.profile[4]->append(new QTableWidgetItem(classSVT));
    object.profile[5]->append(new QTableWidgetItem(doc));
    object.profile[6]->append(new QTableWidgetItem(comment));
}

void MainWindow::checkStatus(){

    int counterNoForm = 0;
    int counterIncorrectClassSZI = 0;
    int counterIncorrectLvlSZI = 0;
    int counterIncorrectClassSVT = 0;

    int numberSelectedObjectRow = ui->tableWidget_ob->selectedItems()[0]->row();
    int numberSelectedObject = -1;
    for(int i = 0 ; i < listOb.count() ; i++){
        if (listOb[i].objectTableRow == ui->tableWidget_ob->item(numberSelectedObjectRow,0)){
            numberSelectedObject = i;
        }
    }

    int catZOKII=4;
    int catISPDn=5;
    int catGIS=4;
    int minClassSZI=7;
    int minLvlSZI=7;
    int minClassSVT=8;
    if(!((listOb[numberSelectedObject].catZOKII == "Укажите кат.")||(listOb[numberSelectedObject].catZOKII == "-"))){
        catZOKII = listOb[numberSelectedObject].catZOKII.toInt();
        if ((listOb[numberSelectedObject].catZOKII.toInt() + 3)<minClassSZI){
            minClassSZI = listOb[numberSelectedObject].catZOKII.toInt() + 3;
        }
        if ((listOb[numberSelectedObject].catZOKII.toInt() + 3)<minLvlSZI){
            minLvlSZI = listOb[numberSelectedObject].catZOKII.toInt() + 3;
        }
        if (5<minClassSVT){
            minClassSVT = 5;
        }
    }
    if(!((listOb[numberSelectedObject].catISPDn == "Укажите кат.")||(listOb[numberSelectedObject].catISPDn == "-"))){
        catISPDn = listOb[numberSelectedObject].catISPDn.toInt();
        switch (listOb[numberSelectedObject].catISPDn.toInt()){
        case 4 :{
            if (6<minClassSZI){
                minClassSZI = 6;
            }
            if (6<minClassSVT){
                minClassSVT = 6;
            }
            break;
        }
        default  :{
            if ((listOb[numberSelectedObject].catISPDn.toInt() + 3)<minClassSZI){
                minClassSZI = listOb[numberSelectedObject].catISPDn.toInt() + 3;
            }
            if (5<minClassSVT){
                minClassSVT = 5;
            }
        }
        }
    }
    if(!((listOb[numberSelectedObject].catGIS == "Укажите кат.")||(listOb[numberSelectedObject].catGIS == "-"))){
        catGIS = listOb[numberSelectedObject].catGIS.toInt();
        if ((listOb[numberSelectedObject].catGIS.toInt() + 3)<minClassSZI){
            minClassSZI = listOb[numberSelectedObject].catGIS.toInt() + 3;
        }
        if ( ((listOb[numberSelectedObject].catGIS.toInt() == 1) || (listOb[numberSelectedObject].catGIS.toInt() == 2)) && (4<minLvlSZI) ){
            minLvlSZI = 4;
        }
        if (5<minClassSVT){
            minClassSVT = 5;
        }
    }

    for (int i = 0 ; i < ui->tableWidget_arr->rowCount() ; i++) {

        int classSZI = 0;
        int lvlSZI = 0;
        int classSVT = 0;

        if(ui->tableWidget_arr->item(i,2)->text() != "Без СЗИ"){
            QString classSZIText = ui->tableWidget_arr->item(i,2)->text();
            classSZI = classSZIText.toInt();
        }
        if(ui->tableWidget_arr->item(i,3)->text() != "Без СЗИ"){
            QString lvlSZIText = ui->tableWidget_arr->item(i,3)->text();
            lvlSZI = lvlSZIText.toInt();
        }
        if(ui->tableWidget_arr->item(i,4)->text() != "Без СВТ"){
            QString classSVTText = ui->tableWidget_arr->item(i,4)->text();
            classSVT = classSVTText.toInt();
        }

        if((ui->tableWidget_arr->item(i,1)->text().replace( " ", "" ) == "")||(classSZI>minClassSZI)||(lvlSZI>minLvlSZI)||(classSVT>minClassSVT)){

            for (int j = 0 ; j < 7; j++) {
                ui->tableWidget_arr->item(i,j)->setBackgroundColor(Qt::red);
            }

            if(ui->tableWidget_arr->item(i,1)->text().replace( " ", "" ) == ""){
                ui->tableWidget_arr->item(i,1)->setBackgroundColor(Qt::darkRed);
                counterNoForm++;
            }
            if(classSZI>minClassSZI){
                ui->tableWidget_arr->item(i,2)->setBackgroundColor(Qt::darkRed);
                counterIncorrectClassSZI++;
            }
            if(lvlSZI>minLvlSZI){
                ui->tableWidget_arr->item(i,3)->setBackgroundColor(Qt::darkRed);
                counterIncorrectLvlSZI++;
            }
            if(classSVT>minClassSVT){
                ui->tableWidget_arr->item(i,4)->setBackgroundColor(Qt::darkRed);
                counterIncorrectClassSVT++;
            }

        }else {
            for (int j = 0 ; j < 7; j++) {
                ui->tableWidget_arr->item(i,j)->setBackgroundColor(Qt::green);
            }
        }
    }

    ui->tableWidget_ob->item(numberSelectedObjectRow,4)->setText("");
    listOb[numberSelectedObject].status = "";
    if((counterNoForm>0)||(counterIncorrectClassSZI>0)||(counterIncorrectLvlSZI>0)||(counterIncorrectClassSVT>0)){

        if(counterNoForm>0){
            ui->tableWidget_ob->item(numberSelectedObjectRow,4)->setText(ui->tableWidget_ob->item(numberSelectedObjectRow,4)->text() + "Без указания реализации:" + QString::number(counterNoForm) + " ");
            listOb[numberSelectedObject].status = listOb[numberSelectedObject].status + "Без указания реализации:" + QString::number(counterNoForm) + " ";
        }
        if(counterIncorrectClassSZI>0){
            ui->tableWidget_ob->item(numberSelectedObjectRow,4)->setText(ui->tableWidget_ob->item(numberSelectedObjectRow,4)->text() + "Не соответствующий класс СЗИ:" + QString::number(counterIncorrectClassSZI) + " ");
            listOb[numberSelectedObject].status = listOb[numberSelectedObject].status + "Не соответствующий класс СЗИ:" + QString::number(counterIncorrectClassSZI) + " ";
        }
        if(counterIncorrectLvlSZI>0){
            ui->tableWidget_ob->item(numberSelectedObjectRow,4)->setText(ui->tableWidget_ob->item(numberSelectedObjectRow,4)->text() + "Не соответствующий уровень доверия СЗИ:" + QString::number(counterIncorrectLvlSZI) + " ");
            listOb[numberSelectedObject].status = listOb[numberSelectedObject].status + "Не соответствующий уровень доверия СЗИ:" + QString::number(counterIncorrectLvlSZI) + " ";
        }
        if(counterIncorrectClassSVT>0){
            ui->tableWidget_ob->item(numberSelectedObjectRow,4)->setText(ui->tableWidget_ob->item(numberSelectedObjectRow,4)->text() + "Не соответствующий класс СВТ:" + QString::number(counterIncorrectClassSVT) + " ");
            listOb[numberSelectedObject].status = listOb[numberSelectedObject].status + "Не соответствующий класс СВТ:" + QString::number(counterIncorrectClassSVT) + " ";
        }

    }else{
        ui->tableWidget_ob->item(numberSelectedObjectRow,4)->setText("Все впорядке");
        listOb[numberSelectedObject].status = "Все впорядке";
    }
    refreshNotification();
}

void MainWindow::on_tableWidget_arr_itemDoubleClicked(QTableWidgetItem *item)
{
    if (ui->label_statusAction_arr->text()==""){
        on_toolButton_change_arr_clicked();
    }
}

void MainWindow::on_tableWidget_ob_itemDoubleClicked(QTableWidgetItem *item)
{
    if (ui->label_statusAction_ob->text()==""){
        on_toolButton_change_ob_clicked();
    }
}

void MainWindow::on_listWidget_org_itemDoubleClicked(QListWidgetItem *item)
{
    if (ui->label_statusAction_org->text()==""){
        on_toolButton_change_org_clicked();
    }
}

void MainWindow::notificationButton_clicked(){
    if(notificationButton->text()!="0"){
        QMessageBox::warning(this, "Уведомления", notificationMessage);
    }
}

void MainWindow::refreshNotification(){

    notificationMessage = "Обратите внимание на объекты: ";
    notificationButton->setText("0");
    for (int i = 0 ; i < ui->tableWidget_ob->rowCount() ; i++) {
        if ((ui->tableWidget_ob->item(i,4)->text() != "Новый") && (ui->tableWidget_ob->item(i,4)->text() != "Все впорядке")){
            notificationMessage = notificationMessage + "\"" + ui->tableWidget_ob->item(i,1)->text() + "\", ";
            notificationButton->setText(QString::number( notificationButton->text().toInt()+1 ) );
        }
    }
    notificationMessage.remove(notificationMessage.count()-2,3);
    notificationMessage = notificationMessage + ".";

}

void MainWindow::on_evaluation_1_valueChanged(const QString &arg1){refreshEvaluatioResult();}
void MainWindow::on_evaluation_2_valueChanged(const QString &arg1){refreshEvaluatioResult();}
void MainWindow::on_evaluation_3_valueChanged(const QString &arg1){refreshEvaluatioResult();}
void MainWindow::on_evaluation_4_valueChanged(const QString &arg1){refreshEvaluatioResult();}
void MainWindow::on_evaluation_5_valueChanged(const QString &arg1){refreshEvaluatioResult();}
void MainWindow::on_evaluation_6_valueChanged(const QString &arg1){refreshEvaluatioResult();}
void MainWindow::on_evaluation_7_valueChanged(const QString &arg1){refreshEvaluatioResult();}
void MainWindow::on_evaluation_8_valueChanged(const QString &arg1){refreshEvaluatioResult();}

void MainWindow::refreshEvaluatioResult()
{
    int last_value = ui->evaluation_result->text().toInt();
    int new_value = 0;
    QList<int> count = {0,0,0,0};
    count[ui->evaluation_1->value()-2]=count[ui->evaluation_1->value()-2]+1;
    count[ui->evaluation_2->value()-2]=count[ui->evaluation_2->value()-2]+1;
    count[ui->evaluation_3->value()-2]=count[ui->evaluation_3->value()-2]+1;
    count[ui->evaluation_4->value()-2]=count[ui->evaluation_4->value()-2]+1;
    count[ui->evaluation_5->value()-2]=count[ui->evaluation_5->value()-2]+1;
    count[ui->evaluation_6->value()-2]=count[ui->evaluation_6->value()-2]+1;
    count[ui->evaluation_7->value()-2]=count[ui->evaluation_7->value()-2]+1;
    count[ui->evaluation_8->value()-2]=count[ui->evaluation_8->value()-2]+1;

    if ((count[3]>3)&&(count[1]==0)&&(count[0]==0)){
        new_value = 5;
    }else{
        if ((count[3]+count[2]>3)&&(count[0]==0)){
            new_value = 4;
        }else{
            if ((count[3]+count[2]<4)&&(count[0]==0)){
                new_value = 3;
            }else{
                new_value = 2;
            }
        }
    }
    if (last_value != new_value){ui->evaluation_result->setText(QString::number(new_value)) ; refreshGraphic(); }
}

void MainWindow::on_allowable_reduction_valueChanged(const QString &arg1){refreshGraphic();}
void MainWindow::on_information_security_status_valueChanged(const QString &arg1){refreshGraphic();}
void MainWindow::on_required_reliability_valueChanged(const QString &arg1){refreshGraphic();}

void  MainWindow::refreshGraphic(){

    double BI = 0;
    switch (ui->evaluation_result->text().toInt()) {
    case 2:{BI = 0.3; break;}
    case 3:{BI = 0.6; break;}
    case 4:{BI = 0.8; break;}
    case 5:{BI = 0.9; break;}
    default:{break;}
    }
    double deltaK = ui->allowable_reduction->value();
    double BItr = ui->information_security_status->value();
    double Dtr = ui->required_reliability->value();

    QVector<double> x0(251), y0(251), x1(2), y1(2);
    y1[0] = 1 - ((1 - deltaK) * qPow( 2.71828182845904523536 , ( ( BI / BItr ) * qLn( 1 - Dtr ) ) ) );
    y1[1] = y1[0];
    x1[0] = 0;
    x1[1] = 0;
    for (int i=0; i<251; i++)
    {
        x0[i] = (double)i/250;
        y0[i] = 1 - ((1 - deltaK) * qPow( 2.71828182845904523536 , ( ( x0[i] / BItr ) * qLn( 1 - Dtr ) ) ) );
        if(y0[i]<=y1[0]){ x1[1] = x0[i]; }
    }

    ui->grafic->xAxis2->setVisible(true);
    ui->grafic->xAxis2->setTickLabels(false);
    ui->grafic->yAxis2->setVisible(true);
    ui->grafic->yAxis2->setTickLabels(false);

    connect(ui->grafic->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->grafic->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->grafic->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->grafic->yAxis2, SLOT(setRange(QCPRange)));

    ui->grafic->graph(0)->setData(x0, y0);
    ui->grafic->graph(1)->setData(x1, y1);

    ui->grafic->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    ui->grafic->xAxis->setRange(0,1);
    ui->grafic->yAxis->setRange(deltaK,1);
    ui->grafic->replot();
    ui->ecKof_result->setText(QString::number(y1[0]));
}

