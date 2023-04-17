#ifndef TOGGLE_BUTTON_H
#define TOGGLE_BUTTON_H

#include <QCheckBox>
#include <QPainter>
#include <QPropertyAnimation>
#include <QString>

/// @brief Custom QCheckBox with a toggle switch-like appearance. This class
/// inherits from QCheckBox and provides a custom look for the checkbox. The
/// checkbox is displayed as a circle that moves to the left or right within a
/// rectangular background to indicate the on or off state. The background,
/// circle, and active circle colors can be customized.
class ToggleButton : public QCheckBox
{
    Q_OBJECT

  private:
    /// @brief Background color of the toggle button.
    QString mBgColor;
    /// @brief Color of the circle inside the toggle button.
    QString mCircleColor;
    /// @brief Color of the circle when the toggle button is active.
    QString mActiveColor;

    /// @brief Helper status function for debugging.
    void debug();

  public:
    /// @brief Constructor for ToggleButton.
    /// @param parent The parent widget.
    /// @param width The width of the toggle button.
    /// @param height The height of the toggle button.
    /// @param bgColor The background color of the toggle button.
    /// @param circleColor The color of the circle inside the toggle button.
    /// @param activeColor The color of the circle when the toggle button is
    /// active.
    ToggleButton(QWidget* parent = nullptr, int width = 60, int height = 28,
                 QString bgColor = "#777", QString circleColor = "#DDD",
                 QString activeColor = "#00A300");

  protected:
    /// @brief Overridden paint event.This function is called when the toggle
    /// button needs to be repainted. It draws the background and the circle
    /// inside the toggle button.
    /// @param event The paint event.
    virtual void paintEvent(QPaintEvent* event) override;

    /// @brief Overridden hit button function. This function is called when the
    /// user clicks on the toggle button. It checks if the click was inside the
    /// circle or not, and toggles the state accordingly.
    /// @param pos The position of the click.
    /// @return Whether the click was inside the circle or not.
    virtual bool hitButton(const QPoint& pos) const override;
};

#endif // TOGGLE_BUTTON_H
