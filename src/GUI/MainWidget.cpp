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
    logo = new Logo("./images/logo.png", this);
    mMicIcon = new Icon("./images/mic.png", QSize(20, 20), this);

    mLayout = new QVBoxLayout(this);

    mCurMicIndex = 0;

    // add all available microphones to list
    addAllMicToList();

    // construct all widgets and layouts
    construct();

    // set window settings
    adjustWindowSettings();

    // on drop down list item change - get microphone device system index
    connect(mMicDropDownList, &QComboBox::currentTextChanged, this,
            &MainWidget::getMicDeviceIndex);

    // on toggle button state change - start noise reduction
    connect(mMicNoiseToggleButton, &QCheckBox::stateChanged, this,
            &MainWidget::reduceNoise);
}

void MainWidget::addAllMicToList()
{
    mMicDropDownList->addItem("--Nothing selected--");
    for (std::string device : mAudioStream.getAllInputDevices()) {
        mMicDropDownList->addItem(QString::fromStdString(device));
    }
}

void MainWidget::construct()
{
    mLogoNameLayout = new QHBoxLayout();
    mLogoNameLayout->addWidget(logo);
    mLogoNameLayout->addWidget(mAppNameText);

    mMicLayout = new QHBoxLayout();
    mMicLayout->addWidget(mMicIcon);
    mMicLayout->addWidget(mSelectMicText);

    mMicDropDownLayout = new QVBoxLayout();
    mMicDropDownLayout->addLayout(mMicLayout);
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

    // set sizes
    mMicDropDownList->setMaximumWidth(320);
    this->setMinimumWidth(320);
    this->setMinimumHeight(180);

    // set to minimum possible size
    this->adjustSize();
    // set fixed size window
    this->setFixedSize(this->width(), this->height());
}

void MainWidget::getMicDeviceIndex()
{
    if (mMicDropDownList->currentIndex() != 0) {
        mCurMicIndex = mAudioStream.get_device_id_by_name(
            mMicDropDownList->currentText().toStdString());
        printf("Current mic index: %d\n", mCurMicIndex);
    } else {
        printf("Current mic: nothing selected\n");
    }
}

void MainWidget::reduceNoise()
{
    // if the toggle button that enables/disables noise cancellation is checked
    if (mMicNoiseToggleButton->isChecked()) {
        // open stream to selected microphone
        // TODO audio driver setting
        mAudioStream.open_stream(5);
        // mAudioStream.open_stream(mCurMicIndex);
    } else {
        mAudioStream.close_stream();
    }
}
