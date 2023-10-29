//
// Created by vlad on 10/29/23.
//

#include "apex_sdk/BaseEntity.h"
#include "apex_sdk/MemoryManagerSinglton.h"
#include "apex_sdk/Offsets.h"

namespace apex_sdk
{
    int BaseEntity::GetHealth() const
    {
        return Memory::Get().ReadMemory<int>(m_pHandle+OFFSET_HEALTH).value();
    }

    int BaseEntity::GetSheildValue() const
    {
        return Memory::Get().ReadMemory<int>(m_pHandle+OFFSET_AMMO).value();
    }

    uml::Vector3 BaseEntity::GetOrigin() const
    {
        return Memory::Get().ReadMemory<uml::Vector3>(m_pHandle+OFFSET_ORIGIN).value();
    }

    float BaseEntity::LastVisibleTime() const
    {
        return 0;
    }

    uml::Vector3 BaseEntity::GetBonePosition(int iBone) const
    {
        struct Bone {
            uint8_t __pad1[0xCC];
            float x;
            uint8_t __pad2[0xC];
            float y;
            uint8_t __pad3[0xC];
            float z;
        };
        auto pBoneMatrix = Memory::Get().ReadMemory<uintptr_t>(m_pHandle + OFFSET_BONES).value();
        const auto bone =  Memory::Get().ReadMemory<Bone>(pBoneMatrix + iBone * 0x30);

        return GetOrigin() + uml::Vector3{bone->x, bone->y, bone->z};
    }

    int BaseEntity::GetTeamNumber() const
    {
        return Memory::Get().ReadMemory<int>(m_pHandle+OFFSET_TEAM).value();
    }

    bool BaseEntity::IsAlive() const
    {
        return !Memory::Get().ReadMemory<bool>(m_pHandle+OFFSET_LIFE_STATE).value();
    }

    bool BaseEntity::IsKnockedout() const
    {
        return Memory::Get().ReadMemory<bool>(m_pHandle + OFFSET_BLEED_OUT_STATE).value();
    }
}