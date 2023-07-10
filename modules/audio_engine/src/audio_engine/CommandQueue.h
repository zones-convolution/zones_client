#pragma once

#include "ir_repository/IrData.h"

#include <choc/containers/choc_VariableSizeFIFO.h>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_core/juce_core.h>

class CommandQueue
{
public:
    struct Delegate
    {
        virtual void RTLoadIr (IrData && ir_data) = 0;
        virtual void RTUpdateParameters () = 0;
    };

    enum CommandID
    {
        kLoadIr,
        kUpdateParameters
    };

    template <CommandID cmd, typename T>
    class CommandBase
    {
    public:
        explicit CommandBase (T && data)
            : data_ (std::move (data))
        {
        }

        T & GetData ()
        {
            return data_;
        }

    private:
        /**
         * This order is imperative to how the commands are read from the queue and should not be
         * re-ordered!
         */
        const CommandID command_id = cmd;
        T data_;
    };

    explicit CommandQueue (Delegate & delegate);

    using LoadIrCommand = CommandBase<CommandID::kLoadIr, IrData>;
    using UpdateParametersCommand = CommandID;

    void LoadIr (IrData && ir_data);
    void UpdateParameters ();

    void RTService ();

private:
    static constexpr int kMaxQueueSize = 400;

    Delegate & delegate_;
    choc::fifo::VariableSizeFIFO command_queue_;
};
