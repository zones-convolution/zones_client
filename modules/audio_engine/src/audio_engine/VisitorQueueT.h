#pragma once

#include "format/IrData.h"

#include <choc/containers/choc_SingleReaderSingleWriterFIFO.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>
#include <variant>

template <typename T>
struct is_variant : std::false_type
{
};

template <typename... Args>
struct is_variant<std::variant<Args...>> : std::true_type
{
};

template <typename T>
inline constexpr bool is_variant_v = is_variant<T>::value;

template <typename Commands, typename Visitor>
class VisitorQueueT
{
    static_assert (is_variant_v<Commands>, "Commands must be a variant type");

public:
    explicit VisitorQueueT ()
    {
        command_queue_.reset (kMaxQueueSize);
    }

    void Service ()
    {
        jassert (visitor_ != nullptr);
        Commands command;
        while (command_queue_.pop (command))
            std::visit (*visitor_, command);
    }

    bool PushCommand (const Commands & command)
    {
        return command_queue_.push (command);
    }

    void SetVisitor (Visitor * visitor)
    {
        visitor_ = visitor;
    }

private:
    static constexpr int kMaxQueueSize = 400;

    Visitor * visitor_ = nullptr;
    choc::fifo::SingleReaderSingleWriterFIFO<Commands> command_queue_;
};
