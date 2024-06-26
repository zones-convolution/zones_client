#include "PlayerRelay.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Player
{
NLOHMANN_JSON_SERIALIZE_ENUM (Player::Resource,
                              {{Player::Resource::kSnare, "snare"},
                               {Player::Resource::kNumbers, "numbers"}})

static void from_json (const json & data, PlayerState & player_state)
{
    data.at ("resource").get_to (player_state.resource);
    data.at ("looping").get_to (player_state.looping);
    data.at ("playing").get_to (player_state.playing);
}

static void to_json (json & data, const PlayerState & player_state)
{
    data = json {
        {"resource", player_state.resource},
        {"looping", player_state.looping},
        {"playing", player_state.playing},
    };
}
}

PlayerRelay::PlayerRelay (juce::WebBrowserComponent & web_browser_component,
                          PlayerController & player_controller)
    : web_browser_component_ (web_browser_component)
    , player_controller_ (player_controller)
{
    player_controller_.OnPlayerStateUpdated = [&]
    {
        auto state = player_controller_.GetPlayerState ();
        json data = state;

        JUCE_ASSERT_MESSAGE_THREAD;
        web_browser_component_.emitEventIfBrowserIsVisible ("on_player_update_native",
                                                            {data.dump ()});
    };
}

PlayerRelay::~PlayerRelay ()
{
    player_controller_.OnPlayerStateUpdated = nullptr;
}

juce::WebBrowserComponent::Options
PlayerRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions
        .withNativeFunction ("player_update_native",
                             [this] (auto & var, auto complete)
                             {
                                 Player::PlayerState state;
                                 json::parse (var [0].toString ().toStdString ()).get_to (state);
                                 player_controller_.SetPlayerState (state);

                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({});
                             })
        .withNativeFunction ("get_player_state_native",
                             [this] (auto & var, auto complete)
                             {
                                 auto state = player_controller_.GetPlayerState ();
                                 json data = state;

                                 JUCE_ASSERT_MESSAGE_THREAD;
                                 complete ({data.dump ()});
                             });
}
