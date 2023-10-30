//
// Created by vlad on 10/29/23.
//

#pragma once
#include <optional>
#include <vector>

namespace apex_sdk
{
    class BaseEntity;
}

namespace hacks
{

    class Aimbot
    {
    public:
        enum class AimPosition : int
        {
            HEAD = 8,
            BODY = 5,
        };
        void Run();
        std::optional<apex_sdk::BaseEntity> GetTheBestTarget();
        static std::vector<apex_sdk::BaseEntity> GetAliveNotKnockedOutEnemies();
        float m_fSmooth = 8.f;
        float m_fFov = 5.f;
    };

} // hacks
