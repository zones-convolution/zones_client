
#include "ProcessResultPool.h"
std::optional<ProcessResultPool::SharedBuffer>
ProcessResultPool::GetResult (const GraphStateKey & key)
{
    std::lock_guard guard (mutex_);
    if (process_results_.contains (key))
        return process_results_ [key];
    return std::nullopt;
}
void ProcessResultPool::CacheResult (const GraphStateKey & key,
                                     const ProcessResultPool::SharedBuffer & buffer)
{
    std::lock_guard guard (mutex_);
    process_results_ [key] = buffer;
}
void ProcessResultPool::RemoveUnusedKeys (const std::vector<GraphStateKey> & used_keys)
{
    std::lock_guard guard (mutex_);
    std::erase_if (process_results_,
                   [&used_keys] (const auto & item)
                   {
                       const auto & [key, _] = item;
                       return std::find (used_keys.begin (), used_keys.end (), key) ==
                              used_keys.end ();
                   });
}
