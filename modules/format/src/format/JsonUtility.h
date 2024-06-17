#pragma once

#include <nlohmann/json.hpp>

template <class J, class T>
void OptionalToJson (J & j, const char * name, const std::optional<T> & value)
{
    if (value)
        j [name] = *value;
}

template <class J, class T>
void OptionalFromJson (const J & j, const char * name, std::optional<T> & value)
{
    const auto it = j.find (name);
    if (it != j.end ())
        value = it->template get<T> ();
    else
        value = std::nullopt;
}
