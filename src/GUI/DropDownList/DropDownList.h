#ifndef DROP_DOWN_LIST_H
#define DROP_DOWN_LIST_H

#include <QComboBox>
#include <QPalette>
#include <QString>
#include <QWidget>

/// @brief A custom QComboBox with simple style.
class DropDownList : public QComboBox
{
    Q_OBJECT

  private:
    /// @brief Sets up the style of the DropDownList.
    void setupStyle();
    /// @brief Outputs current item index after item change event.
    void debug();

  public:
    /// @brief Constructs a DropDownList object.
    /// @param parent QWidget parent. Defaults to nullptr.
    DropDownList(QWidget* parent = nullptr);
};

#endif // DROP_DOWN_LIST_H
