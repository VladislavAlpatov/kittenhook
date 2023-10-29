//
// Created by vlad on 10/29/23.
//

#pragma once


#include <cstdint>
#include <optional>
#include <uml/Vector3.h>

namespace apex_sdk
{
    class BaseEntity
    {
    public:
        explicit BaseEntity(uintptr_t addr) {m_pHandle = addr;}

        [[nodiscard]] int GetHealth() const;

        [[nodiscard]] int GetSheildValue() const;

        // [[nodiscard]] std::optional<Weapon> GetActiveWeapon() const;

        [[nodiscard]] uml::Vector3 GetOrigin() const;

        [[nodiscard]] int GetTeamNumber() const;

        [[nodiscard]] float LastVisibleTime() const;

        [[nodiscard]] bool IsAlive() const;

        [[nodiscard]] bool IsKnockedout() const;

        [[nodiscard]] uml::Vector3 GetBonePosition(int iBone) const;
        [[nodiscard]] uml::Vector3 GetCameraPosition() const;
        [[nodiscard]] uml::Vector3 GetViewAngles() const;
        [[nodiscard]] uml::Vector3 GetVel() const;
        [[nodiscard]] uml::Vector3 GetBreathAngle() const;
        [[nodiscard]] uml::Vector3 GetAimPunch() const;

        int GetFlags() const;

    private:
        uintptr_t m_pHandle = 0;
    };
}