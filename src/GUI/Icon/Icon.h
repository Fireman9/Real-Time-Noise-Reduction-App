#ifndef ICON_H
#define ICON_H

#include <QLabel>
#include <QPixmap>

/// @brief The Icon class is a QLabel subclass that displays an image using a
/// given file path and size.
class Icon : public QLabel
{
    Q_OBJECT

  private:
    /// @brief The QPixmap that holds the image data.
    QPixmap mImage;

  public:
    /// @brief Constructs an Icon object with the specified file path and size.
    /// @param filePath The path of the image file to be displayed.
    /// @param size The size to which the image should be resized. Default is
    /// QSize(40, 40).
    /// @param parent The parent widget of the Icon. Default is nullptr.
    Icon(const QString& filePath, const QSize& size = QSize(40, 40),
         QWidget* parent = nullptr);
};

#endif // ICON_H
