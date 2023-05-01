#ifndef MODEL_TESTER_H
#define MODEL_TESTER_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

#include "../TextLabel/TextLabel.h"

// TODO: docs
class ModelTester : public QDialog
{
    Q_OBJECT

  private:
    QVBoxLayout* mMainLayout;

  public:
    /// @brief Constructor for ModelTester class.
    /// @param parent A pointer to the QWidget parent. Defaults to nullptr.
    ModelTester(QWidget* parent = nullptr);
};

#endif // MODEL_TESTER_H
