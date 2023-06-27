#pragma once

#include <filesystem>
#include <optional>

class ProjectPathPickerDelegate
{
public:
    struct Result
    {
        std::filesystem::path path;
    };

    using ProjectPathPickerCallback = std::function<void (Result)>;
    virtual void PickPath (ProjectPathPickerCallback callback) = 0;
};