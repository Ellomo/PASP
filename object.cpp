#include "object.h"

Object::Object()
{
    nameOrg = NULL;
    profile.append(new QList<QTableWidgetItem*>); // name
    profile.append(new QList<QTableWidgetItem*>); // form
    profile.append(new QList<QTableWidgetItem*>); // classSZI
    profile.append(new QList<QTableWidgetItem*>); // lvlSZI
    profile.append(new QList<QTableWidgetItem*>); // classSVT
    profile.append(new QList<QTableWidgetItem*>); // doc
    profile.append(new QList<QTableWidgetItem*>); // coment
}


