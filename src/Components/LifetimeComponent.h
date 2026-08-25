#pragma once

#include <algorithm>
#include <cassert>

struct LifetimeComponent
{
    float lifetimeDuration;
    float remainingDuration;

    LifetimeComponent() = delete;

    explicit LifetimeComponent(const float t_lifetimeDuration)
        : lifetimeDuration(t_lifetimeDuration), remainingDuration(t_lifetimeDuration) {}

    void Decrease(const float amount)
    {
        assert(amount >= 0);
        remainingDuration = std::max(0.0f, remainingDuration - amount);
    }

    [[nodiscard]]
    bool IsAlive() const
    {
        return remainingDuration > 0.0f;
    }
};
