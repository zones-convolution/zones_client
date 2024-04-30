#include "Action.h"

Result Update (Model model, Action action)
{
    return std::visit (
        lager::visitor {[&] (const ZoneRepositoryAction & zone_repository_action) -> Result
                        {
                            auto [updated_model, effect] = UpdateZoneRepository (
                                model.zone_repository_model, zone_repository_action);
                            model.zone_repository_model = updated_model;
                            return {model, effect};
                        }},
        action);
}
