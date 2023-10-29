//
// Created by vlad on 10/29/23.
//

#pragma once
#include <string>
#include <optional>
#include <vector>
#include <cstdint>
#include <any>

namespace memory_manager
{

    class MemoryManager
    {
    public:
        explicit MemoryManager(const std::string& procName);

        [[nodiscard]]
        static std::optional<pid_t> GetProcessIdByName(const std::string& procName);

        [[nodiscard]]
        std::optional<uintptr_t> GetModuleBaseAddressByName(const std::string& moduleName) const;

        [[nodiscard]]
        std::vector<uint8_t> ReadMemory(uintptr_t addr, size_t size) const;

        void WriteMemory(void* buffer, uintptr_t addr, size_t size);

        template<class type>
        void WriteMemory(uintptr_t addr, const type val)
        {
            WriteMemory((void*)&val, addr, sizeof(val));
        }


        template<class type>
        std::optional<type> ReadMemory(uintptr_t addr) const
        {
            return *reinterpret_cast<type*>(ReadMemory(addr, sizeof(type)).data());
        }

    private:
        pid_t m_procId;
    };
} // memory_manager
