//
// Created by vlad on 11/12/23.
//

#include "Menu.h"
#include <QApplication>
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QDebug>
#include <QPainter>
#include <QVBoxLayout>
#include <QMouseEvent>

namespace ktth::menu
{
    Menu::Menu(QWidget* parent) : QWidget(parent)
    {
        InitUserInterface();
        setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    }

    void Menu::InitUserInterface()
    {
        auto *layout = new QVBoxLayout(this);

        // Create a label
        auto *label = new QLabel("Kitten hook", this);
        layout->addWidget(label);

        // Create a checkbox
        auto *checkBox = new QCheckBox("AimBot", this);
        layout->addWidget(checkBox);

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
} // menu
// ktth