#pragma once
#include <choc/containers/choc_VariableSizeFIFO.h>

class NotificationQueue
{
public:
    enum NotificationID
    {
        kPatternIndexDidChange
    };

    template <class T>
    struct NotificationBase
    {
        NotificationID notification_id;
        T notification;
    };

    using PatternIndexUpdatedNotification = NotificationBase<int>;

    NotificationQueue ();
    void Service ();
    void NotifyPatternIndexUpdated (int pattern_index);
    
private:
    choc::fifo::VariableSizeFIFO notification_queue_;
};
