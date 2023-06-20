#include "ScopedIrIO.h"

#include <utility>

ScopedIrIO::ScopedIrIO (std::filesystem::path directory_path)
    : directory_path_ (std::move (directory_path))
{
}
