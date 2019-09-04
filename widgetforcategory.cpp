#include "widgetforcategory.h"

WidgetForCategory::WidgetForCategory()
{
    catButton = new QToolButton(this);
    catButton->setText("Определить");
    catButton->setMaximumSize(QSize(24, 24));
    catButton->setMinimumSize(QSize(24, 24));
    catButton->setIcon(QIcon(":icon/img/test.png"));

    catBox = new QComboBox(this);
    catBox->addItem("Укажите кат.");
    catBox->addItem("-");
    catBox->addItem("1");
    catBox->addItem("2");
    catBox->addItem("3");

    catLayout = new QHBoxLayout(this);
    catLayout->setMargin(0);
    catLayout->setSpacing(0);
    catLayout->addWidget(catButton);
    catLayout->addWidget(catBox);

    this->setMinimumWidth(49);
    this->resize(125, this->height());
}

WidgetForCategory::~WidgetForCategory()
{
     catButton->deleteLater();
     catBox->deleteLater();
     catLayout->deleteLater();
}
