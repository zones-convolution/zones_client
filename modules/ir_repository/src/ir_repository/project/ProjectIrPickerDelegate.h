#pragma once
#include <filesystem>
#include <optional>

class ProjectIrPickerDelegate
{
public:
    struct Result
    {
        std::optional<std::string> ir_name;
        std::filesystem::path ir_path;
    };

    using ProjectIrPickerCallback = std::function<void (Result)>;
    virtual void PickIr (ProjectIrPickerCallback callback) = 0;
};