#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "DropDownList/DropDownList.h"
#include "Logo/Logo.h"
#include "TextLabel/TextLabel.h"
#include "ToggleButton/ToggleButton.h"

/// @brief The MainWidget class represents the main GUI widget for the Real-time
/// Noise Reduction application.
class MainWidget : public QWidget
{
    Q_OBJECT

  private:
    /// @brief The label that displays the application name.
    TextLabel* mAppNameText;
    /// @brief The label that displays the "Microphone:" text before microphone
    /// selection drop-down list.
    TextLabel* mSelectMicText;
    /// @brief The label that displays the "Noise Cancellation" text before
    /// toggle button.
    TextLabel* mMicNoiseToggleText;

    /// @brief The logo displayed in the widget.
    Logo* logo;
    /// @brief The drop-down list that contains the available microphones.
    DropDownList* mMicDropDownList;
    /// @brief The toggle button that enables/disables noise cancellation.
    ToggleButton* mMicNoiseToggleButton;

    /// @brief The main vertical layout of the widget.
    QVBoxLayout* mLayout;
    /// @brief The horizontal layout that contains the logo and the application
    /// name.
    QHBoxLayout* mLogoNameLayout;
    /// @brief The vertical layout that contains the "Microphone:" label and the
    /// drop-down list.
    QVBoxLayout* mMicDropDownLayout;
    /// @brief The horizontal layout that contains the "Noise Cancellation"
    /// label and the toggle button.
    QHBoxLayout* mMicNoiseToggleLayout;

    /// @brief Adds all available microphones to the drop-down list.
    void addAllMicToList();
    /// @brief Constructs the widgets and layouts.
    void construct();
    /// @brief Adjusts the window settings.
    void adjustWindowSettings();

  public:
    /// @brief Constructs a new MainWidget object.
    /// @param parent The parent widget.
    MainWidget(QWidget* parent = nullptr);
};

#endif // MAIN_WIDGET_H
