#ifndef LOGO_H
#define LOGO_H

#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QWidget>

/// @brief A class for displaying a logo image as a QWidget.
class Logo : public QWidget
{
    Q_OBJECT

  private:
    /// @brief A pointer to the QLabel displaying the logo image.
    QLabel* mLogoLabel;
    /// @brief A pointer to the QVBoxLayout containing the QLabel.
    QVBoxLayout* layout;

  public:
    /// @brief Logo class constructor.
    /// @param filepath The filepath of the image to display.
    /// @param parent A pointer to the QWidget parent.
    Logo(QString filepath, QWidget* parent = nullptr);
};

#endif // LOGO_H
