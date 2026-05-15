#pragma once

#include <string>
#include <utility>

struct NameComponent
{
    std::string name;

    NameComponent() = default;

    explicit NameComponent(std::string t_name) : name(std::move(t_name))
    {
    }
};
