#pragma once

#include "IrReader.h"
#include "IrWriter.h"

#include <filesystem>
#include <lager/reader.hpp>

class ScopedIrIO
{
public:
    explicit ScopedIrIO (std::filesystem::path directory_path);

private:
    const std::filesystem::path directory_path_;
    lager::reader<int> test_reader;
};
