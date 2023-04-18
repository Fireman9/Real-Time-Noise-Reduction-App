#include "MainWidget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent)
{
    // init class members
    mAppNameText = new TextLabel("Real-time\nNoise Reduction",
                                 QFont("Arial", 14), Qt::AlignCenter, this);
    mSelectMicText =
        new TextLabel("Microphone:", QFont("Arial", 12), Qt::AlignLeft, this);
    mMicNoiseToggleText = new TextLabel(
        "Noise Cancellation", QFont("Arial", 12), Qt::AlignVCenter, this);

    mMicDropDownList = new DropDownList(this);
    mMicNoiseToggleButton = new ToggleButton(this);
    logo = new Logo("C:/Users/Admin/Desktop/logo.png", this);

    mLayout = new QVBoxLayout(this);

    // add all available microphones to list
    addAllMicToList();

    // construct all widgets and layouts
    construct();

    // set window settings
    adjustWindowSettings();
}

void MainWidget::addAllMicToList()
{
    mMicDropDownList->addItem("Test1");
    mMicDropDownList->addItem("Test2");
    mMicDropDownList->addItem("Test3");
    mMicDropDownList->addItem("Test4");
    mMicDropDownList->addItem("Test5");
}

void MainWidget::construct()
{
    mLogoNameLayout = new QHBoxLayout();
    mLogoNameLayout->addWidget(logo);
    mLogoNameLayout->addWidget(mAppNameText);

    mMicDropDownLayout = new QVBoxLayout();
    mMicDropDownLayout->addWidget(mSelectMicText);
    mMicDropDownLayout->addWidget(mMicDropDownList);

    mMicNoiseToggleLayout = new QHBoxLayout();
    mMicNoiseToggleLayout->addWidget(mMicNoiseToggleText);
    mMicNoiseToggleLayout->addWidget(mMicNoiseToggleButton);

    mLayout->addLayout(mLogoNameLayout);
    mLayout->addLayout(mMicDropDownLayout);
    mLayout->addLayout(mMicNoiseToggleLayout);

    setLayout(mLayout);
}

void MainWidget::adjustWindowSettings()
{
    setWindowTitle("RTNR");
    setWindowIcon(QIcon("C:/Users/Admin/Desktop/logo.png"));
    // remove maximize button
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    this->setMinimumWidth(320);
    this->setMinimumHeight(180);
    // set to minimum possible size
    this->adjustSize();
    // set fixed size window
    this->setFixedSize(this->width(), this->height());
}
