#include "ParameterAction.h"

ParameterModel UpdateParameter (ParameterModel model, ParameterAction action)
{
    return std::visit (
        lager::visitor {[&] (const SetRoomSizeAction & set_room_size_action) -> ParameterModel
                        {
                            model.room_size = set_room_size_action.room_size;
                            return model;
                        },
                        [&] (const SetDryWetMixAction & set_dry_wet_mix_action) -> ParameterModel
                        {
                            model.dry_wet_mix = set_dry_wet_mix_action.dry_wet_mix;
                            return model;
                        }},
        action);
}
