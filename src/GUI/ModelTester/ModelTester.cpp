#include "ModelTester.h"

ModelTester::ModelTester(QWidget* parent) : QDialog(parent)
{

    mDurationText = new QLabel(this);
    mTimer = new QTimer(this);
    mRecordButton = new QPushButton("Start recording", this);
    mSaveButton = new QPushButton("Save recording", this);
    mMainLayout = new QVBoxLayout(this);

    mMainLayout->addWidget(mDurationText);
    mMainLayout->addWidget(mRecordButton);
    mMainLayout->addWidget(mSaveButton);

    restartDialog();

    setLayout(mMainLayout);
    setFixedSize(300, 100);
    setWindowTitle("Test Noise Reduction:");

    connect(mRecordButton, &QPushButton::clicked, this,
            &ModelTester::startRecording);
    connect(mSaveButton, &QPushButton::clicked, this,
            &ModelTester::stopRecording);
    connect(mTimer, &QTimer::timeout, this, &ModelTester::updateDuration);
}

int ModelTester::exec()
{
    restartDialog();
    return QDialog::exec();
}

void ModelTester::restartDialog()
{
    mDuration = 0;
    mDurationText->setText("Duration: 0 s");
    mDurationText->hide();
    mSaveButton->hide();
    mRecordButton->show();
}

void ModelTester::startRecording()
{
    mDuration = 0;
    mRecordButton->hide();
    mDurationText->show();
    mSaveButton->show();

    mTimer->start(1000);
}

void ModelTester::stopRecording()
{
    // mDurationText->hide();
    mSaveButton->hide();

    mTimer->stop();
}

void ModelTester::updateDuration()
{
    mDuration++;
    mDurationText->setText(QString("Duration: %1 s").arg(mDuration));
}
