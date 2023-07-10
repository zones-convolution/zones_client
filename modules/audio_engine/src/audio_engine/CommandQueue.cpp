#include "CommandQueue.h"

CommandQueue::CommandQueue (Delegate & delegate)
    : delegate_ (delegate)
{
    command_queue_.reset (kMaxQueueSizeInBytes);
}

void CommandQueue::RTService ()
{
    command_queue_.popAllAvailable (
        [&] (void * data, uint32_t size)
        {
            auto command_id = *static_cast<const CommandID *> (data);
            switch (command_id)
            {
                case kLoadIr:
                    {
                        jassert (size == sizeof (LoadIrCommand));
                        auto load_ir_command = static_cast<LoadIrCommand *> (data);
                        delegate_.RTLoadIr (std::move (load_ir_command->GetData ()));
                    }
                    break;
                case kUpdateParameters:
                    jassert (size == sizeof (CommandID));
                    delegate_.RTUpdateParameters ();
                    break;
            }
        });
}

void CommandQueue::LoadIr (IrData && ir_data)
{
    auto load_ir_command = LoadIrCommand (std::move (ir_data));
    command_queue_.push (&load_ir_command, sizeof (load_ir_command));
}

void CommandQueue::UpdateParameters ()
{
    auto update_parameters_command = CommandID::kUpdateParameters;
    command_queue_.push (&update_parameters_command, sizeof (update_parameters_command));
}