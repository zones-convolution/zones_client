#pragma once

#include "format/IrData.h"

#include <choc/containers/choc_SingleReaderSingleWriterFIFO.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>
#include <lager/util.hpp>
#include <variant>

template <typename Commands, typename Visitor>
class VisitorQueue
{
    static_assert (lager::is_variant<Commands>::value, "Commands must be a variant type");

public:
    explicit VisitorQueue ()
    {
        command_queue_.reset (kMaxQueueSize);
    }

    void Service ()
    {
        jassert (visitor_ != nullptr);

        Commands command;
        while (command_queue_.pop (command))
            std::visit (visitor_, command);
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
