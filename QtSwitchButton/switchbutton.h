#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QPushButton>

class QVariantAnimation;

class SwitchButton : public QPushButton
{
    Q_OBJECT
public:
    explicit SwitchButton(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *);

private:
    void onToggled(bool checked);
private:
    qreal progress = 0;
    QVariantAnimation *animation = nullptr;
};

#endif // SWITCHBUTTON_H
