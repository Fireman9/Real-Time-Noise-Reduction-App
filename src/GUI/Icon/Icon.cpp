#include "Icon.h"

Icon::Icon(const QString& filePath, const QSize& size, QWidget* parent) :
    QLabel(parent)
{
    // scale image to required size
    mImage = QPixmap(filePath).scaled(size, Qt::KeepAspectRatio,
                                      Qt::SmoothTransformation);
    // set image to qlabel
    setPixmap(mImage);
    setFixedSize(size);
}
