#include "MeteringRelay.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

static void to_json (json & data, const MeteringRelay::ChannelMetering & channel_metering)
{
    data = json {
        {"isClipping", channel_metering.is_clipping},
        {"peakValue", channel_metering.peak_value},
    };
}

MeteringRelay::MeteringRelay (AudioGraphMetering & input_metering,
                              AudioGraphMetering & output_metering)
    : input_metering_ (input_metering)
    , output_metering_ (output_metering)
{
}

juce::WebBrowserComponent::Options
MeteringRelay::buildOptions (const juce::WebBrowserComponent::Options & initialOptions)
{
    return initialOptions.withNativeFunction (
        "get_metering_native",
        [this] (auto & var, auto complete)
        {
            JUCE_ASSERT_MESSAGE_THREAD;

            std::vector<std::vector<ChannelMetering>> metering {{}, {}};

            for (auto channel_index = 0; channel_index < input_metering_.GetNumChannels ();
                 ++channel_index)
                metering [0].push_back ({
                    .is_clipping = input_metering_.GetChannelClipping (channel_index),
                    .peak_value = input_metering_.GetChannelPeak (channel_index),
                });

            for (auto channel_index = 0; channel_index < output_metering_.GetNumChannels ();
                 ++channel_index)
                metering [1].push_back ({
                    .is_clipping = output_metering_.GetChannelClipping (channel_index),
                    .peak_value = output_metering_.GetChannelPeak (channel_index),
                });

            json data = metering;
            complete ({data.dump ()});
        });
}
