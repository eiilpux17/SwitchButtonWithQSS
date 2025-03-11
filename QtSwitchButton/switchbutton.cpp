#include "switchbutton.h"

#include <QPainter>
#include <QStyleOptionSlider>
#include <QVariantAnimation>

SwitchButton::SwitchButton(QWidget *parent) : QPushButton(parent)
{
    setCheckable(true);

    animation = new QVariantAnimation(this);
    animation->setStartValue(0.0);
    animation->setEndValue(1.0);
    animation->setDuration(150);
    connect(animation, &QVariantAnimation::valueChanged, this, [this](const QVariant & val){
        progress = val.toReal();    // progress定义为成员
        update();
    });

    connect(this, &SwitchButton::toggled, this, &SwitchButton::onToggled);
}

void SwitchButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QStyleOptionButton buttonOpt;
    initStyleOption(&buttonOpt);    // 初始化状态
    buttonOpt.rect.adjust(0, 0, -1, 0); // 绘制滑块可能会有一像素偏差
    buttonOpt.state &= ~QStyle::State_On;   // 先绘制Unchecked时背景
    style()->drawControl(QStyle::CE_PushButtonBevel, &buttonOpt, &painter, this);

    painter.setOpacity(progress);   // 定义一个动态渐变值，0~1变化，用透明度动画控制切换

    buttonOpt.state |= QStyle::State_On;    // 绘制Checked时背景
    style()->drawControl(QStyle::CE_PushButtonBevel, &buttonOpt, &painter, this);


    painter.setOpacity(1.0); //滑块不透明
    QStyleOptionSlider sliderOpt;
    // sliderOpt.init(this);
    // Qt5和Qt6中init()成员变为initFrom()了，应改为如下语句 
    sliderOpt.initFrom(this);
    sliderOpt.subControls = QStyle::SC_SliderHandle;
    sliderOpt.activeSubControls = QStyle::SC_ScrollBarSlider;   // 这里需要使用ScrollBar
    sliderOpt.minimum = 0;
    sliderOpt.maximum = sliderOpt.rect.width(); // 直接使用像素范围
    int position = int(progress * (sliderOpt.rect.width()));   // 根据动态值控制滑块范围
    sliderOpt.sliderPosition = qMin(qMax(position, 0), sliderOpt.maximum);
    sliderOpt.sliderValue = sliderOpt.sliderPosition;

    sliderOpt.state |= QStyle::State_Off;
    sliderOpt.state &= ~QStyle::State_On;
    // 重设滑块区域，Qt源码会这么做，否则会绘制到整个按钮上
    sliderOpt.rect = style()->subControlRect(QStyle::CC_ScrollBar, &sliderOpt, QStyle::SC_ScrollBarSlider, this);
    style()->drawControl(QStyle::CE_ScrollBarSlider, &sliderOpt, &painter, this);   // 绘制滑块

    painter.setOpacity(progress);
    sliderOpt.state &= ~QStyle::State_Off;
    sliderOpt.state |= QStyle::State_On;
    style()->drawControl(QStyle::CE_ScrollBarSlider, &sliderOpt, &painter, this);   // 绘制滑块
}

void SwitchButton::onToggled(bool checked)
{
    QAbstractAnimation::Direction direction =
            checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward;
    bool pause = animation->state() == QAbstractAnimation::Running;
    if(pause)
        animation->pause();
    animation->setDirection(direction);
    if(pause)
        animation->resume();
    else
        animation->start(QAbstractAnimation::KeepWhenStopped);
    update();

}
