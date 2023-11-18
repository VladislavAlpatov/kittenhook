//
// Created by vlad on 11/12/23.
//

#pragma once
#include <QWidget>

class QMouseEvent;
class QBoxLayout;


namespace ktth::menu
{
    class Menu final : public QWidget
    {
    public:
        explicit Menu(QWidget *parent = nullptr);
    private:
        void InitUserInterface();

    protected:
        void mousePressEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;

    private:
        bool m_canDrag = false;
        QPoint m_dragPosition;

        void InputFloat(const std::string& str, float* val, QBoxLayout* layout = nullptr);
    };
} // ktth
