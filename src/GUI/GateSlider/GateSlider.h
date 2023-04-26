#ifndef GATE_SLIDER_H
#define GATE_SLIDER_H

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

/// @brief A custom Qt widget class for noise gating. Includes a progress bar to
/// show the sound level, a slider and a spin box to set the threshold.
class GateSlider : public QWidget
{
    Q_OBJECT

  private:
    /// @brief The progress bar representing the current volume level.
    QProgressBar* mVolume;
    /// @brief The spin box allowing the user to set threshold.
    QSpinBox* mSpinBox;
    /// @brief The slider allowing the user to set threshold.
    QSlider* mSlider;
    /// @brief The label displaying the decibel (dB) unit.
    QLabel* mDBText;

    /// @brief The vertical layout containing the progress bar and the slider.
    QVBoxLayout* mVolumeSliderLayout;
    /// @brief The horizontal layout containing the spin box and the label.
    QHBoxLayout* mSpinLayout;
    /// @brief The main horizontal layout containing the volume slider and the
    /// spin box.
    QHBoxLayout* mLayout;

    /// @brief Constructs the GateSlider widget by setting up the UI elements
    /// and the layouts.
    void construct();

  public:
    /// @brief Constructor of the GateSlider class. Defines the parent widget.
    /// @param parent The parent widget parameter.
    GateSlider(QWidget* parent = nullptr);

    /// @brief Getter function to retrieve slider.
    /// @return Pointer to slider.
    QSlider* getSlider();

  private slots:
    /// @brief Private slot for connecting spin box value to slider.
    /// @param value Selected by slider value.
    void onSliderValueChanged(int value);
    /// @brief Private slot for connecting slider value to spin box.
    /// @param value Selected by spin box value.
    void onSpinBoxChanged(int value);
};

#endif // GATE_SLIDER_H
