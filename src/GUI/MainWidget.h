#ifndef MAIN_WIDGET_H
#define MAIN_WIDGET_H

#include <QAction>
#include <QApplication>
#include <QEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QWidget>

#include "../Stream/AudioStream.h"
#include "DropDownList/DropDownList.h"
#include "GateSlider/GateSlider.h"
#include "Icon/Icon.h"
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
    /// @brief The microphone icon.
    Icon* mMicIcon;
    /// @brief The drop-down list that contains the available microphones.
    DropDownList* mMicDropDownList;
    /// @brief The toggle button that enables/disables noise cancellation.
    ToggleButton* mMicNoiseToggleButton;

    TextLabel* mGateText;
    GateSlider* mGateSlider;

    /// @brief The main vertical layout of the widget.
    QVBoxLayout* mLayout;
    /// @brief The horizontal layout that contains the logo and the application
    /// name.
    QHBoxLayout* mLogoNameLayout;
    /// @brief The horizontal layout that contains the mic icon and the
    /// "Microphone:" label
    QHBoxLayout* mMicLayout;
    /// @brief The vertical layout that contains mic icon, "Microphone:" label
    /// and the drop-down list.
    QVBoxLayout* mMicDropDownLayout;
    /// @brief The horizontal layout that contains the "Noise Cancellation"
    /// label and the toggle button.
    QHBoxLayout* mMicNoiseToggleLayout;

    QVBoxLayout* mNoiseGateLayout;

    /// @brief Audio stream manager class.
    AudioStream mAudioStream;
    /// @brief Current microphone index selected for noise reduction
    int mCurMicIndex;

    /// @brief System tray icon class for minimizing to tray.
    QSystemTrayIcon* mTrayIcon;
    /// @brief Menu for system tray.
    QMenu* mTrayIconMenu;
    /// @brief Exit action for system tray menu.
    QAction* mExitAction;

    /// @brief Adds all available microphones to the drop-down list.
    void addAllMicToList();
    /// @brief Constructs the widgets and layouts.
    void construct();
    /// @brief Adjusts the window settings.
    void adjustWindowSettings();
    /// @brief Initialize system tray and its menu.
    void initSystemTray();
    /// @brief Connect all signals and slots here.
    void connectAll();

  protected:
    virtual bool event(QEvent* e) override;

  public:
    /// @brief Constructs a new MainWidget object.
    /// @param parent The parent widget.
    MainWidget(QWidget* parent = nullptr);

  public slots:
    /// @brief Slot function for retrieving microphone device system index after
    /// dropdown list of available microphones item change
    void getMicDeviceIndex();
    /// @brief Slot function to start noise reduction on the selected
    /// microphone after toggle button check.
    void reduceNoise();
    /// @brief Slot function to restore window on tray icon click.
    /// @param reason Specifies the reason for activation.
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    /// @brief Slot function to exit app on tray menu exit option click.
    void onExitAction();
};

#endif // MAIN_WIDGET_H
