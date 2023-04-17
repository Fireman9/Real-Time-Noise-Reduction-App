#include "ToggleButton.h"

ToggleButton::ToggleButton(QWidget* parent, int width, int height,
                           QString bgColor, QString circleColor,
                           QString activeColor) :
    QCheckBox(parent),
    mBgColor(bgColor), mCircleColor(circleColor), mActiveColor(activeColor)
{
    // set size
    setFixedSize(width, height);
    // set cursor on hover event
    setCursor(Qt::PointingHandCursor);

    // run debug after button state change
    connect(this, &QCheckBox::stateChanged, this, &ToggleButton::debug);
}

void ToggleButton::paintEvent(QPaintEvent* event)
{
    // adjucst painter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    // rectangle init
    QRect rect(0, 0, this->width(), this->height());

    // check if checked
    if (!this->isChecked()) {
        // draw background
        painter.setBrush(QColor(mBgColor));
        painter.drawRoundedRect(0, 0, rect.width(), this->height(),
                                this->height() / 2, this->height() / 2);

        // draw circle
        painter.setBrush(QColor(mCircleColor));
        painter.drawEllipse(3, 3, 22, 22);
    } else {
        // draw background
        painter.setBrush(QColor(mActiveColor));
        painter.drawRoundedRect(0, 0, rect.width(), this->height(),
                                this->height() / 2, this->height() / 2);

        // draw circle
        painter.setBrush(QColor(mCircleColor));
        painter.drawEllipse(this->width() - 26, 3, 22, 22);
    }

    // paint end
    painter.end();
}

bool ToggleButton::hitButton(const QPoint& pos) const
{
    return this->contentsRect().contains(pos);
}

void ToggleButton::debug()
{
    printf("Status: %d \n", isChecked());
}
