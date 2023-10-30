//
// Created by vlad on 10/29/23.
//

#include "Aimbot.h"
#include <apex_sdk/BaseEntity.h>
#include <X11/Xlib.h>
#include <cmath>
#include <apex_sdk/EntityList.h>
#include <algorithm>
#include <ranges>

bool isRightButtonPressed() {
    static Display* display = nullptr;
    static Window root;
    Window root_return, child_return;
    int root_x_return, root_y_return, win_x_return, win_y_return;
    unsigned int mask_return;

    if (display == nullptr) {
        display = XOpenDisplay(NULL);
        if (display == NULL) {
            return false;
        }
        root = DefaultRootWindow(display);
    }

    if (XQueryPointer(display, root, &root_return, &child_return, &root_x_return, &root_y_return, &win_x_return, &win_y_return, &mask_return)) {
        if (mask_return & Button3Mask) { // Button3Mask indicates the right button
            return true;
        }
    }
    return false;
}


float RadToDeg(const float rads)
{
    return rads * 180.f / 3.1415926535f;
}

float DegToRad(const float degrees)
{
    return degrees * 3.1415926535f / 180.f;
}

uml::Vector3 CalcAimViewAngles(const uml::Vector3& origin, const uml::Vector3& target)
{
    uml::Vector3 out;
    const float distance = origin.DistTo(target);

    // Make x negative since -89 is top and 89 is bottom
    out.x = -RadToDeg(asinf((target.z - origin.z) / distance));
    out.y =  RadToDeg(atan2f(target.y - origin.y, target.x - origin.x));

    return out;
}

namespace hacks
{
    void Aimbot::Run()
    {
        auto target = GetTheBestTarget();
        auto local = apex_sdk::EntityList::GetLocalPlayer();

        if (!isRightButtonPressed())
            return;

        if (!target or !local)
            return;

        auto aimAngles = CalcAimViewAngles(local->GetCameraPosition(), target->GetBonePosition(5));
        aimAngles = local->GetViewAngles() + ((aimAngles - local->GetViewAngles()) / m_fSmooth);
        if ((aimAngles-local->GetViewAngles()).Length2D() <= m_fFov)
            local->SetViewAngles(aimAngles);
    }

    std::optional<apex_sdk::BaseEntity> Aimbot::GetTheBestTarget()
    {

        const auto localPlayer = apex_sdk::EntityList::GetLocalPlayer();

        const auto visible = GetAliveNotKnockedOutEnemies();

        if (visible.empty())
            return std::nullopt;


        return *std::ranges::min_element(visible,
        [&localPlayer](const apex_sdk::BaseEntity& first, const apex_sdk::BaseEntity& second) -> bool
        {
            const auto aimAnglesFirst = CalcAimViewAngles(localPlayer->GetCameraPosition(),
                                                          first.GetBonePosition(8));

            const auto aimAnglesSecond = CalcAimViewAngles(localPlayer->GetCameraPosition(),
                                                           second.GetBonePosition(8));

            const auto fDistFirst = localPlayer->GetViewAngles().DistTo(aimAnglesFirst);
            const auto fDistSecond = localPlayer->GetViewAngles().DistTo(aimAnglesSecond);

            return fDistFirst < fDistSecond;
        });

    }

    std::vector<apex_sdk::BaseEntity> Aimbot::GetAliveNotKnockedOutEnemies()
    {
        const auto localEnt = apex_sdk::EntityList::GetLocalPlayer();

        if (!localEnt)
            return {};

        static std::array<float, 100> oldTime;
        std::vector<apex_sdk::BaseEntity> ents;
        int i = 0;
        for (const apex_sdk::BaseEntity& ent : apex_sdk::EntityList::GetAllEntities())
        {

            auto newTime = ent.LastVisibleTime();
            auto& oldTimeF = oldTime[i++];
            const auto is_vis = newTime > oldTimeF or newTime < 0.f and oldTimeF > 0.f;
            oldTimeF = newTime;

            if (ent == localEnt or !ent.IsAlive() or ent.IsKnockedout() or ent.GetTeamNumber() == localEnt->GetTeamNumber() or !is_vis)
                continue;
            ents.push_back(ent);
        }
        return ents;
    }
} // hacks