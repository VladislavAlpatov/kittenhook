//
// Created by vlad on 10/29/23.
//

#include "memory_manager/MemoryManager.h"

#include <filesystem>
#include <ranges>
#include <algorithm>
#include <fstream>
#include <sys/uio.h>
#include <format>
#include <source_location>
#include <regex>

namespace memory_manager
{

    MemoryManager::MemoryManager(const std::string &procName)
    {
        const auto pid = GetProcessIdByName(procName);

        if (!pid)
            throw std::runtime_error(std::format("{}, Process with name '{}' not found!",
                                                 std::source_location::current().function_name(),
                                                 procName));

        m_procId = pid.value();
    }

    std::optional<pid_t> MemoryManager::GetProcessIdByName(const std::string& procName)
    {
        for (const auto& entry : std::filesystem::directory_iterator("/proc"))
        {
            const std::string path = entry.path();

            if (!std::ranges::all_of(std::ranges::drop_view(path,6),
                                     [](char chr) {return std::isdigit(chr);}))
                continue;

            std::ifstream statusFile(path + "/status");

            if (!statusFile.is_open())
                continue;

            std::string line;
            while (std::getline(statusFile, line))
            {
                if (!line.starts_with("Name:"))
                    continue;

                if (line.substr(6) == procName)
                    return std::stoi(path.substr(6));

                break;
            }
        }
        return std::nullopt;
    }

    std::vector<uint8_t> MemoryManager::ReadMemory(uintptr_t addr, size_t size) const
    {
        if (!size)
            return {};


        std::vector<uint8_t> buffer(size, '\0');

        iovec localMemoryRegion{.iov_base = buffer.data(), .iov_len = size };
        iovec remoteMemoryRegion{.iov_base = reinterpret_cast<void*>(addr), .iov_len = size };

        ssize_t readBytes = process_vm_readv(m_procId,
                                             &localMemoryRegion,
                                             1,
                                             &remoteMemoryRegion,
                                             1,
                                             0);

        return readBytes == static_cast<ssize_t>(size) ? buffer : std::vector<uint8_t>{};
    }

    std::optional<uintptr_t>
    MemoryManager::GetModuleBaseAddressByName([[maybe_unused]] const std::string &moduleName) const
    {
        std::fstream file(std::format("/proc/{}/maps", m_procId));

        if (!file.is_open())
            return std::nullopt;

        std::string line;

        while (std::getline(file, line))
        {
            if (!line.ends_with(moduleName))
                continue;

            const std::regex pattern(R"(\b([0-9a-fA-F]+)\b)");

            std::smatch match;

            if (!std::regex_search(line, match, pattern))
                return std::nullopt;

            std::string extractedNumber = match[1].str();

            return std::stoull(extractedNumber, nullptr,16);
        }

        return std::nullopt;
    }

    void MemoryManager::WriteMemory(void *buffer, uintptr_t addr, size_t size)
    {
        iovec localMemoryRegion{.iov_base = buffer, .iov_len = size };
        iovec remoteMemoryRegion{.iov_base = reinterpret_cast<void*>(addr), .iov_len = size };

        ssize_t writtenBytes = process_vm_writev(m_procId,
                                             &localMemoryRegion,
                                             1,
                                             &remoteMemoryRegion,
                                             1,
                                             0);

        if (writtenBytes != static_cast<ssize_t>(size))
            throw std::runtime_error(std::format("{}, Failed to write memory, "
                                                 "{} of {} bytes written!",
                                     std::source_location::current().function_name(),
                                     writtenBytes, size));

    }

} // memory_manager