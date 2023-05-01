#include "ModelTester.h"

ModelTester::ModelTester(QWidget* parent) : QDialog(parent)
{
    mMainLayout = new QVBoxLayout(this);
    setLayout(mMainLayout);

    setFixedSize(300, 100);
    setWindowTitle("Test Noise Reduction:");
}
