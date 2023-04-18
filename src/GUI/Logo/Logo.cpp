#include "Logo.h"

Logo::Logo(QString filepath, QWidget* parent) : QWidget(parent)
{
    mLogoLabel = new QLabel(this);
    layout = new QVBoxLayout(this);

    QPixmap logoPixmap(filepath);
    mLogoLabel->setPixmap(logoPixmap);
    mLogoLabel->setScaledContents(true);

    // make resizable
    layout->addWidget(mLogoLabel);
    mLogoLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    mLogoLabel->setMinimumSize({60, 40});
    mLogoLabel->setMaximumSize({200, 150});
}
