#ifndef OBJECT_H
#define OBJECT_H

#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QPointer>
#include <QList>

class Object
{
public:

    QTableWidgetItem* objectTableRow;
    QListWidgetItem* nameOrg;  
    QString name;
    QString type;
    QString activityOrg;
    QString status;
    QString catZOKII;
    QString catISPDn;
    QString catGIS;
    QList<QList<QTableWidgetItem*>*> profile;

    Object();

};

#endif // OBJECT_H
