#include "AudioChart.h"

AudioChart::AudioChart(QWidget* parent) :
    QChartView(parent), mChartView(new QChartView(this)),
    mSeries(new QLineSeries(this)), mXAxis(new QValueAxis(this)),
    mYAxis(new QValueAxis(this)), mChart(new QChart()), mAudioBuffer(32, 0)
{
    // Set up chart and axes
    setupChart();
}

void AudioChart::setupChart()
{
    // Set up chart
    mChart->setTitle("Real-time audio chart");
    mChart->legend()->hide();
    mChart->setAnimationOptions(QChart::NoAnimation);
    mChart->setTheme(QChart::ChartTheme::ChartThemeLight);
    // set values
    mChart->addSeries(mSeries);

    // Set up X-axis
    mXAxis->setRange(0, 32);
    mXAxis->setLabelFormat("%g");
    mXAxis->setLabelsVisible(false);

    // Set up Y-axis
    mYAxis->setRange(0, 1);
    mYAxis->setLabelFormat("%g");

    // Add axes to chart
    mChart->addAxis(mXAxis, Qt::AlignBottom);
    mChart->addAxis(mYAxis, Qt::AlignLeft);
    mSeries->attachAxis(mXAxis);
    mSeries->attachAxis(mYAxis);

    // Set chart as view's chart
    mChartView->setChart(mChart);

    // adjust chart size
    mChart->setMinimumWidth(320);
    mChart->setMinimumHeight(200);
}

void AudioChart::appendData(float value)
{
    // Append new audio data to buffer
    mAudioBuffer.push_back(value);

    // Remove oldest data
    mAudioBuffer.erase(mAudioBuffer.begin());

    // Update chart data
    mSeries->clear();
    for (int i = 0; i < mAudioBuffer.size(); ++i) {
        mSeries->append(i, mAudioBuffer[i]);
    }

    // update chart view
    mChartView->update();
}
