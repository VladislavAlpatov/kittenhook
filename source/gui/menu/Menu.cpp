//
// Created by vlad on 11/12/23.
//

#include "Menu.h"

#include <QApplication>
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QDoubleSpinBox>

#include "hacks/Aimbot.h"

namespace ktth::menu
{
    Menu::Menu(QWidget* parent) : QWidget(parent)
    {
        InitUserInterface();
        setWindowFlags(Qt::FramelessWindowHint);
    }

    void Menu::InitUserInterface()
    {
        auto layout = new QVBoxLayout(this);

        // Create a label
        auto label = new QLabel("Kitten hook", this);
        label->setAlignment(Qt::AlignHCenter);
        layout->addWidget(label);

        auto checkBox = new QCheckBox("AimBot", this);
        layout->addWidget(checkBox);

        InputFloat("Aimbot fov", &hacks::Aimbot::m_fFov, layout);
        InputFloat("Aimbot smooth", &hacks::Aimbot::m_fSmooth, layout);
        // Connect the checkbox's stateChanged signal
        connect(checkBox, &QCheckBox::stateChanged, [&](int state){
            qDebug() << "Checkbox state changed to:" << (state == Qt::Checked ? "Checked" : "Unchecked");
        });

        setLayout(layout);
    }

    void Menu::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() != Qt::LeftButton)
            return;

        m_canDrag = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();

        event->accept();
    }

    void Menu::mouseReleaseEvent(QMouseEvent* event)
    {
        m_canDrag = false;
    }

    void Menu::mouseMoveEvent(QMouseEvent* event)
    {
        if (m_canDrag && event->buttons() & Qt::LeftButton) {
            move(event->globalPosition().toPoint() - m_dragPosition);
            event->accept();
        }
    }

    void Menu::InputFloat(const std::string &str, float* val, QBoxLayout* layout)
    {
        auto hLayout = new QHBoxLayout(this);

        auto label = new QLabel(str.c_str());

        auto spinBox = new QDoubleSpinBox(this);
        spinBox->setValue(static_cast<double>(*val));

        layout->addLayout(hLayout);

        hLayout->addWidget(label);
        hLayout->addWidget(spinBox);

        connect(spinBox, &QDoubleSpinBox::valueChanged, this, [val](double newVal)
        {
            *val = static_cast<float>(newVal);
        });

    }
} // menu
// ktth