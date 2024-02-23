#include <QApplication>
#include <thread>
#include <iostream>
#include "gui/overlay/Overlay.h"
#include "gui/menu/Menu.h"
#include "hacks/Aimbot.h"
#include "apex_sdk/MemoryManagerSinglton.h"
#include <uml/color.h>
#include <apex_sdk/Offsets.h>


int main(int argc, char *argv[])
{
    auto& manager = apex_sdk::Memory::Get();
    const auto val = manager.PatternScan("r5apex.exe", apex_sdk::signatures::m_bleedOutState.data());
    setbuf(stdout, nullptr);
    std::cout << "Pattern scan result: 0x" << std::hex << val.value_or(0);
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

    return QApplication::exec();
}
