#include "GateSlider.h"

GateSlider::GateSlider(QWidget* parent) : QWidget(parent)
{
    mVolume = new QProgressBar();
    mSlider = new QSlider();
    mSpinBox = new QSpinBox();
    mDBText = new QLabel("dB");

    // setup volume leveler
    mVolume->setMinimum(-100);
    mVolume->setMaximum(0);
    mVolume->setValue(-100);
    mVolume->setTextVisible(false);

    // setup slider
    mSlider->setOrientation(Qt::Horizontal);
    mSlider->setMinimum(-100);
    mSlider->setMaximum(0);
    mSlider->setValue(-100);
    mSlider->setStyleSheet(R"(
        QSlider::handle {
	        background: gray;
	        border: 1px solid black;
        })");

    // setup slider value spin box
    mSpinBox->setMinimum(-100);
    mSpinBox->setMaximum(0);
    mSpinBox->setValue(-100);
    mSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    construct();

    // Connect the slider and text input field
    connect(mSlider, &QSlider::valueChanged, this,
            &GateSlider::onSliderValueChanged);
    connect(mSpinBox, &QSpinBox::valueChanged, this,
            &GateSlider::onSpinBoxChanged);
}

void GateSlider::construct()
{
    mVolumeSliderLayout = new QVBoxLayout();
    mSpinLayout = new QHBoxLayout();
    mLayout = new QHBoxLayout();

    mVolumeSliderLayout->addWidget(mVolume);
    mVolumeSliderLayout->addWidget(mSlider);
    // adjust margins...
    mVolumeSliderLayout->setSpacing(0);
    mVolumeSliderLayout->setContentsMargins(0, 0, 0, 0);

    mSpinLayout->addWidget(mSpinBox);
    mSpinLayout->addWidget(mDBText);
    // adjust margins...
    mSpinLayout->setSpacing(0);
    mSpinLayout->setContentsMargins(0, 0, 0, 0);

    mLayout->addLayout(mVolumeSliderLayout);
    mLayout->addLayout(mSpinLayout);
    // adjust margins...
    mLayout->setSpacing(6);
    mLayout->setContentsMargins(0, 0, 0, 0);
    mLayout->setAlignment(Qt::AlignCenter);

    setLayout(mLayout);
}

void GateSlider::onSliderValueChanged(int value)
{
    mSpinBox->setValue(value);
}

void GateSlider::onSpinBoxChanged(int value)
{
    mSlider->setValue(value);
}

QSlider* GateSlider::getSlider()
{
    return mSlider;
}

QProgressBar* GateSlider::getProgressBar()
{
    return mVolume;
}
