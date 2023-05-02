#ifndef MODEL_TESTER_H
#define MODEL_TESTER_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

#include "../TextLabel/TextLabel.h"

// TODO: docs
class ModelTester : public QDialog
{
    Q_OBJECT

  private:
    int mDuration;
    QTimer* mTimer;

    QLabel* mDurationText;
    QPushButton* mRecordButton;
    QPushButton* mSaveButton;

    QVBoxLayout* mMainLayout;

    void restartDialog();

  public:
    /// @brief Constructor for ModelTester class.
    /// @param parent A pointer to the QWidget parent. Defaults to nullptr.
    ModelTester(QWidget* parent = nullptr);

    int exec();

  public slots:
    void startRecording();
    void updateDuration();
    void stopRecording();
};

#endif // MODEL_TESTER_H
