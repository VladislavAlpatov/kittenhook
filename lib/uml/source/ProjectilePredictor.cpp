//
// Created by vlad on 11/6/23.
//

#include "uml/ProjectilePredictor.h"
#include "uml/Vector3.h"
#include "uml/angles.h"

#include <cmath>

namespace uml::prediction
{

    std::optional<Vector3>
    ProjectilePredictor::CalculateViewAngles(const Vector3 &origin,
                                             const Vector3 &target,
                                             const Vector3 &targetVelocity,
                                             float gravity,
                                             float bulletSpeed,
                                             float bulletGravity)
    {
        constexpr float maxTime = 1.0f;
        constexpr float timeStep = 0.1f;

        for (float time = 0.0f; time <= maxTime; time += timeStep)
        {
            const auto predPos = target + targetVelocity * time;
            const auto angle = CalculateAimPitch(origin, target, bulletSpeed, bulletGravity);

            if (!angle.has_value())
                return std::nullopt;

            const auto timeToHit = ProjectileTravelTime(origin.DistTo(target), *angle, bulletSpeed);

            if (timeToHit > time)
                return std::nullopt;


            auto viewAngles = origin.ViewAngleTo(predPos);
            viewAngles.x = angle.value();

            return viewAngles;
        }
    }

    std::optional<float>
    ProjectilePredictor::CalculateAimPitch(const Vector3 &origin, const Vector3 &target,
                                           float bulletSpeed, float bulletGravity)
    {
        const auto delta = target - origin;

        const auto distance = delta.Length2D();


        const auto root = std::pow(bulletSpeed, 4)
                - bulletGravity * (bulletGravity * std::pow(distance, 2)
                + 2.f * delta.z * std::pow(bulletSpeed, 2));

        if (root < 0.f)
            return std::nullopt;


        return angles::RadToDeg(std::atan(std::pow(bulletSpeed, 2)
        - std::sqrt(root) / (bulletGravity * distance)));
    }

    float ProjectilePredictor::ProjectileTravelTime(float distance, float angle, float speed)
    {
        return distance / (std::cos(angles::DegToRad(angle)) * speed);
    }
}