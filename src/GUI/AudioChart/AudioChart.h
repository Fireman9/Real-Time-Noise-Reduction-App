#ifndef AUDIO_CHART_H
#define AUDIO_CHART_H

#include <QChartView>
#include <QLineSeries>
#include <QValueAxis>
#include <QtCharts>

class AudioChart : public QChartView
{
    Q_OBJECT

  private:
    QChartView* mChartView;
    QLineSeries* mSeries;
    QValueAxis* mXAxis;
    QValueAxis* mYAxis;
    QChart* mChart;

    std::vector<float> mAudioBuffer;

    void setupChart();

  public:
    AudioChart(QWidget* parent = nullptr);

  public slots:
    void appendData(float value);
};

#endif // AUDIO_CHART_H