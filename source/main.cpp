#include <iostream>
#include "memory_manager/MemoryManager.h"


int main()
{
    // 0x140000000;

    auto memManager = memory_manager::MemoryManager("r5apex.exe");

    auto addr = memManager.ReadMemory<uintptr_t>(0x140000000+0x21d1758).value()+(0x24f4 - 0x14);
    std::cout << "Pitch: " << memManager.ReadMemory<float>(addr).value();
    memManager.WriteMemory<float>(addr,0.f);

    return 0;
}
