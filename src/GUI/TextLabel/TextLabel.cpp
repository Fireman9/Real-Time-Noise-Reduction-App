#include "TextLabel.h"

TextLabel::TextLabel(const QString& text, const QFont& font,
                     Qt::Alignment alignment, QWidget* parent) :
    QWidget(parent)
{
    // set text and parent widget
    mLabel = new QLabel(text, this);
    // set font and font size
    mLabel->setFont(font);
    // set alignment
    mLabel->setAlignment(alignment);
    // init setting setup
    setup();
}

void TextLabel::setup()
{
    // wrap words in qlabel
    mLabel->setWordWrap(true);
}

void TextLabel::setText(const QString& text)
{
    mLabel->setText(text);
}

void TextLabel::setFont(const QFont& font)
{
    mLabel->setFont(font);
}

void TextLabel::setAlignment(Qt::Alignment alignment)
{
    mLabel->setAlignment(alignment);
}
