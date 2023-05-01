#include "MainWidget.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent)
{
    // init class members
    mAppNameText = new TextLabel("Real-time\nNoise Reduction",
                                 QFont("Arial", 14), Qt::AlignCenter, this);
    mSelectMicText =
        new TextLabel("Microphone:", QFont("Arial", 12), Qt::AlignLeft, this);
    mMicNoiseToggleText = new TextLabel("Noise Reduction", QFont("Arial", 12),
                                        Qt::AlignVCenter, this);

    mMicDropDownList = new DropDownList(this);
    mMicNoiseToggleButton = new ToggleButton(this);
    mMicNoiseToggleButton->setDisabled(true);
    logo = new Logo("./images/logo.png", this);
    mMicIcon = new Icon("./images/mic.png", QSize(20, 20), this);
    mTestNRButton =
        new QLabel("<a href =\"whatever\">Test noise reduction</a>", this);

    mGateText =
        new TextLabel("Noise Gate:", QFont("Arial", 12), Qt::AlignLeft, this);
    mGateSlider = new GateSlider(this);

    mAudioChart = new AudioChart(this);

    mModelTester = new ModelTester(this);

    mLayout = new QVBoxLayout(this);

    mAudioStream = std::make_unique<AudioStream>();
    mCurMicIndex = 0;

    // Initialize system tray and its menu
    initSystemTray();

    // add all available microphones to list
    addAllMicToList();

    // construct all widgets and layouts
    construct();

    // set window settings
    adjustWindowSettings();

    // connect all signals and slots
    connectAll();
}

