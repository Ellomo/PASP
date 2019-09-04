#ifndef ORGANIZATION_H
#define ORGANIZATION_H

#include <QListWidgetItem>

class Organization
{
public:
    QListWidgetItem Name;
    QString Type;
    QString INN;
    QString ORGN;
    QString YAdr;
    QString FAdr;
    Organization();


};

#endif // ORGANIZATION_H
