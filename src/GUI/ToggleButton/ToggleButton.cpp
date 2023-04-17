#include "ToggleButton.h"

ToggleButton::ToggleButton(QWidget* parent, int width, int height,
                           QString bgColor, QString circleColor,
                           QString activeColor) :
    QCheckBox(parent),
    mBgColor(bgColor), mCircleColor(circleColor), mActiveColor(activeColor)
{
    setFixedSize(width, height);
    setCursor(Qt::PointingHandCursor);

    connect(this, &QCheckBox::stateChanged, this, &ToggleButton::debug);
}

void ToggleButton::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    QRect rect(0, 0, this->width(), this->height());

    if (!this->isChecked()) {
        painter.setBrush(QColor(mBgColor));
        painter.drawRoundedRect(0, 0, rect.width(), this->height(),
                                this->height() / 2, this->height() / 2);

        painter.setBrush(QColor(mCircleColor));
        painter.drawEllipse(3, 3, 22, 22);
    } else {
        painter.setBrush(QColor(mActiveColor));
        painter.drawRoundedRect(0, 0, rect.width(), this->height(),
                                this->height() / 2, this->height() / 2);

        painter.setBrush(QColor(mCircleColor));
        painter.drawEllipse(this->width() - 26, 3, 22, 22);
    }

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
