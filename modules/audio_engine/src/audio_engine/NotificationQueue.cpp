#include "NotificationQueue.h"

NotificationQueue::NotificationQueue ()
{
    notification_queue_.reset (400);
}

void NotificationQueue::Service ()
{
    notification_queue_.popAllAvailable (
        [&] (const void * data, uint32_t size)
        {
            auto notification_id = *static_cast<const NotificationID *> (data);
            switch (notification_id)
            {
                case kPatternIndexDidChange:
                    {
                        auto pattern_index_updated_notification =
                            *static_cast<const PatternIndexUpdatedNotification *> (data);
                    }
                    break;
            }
        });
}

void NotificationQueue::NotifyPatternIndexUpdated (int pattern_index)
{
    auto pattern_index_updated_notification = PatternIndexUpdatedNotification {
        .notification_id = NotificationID::kPatternIndexDidChange, .notification = pattern_index};
    notification_queue_.push (&pattern_index_updated_notification,
                              sizeof (pattern_index_updated_notification));
}