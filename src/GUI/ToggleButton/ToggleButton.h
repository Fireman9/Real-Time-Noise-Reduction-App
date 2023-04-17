#ifndef TOGGLE_BUTTON_H
#define TOGGLE_BUTTON_H

#include <QCheckBox>
#include <QPainter>
#include <QPropertyAnimation>
#include <QString>

class ToggleButton : public QCheckBox
{
    Q_OBJECT

  private:
    QString mBgColor;
    QString mCircleColor;
    QString mActiveColor;

    void debug();

  public:
    ToggleButton(QWidget* parent = nullptr, int width = 60, int height = 28,
                 QString bgColor = "#777", QString circleColor = "#DDD",
                 QString activeColor = "#00A300");

  protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool hitButton(const QPoint& pos) const override;
};

#endif // TOGGLE_BUTTON_H
