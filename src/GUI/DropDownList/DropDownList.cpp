#include "DropDownList.h"

DropDownList::DropDownList(QWidget* parent) : QComboBox(parent)
{
    setupStyle();

    // on drop-down list item change call debug function
    connect(this, &QComboBox::currentTextChanged, this, &DropDownList::debug);
}

void DropDownList::setupStyle()
{
    QString style = R"(
        QComboBox {
            border: 1px solid #ccc;
            border-radius: 4px;
            padding: 2px 2px 2px 8px;
            min-width: 6em;
            font-size: 18px;
            color: #555;
            background-color: #fff;
            selection-background-color: #0078d7;
            selection-color: #fff;
        }

        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 24px;
            border-left-width: 1px;
            border-left-color: #ccc;
            border-top-right-radius: 4px;
            border-bottom-right-radius: 4px;
            background-color: #f1f1f1;
        }
    )";

    this->setStyleSheet(style);
}

void DropDownList::debug()
{
    printf("Dropdown list item index selected: %u \n", currentIndex());
}
