#ifndef TEXT_LABEL_H
#define TEXT_LABEL_H

#include <QFont>
#include <QLabel>
#include <QWidget>

/// @brief The TextLabel class is a custom QWidget that displays text with
/// customizable font and alignment.
class TextLabel : public QWidget
{
    Q_OBJECT

  private:
    /// @brief The QLabel used to display the text.
    QLabel* mLabel;

    /// @brief Private helper function to initialize the widget.
    void setup();

  public:
    /// @brief Constructor for the TextLabel class.
    /// @param text The initial text to display.
    /// @param font The initial font to use. Defaults to Arial - 14.
    /// @param alignment The initial alignment of the text. Defaults to
    /// AlignLeft.
    /// @param parent The parent widget. Default is nullptr.
    TextLabel(const QString& text, const QFont& font = QFont("Arial", 14),
              Qt::Alignment alignment = Qt::AlignLeft,
              QWidget* parent = nullptr);

    /// @brief Sets the text displayed by the label.
    /// @param text The new text to display.
    void setText(const QString& text);
    /// @brief Sets the font used to display the text.
    /// @param font The new font to use.
    void setFont(const QFont& font);
    /// @brief Sets the alignment of the text within the label.
    /// @param alignment The new alignment to use.
    void setAlignment(Qt::Alignment alignment);
};

#endif // TEXT_LABEL_H
