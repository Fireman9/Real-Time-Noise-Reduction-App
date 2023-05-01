#ifndef AUDIO_CHART_H
#define AUDIO_CHART_H

#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QtCharts>

/// @brief AudioChart class is a custom widget that displays real-time audio
/// data as a line chart.
class AudioChart : public QChartView
{
    Q_OBJECT

  private:
    /// @brief Pointer to the chart view object.
    QChartView* mChartView;
    /// @brief Pointer to the line series object.
    QLineSeries* mSeries;
    /// @brief Pointer to the X-axis object.
    QValueAxis* mXAxis;
    /// @brief Pointer to the Y-axis object.
    QValueAxis* mYAxis;
    /// @brief Pointer to the chart object.
    QChart* mChart;

    /// @brief A vector that stores the audio data received.
    std::vector<float> mAudioBuffer;

    /// @brief Private method to set up the chart and axes.
    void setupChart();

  public:
    /// @brief Constructor for AudioChart class.
    /// @param parent A pointer to the QWidget parent. Defaults to nullptr.
    AudioChart(QWidget* parent = nullptr);

  public slots:
    /// @brief Slot function to append new audio data to the chart.
    /// @param value New audio data value to append.
    void appendData(float value);
};

#endif // AUDIO_CHART_H
