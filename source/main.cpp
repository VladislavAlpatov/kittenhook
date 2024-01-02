#include <QApplication>
#include <thread>

#include "gui/overlay/Overlay.h"
#include "gui/menu/Menu.h"
#include "hacks/Aimbot.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    std::thread([]{ktth::overlay::Overlay().Run();}).detach();
    std::thread([]
    {
        auto x = hacks::Aimbot();

        while (true)
            x.Run();
    }).detach();

    auto menu = ktth::menu::Menu();
    menu.show();

    app.exec();
}