void MainWidget::addAllMicToList()
{
    mMicDropDownList->addItem("--Nothing selected--");
    for (std::string device : mAudioStream.get()->getAllInputDevices()) {
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
    mMicLayout->setAlignment(Qt::AlignVCenter);

    mMicDropDownLayout = new QVBoxLayout();
    mMicDropDownLayout->addLayout(mMicLayout);
    mMicDropDownLayout->addWidget(mMicDropDownList);

    mMicNoiseToggleLayout = new QHBoxLayout();
    mMicNoiseToggleLayout->addWidget(mMicNoiseToggleText);
    mMicNoiseToggleLayout->addWidget(mMicNoiseToggleButton);
    mMicNoiseToggleLayout->setAlignment(Qt::AlignVCenter);

    mTestNRLayout = new QHBoxLayout();
    mTestNRLayout->addWidget(mTestNRButton);
    mTestNRLayout->setAlignment(Qt::AlignRight);
    mTestNRLayout->setContentsMargins(0, 0, 0, 20);

    mNoiseGateLayout = new QVBoxLayout();
    mNoiseGateLayout->addWidget(mGateText);
    mNoiseGateLayout->addWidget(mGateSlider);
    mNoiseGateLayout->setContentsMargins(0, 0, 0, 20);

    mAudioChartLayout = new QVBoxLayout();
    mAudioChartLayout->addWidget(mAudioChart);

    mLayout->addLayout(mLogoNameLayout);
    mLayout->addLayout(mMicDropDownLayout);
    mLayout->addLayout(mMicNoiseToggleLayout);
    mLayout->addLayout(mTestNRLayout);
    mLayout->addLayout(mNoiseGateLayout);
    mLayout->addLayout(mAudioChartLayout);

    setLayout(mLayout);
}

void MainWidget::adjustWindowSettings()
{
    setWindowTitle("RTNR");
    setWindowIcon(QIcon("./images/logo.png"));
    // remove maximize button
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    // set sizes
    mMicDropDownList->setMaximumWidth(320);
    mAudioChart->setMaximumHeight(200);
    mGateText->setMinimumHeight(20);

    // set window size
    this->setMinimumWidth(320);
    this->setMinimumHeight(500);

    // set to minimum possible size
    this->adjustSize();
    // set fixed size window
    this->setFixedSize(this->width(), this->height());
}

void MainWidget::initSystemTray()
{
    mTrayIcon = new QSystemTrayIcon(this);

    mTrayIconMenu = new QMenu(this);

    // actions
    mExitAction = new QAction("Exit", this);

    // add actions to menu
    mTrayIconMenu->addAction(mExitAction);

    // set system tray icon
    mTrayIcon->setIcon(QIcon("./images/logo.png"));
    // set menu to tray
    mTrayIcon->setContextMenu(mTrayIconMenu);
}

void MainWidget::connectAll()
{
    // on drop down list item change - get microphone device system index
    connect(mMicDropDownList, &QComboBox::currentTextChanged, this,
            &MainWidget::getMicDeviceIndex);

    // on toggle button state change - start noise reduction
    connect(mMicNoiseToggleButton, &QCheckBox::stateChanged, this,
            &MainWidget::reduceNoise);

    // on label link click - open dialogue to test noise reduction
    connect(mTestNRButton, &QLabel::linkActivated, this,
            &MainWidget::startNRTest);

    // process tray icon activation
    connect(mTrayIcon, &QSystemTrayIcon::activated, this,
            &MainWidget::trayIconActivated);

    // on system tray menu exit option click - close app
    connect(mExitAction, &QAction::triggered, this, &MainWidget::onExitAction);

    // on slider change value - set new noise gate threshold
    connect(mGateSlider->getSlider(), &QSlider::valueChanged,
            mAudioStream.get()->mNoiseGate.get(), &NoiseGate::setThreshold);

    // set volume bar value on receipt of a sound signal
    connect(mAudioStream.get(), &AudioStream::tick, mGateSlider->getVolumeBar(),
            &QProgressBar::setValue);

    // update audio chart on receipt of a sound signal
    connect(mAudioStream.get(), &AudioStream::tickGated, mAudioChart,
            &AudioChart::appendData);
}

void MainWidget::getMicDeviceIndex()
{
    if (mMicDropDownList->currentIndex() != 0) {
        mCurMicIndex = mAudioStream.get()->getDeviceIdByName(
            mMicDropDownList->currentText().toStdString());
        printf("Current mic index: %d\n", mCurMicIndex);

        // enable toggle button
        mMicNoiseToggleButton->setEnabled(true);
    } else {
        printf("Current mic: nothing selected\n");

        // disable toggle button
        mMicNoiseToggleButton->setEnabled(false);
    }
}

void MainWidget::reduceNoise()
{
    // if the toggle button that enables/disables noise cancellation is checked
    if (mMicNoiseToggleButton->isChecked()) {
        // disable drop-down list
        mMicDropDownList->setDisabled(true);

        // TODO audio driver setting
        // open stream to selected microphone
        mAudioStream.get()->openStream(5);
        // mAudioStream.get()->openStream(mCurMicIndex);
    } else {
        // enable drop-down list
        mMicDropDownList->setDisabled(false);

        mAudioStream.get()->closeStream();
        // set volume leveler value to zero
        mGateSlider->getVolumeBar()->setValue(-100);
    }
}

bool MainWidget::event(QEvent* e)
{
    if (e->type() == QEvent::WindowStateChange) {
        if (isMinimized()) {
            // hide main widget
            this->hide();
            // show tray icon
            mTrayIcon->show();
            e->ignore();
        } else {
            e->accept();
        }
    }
    return QWidget::event(e);
}

void MainWidget::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    // if tray icon was triggered
    if (reason == QSystemTrayIcon::Trigger) {
        // show main widget
        this->show();
        // bring window to foreground
        this->setWindowState(Qt::WindowState::WindowActive);
    }
    // if tray icon menu was triggered(right mouse click)
    else if (reason == QSystemTrayIcon::Context) {
        mTrayIconMenu->exec(QCursor::pos());
    }
}

void MainWidget::onExitAction()
{
    QApplication::quit();
}

void MainWidget::startNRTest()
{
    mModelTester->exec();
}
