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

static void from_json (const json & data,
                       MeteringRelay::ResetChannelClipping & reset_channel_clipping)
{
    data.at ("groupIndex").get_to (reset_channel_clipping.group_index);
    data.at ("channelIndex").get_to (reset_channel_clipping.channel_index);
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
    return initialOptions
        .withNativeFunction (
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
            })
        .withNativeFunction (
            "reset_channel_clipping_native",
            [this] (auto & var, auto complete)
            {
                ResetChannelClipping reset_channel_clipping {};
                json::parse (var [0].toString ().toStdString ()).get_to (reset_channel_clipping);

                if (reset_channel_clipping.group_index == 0)
                    input_metering_.ResetClipping (reset_channel_clipping.channel_index);
                if (reset_channel_clipping.group_index == 1)
                    output_metering_.ResetClipping (reset_channel_clipping.channel_index);

                JUCE_ASSERT_MESSAGE_THREAD;
                complete ({});
            });
}
