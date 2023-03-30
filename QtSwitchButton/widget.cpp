#include "widget.h"
#include "ui_widget.h"

#include "switchbutton.h"
#include <QPainter>
#include <QVariantAnimation>

const char * style_sheet = \
R"(
SwitchButton{
    background:#A7A7A7; /*Unchecked背景*/
    border: none;
    border-radius: 15px; /*圆角*/
    height: 30px;
    width: 80px;
}
SwitchButton:checked{
    background: #4CCCE6;
}
SwitchButton::handle{
    background: #DDDDDD;
    border:0px solid;
    min-width:30px;
    max-width:30px;
    border-radius:15px; /*圆角*/
}
SwitchButton::handle:checked{
    background: red;
}
)";

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setStyleSheet(style_sheet);
    new SwitchButton(this);
}

Widget::~Widget()
{
    delete ui;
}
