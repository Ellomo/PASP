#ifndef WIDGETFORCATEGORY_H
#define WIDGETFORCATEGORY_H

#include <QWidget>
#include <QToolButton>
#include <QComboBox>
#include <QHBoxLayout>


class WidgetForCategory : public QWidget
{
public:

    QToolButton* catButton;
    QComboBox* catBox;
    QHBoxLayout* catLayout;

    WidgetForCategory();
    ~WidgetForCategory();
};

#endif // WIDGETFORCATEGORY_H
