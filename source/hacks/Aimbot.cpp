//
// Created by vlad on 10/29/23.
//

#include "Aimbot.h"
#include <apex_sdk/BaseEntity.h>
#include <X11/Xlib.h>
#include <apex_sdk/EntityList.h>
#include <algorithm>
#include <ranges>
#include <chrono>
#include <apex_sdk/ProjectilePrediction.h>

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
        if (mask_return & Button1Mask) { // Button3Mask indicates the right button
            return true;
        }
    }
    return false;
}


namespace hacks
{
    float Aimbot::m_fFov = 5.f;
    float Aimbot::m_fSmooth = 1.f;
    bool Aimbot::m_bEnabled = false;
    void Aimbot::Run()
    {
        const auto start = std::chrono::high_resolution_clock::now();

        auto target = GetTheBestTarget();
        auto local = apex_sdk::EntityList::GetLocalPlayer();

        if (!isRightButtonPressed() || !m_bEnabled)
            return;

        if (!target or !local)
            return;

        auto aimAngles = apex_sdk::ProjectilePrediction::CalculateViewAngles(*local, *target);

        if (!aimAngles or local->GetViewAngles().DistTo(*aimAngles) > m_fFov)
            return;

        const auto delta = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.f;
        if (m_fSmooth < 1.f)
            aimAngles = local->GetViewAngles() + ((*aimAngles - local->GetViewAngles()) * (m_fSmooth * delta));
        else
            *aimAngles -= local->GetBreathAngle() - local->GetViewAngles();

        local->SetViewAngles(*aimAngles);
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
            const auto aimAnglesFirst = localPlayer->GetCameraPosition().ViewAngleTo(
                                                          first.GetBonePosition(8));

            const auto aimAnglesSecond =  localPlayer->GetCameraPosition().ViewAngleTo(
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