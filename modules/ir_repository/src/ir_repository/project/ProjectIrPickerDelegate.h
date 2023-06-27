#pragma once
#include <filesystem>
#include <optional>

class ProjectIrPickerDelegate
{
public:
    struct Result
    {
        std::string name;
        std::filesystem::path path;
        std::string description;
    };

    using ProjectIrPickerCallback = std::function<void (Result)>;
    virtual void PickIr (ProjectIrPickerCallback callback) = 0;
};