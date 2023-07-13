#include "AudioEngine.h"

AudioEngine::AudioEngine (CommandQueue & command_queue,
                          const ProjectIrLoadController & load_controller,
                          lager::reader<CurrentProjectIrOptional> current_ir_reader)
    : command_queue_ (command_queue)
    , current_ir_reader_ (current_ir_reader)
    , load_controller_ (load_controller)
{
    lager::watch (
        current_ir_reader_,
        [&command_queue, &load_controller] (const CurrentProjectIrOptional & current_project_ir)
        {
            if (current_project_ir.has_value ())
            {
                command_queue.LoadIr (load_controller.GetCurrentProjectIrData ());
            }
        });
}
