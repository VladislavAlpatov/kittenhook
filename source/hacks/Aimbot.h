//
// Created by vlad on 10/29/23.
//

#pragma once
#include <optional>
#include <vector>
#include "uml/Vector3.h"

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
        static float m_fSmooth;
        static float m_fFov;
        static bool m_bEnabled;
    private:
        uml::Vector3 oldBreathAngle;
    };

} // hacks
