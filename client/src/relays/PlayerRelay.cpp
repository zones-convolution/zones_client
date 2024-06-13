#include "PlayerRelay.h"

using namespace Zones;

PlayerRelay::PlayerRelay (juce::WebBrowserComponent & web_browser_component,
                          PlayerController & player_controller)
    : web_browser_component_ (web_browser_component)
    , player_controller_ (player_controller)
{
}

juce::WebBrowserComponent::Options
PlayerRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions
        .withNativeFunction ("player_update_native",
                             [this] (auto & var, auto complete)
                             {
                                 Player::PlayerState state;
                                 player_controller_.Play (state);
                                 complete ({});
                             })
        .withNativeFunction ("get_player_state",
                             [this] (auto & var, auto complete)
                             {
                                 auto state = player_controller_.GetPlayerState ();

                                 flatbuffers::FlatBufferBuilder builder (1024);

                                 PlayerStateBuilder player_state_builder (builder);
                                 player_state_builder.add_looping (state.is_looping);
                                 player_state_builder.add_playing (state.is_playing);
                                 player_state_builder.add_resource (
                                     PlayerResource {static_cast<int> (state.file)});

                                 auto player_state_offset = player_state_builder.Finish ();
                                 builder.Finish (player_state_offset);

                                 uint8_t * buf = builder.GetBufferPointer ();
                                 auto size = builder.GetSize ();
                                 complete ({buf, size});
                             });
}
