set(BINARY zones_client)

set(LINK_JUCE juce::juce_gui_extra juce::juce_audio_devices
        juce::juce_audio_utils)

set(LINK_COMMON
        ir_repository
        ir_format
        ir_engine
        look_and_feel
        audio_engine
        model
        layout
        browser
        editor
        zones_convolver
        preferences)

set(SOURCES
        src/PluginProcessor.h
        src/PluginProcessor.cpp
        src/PluginEditor.h
        src/PluginEditor.cpp
        src/ProcessorContainer.h
        src/ProcessorContainer.cpp
        src/Translations.h
        src/Translations.cpp
        src/components/metering/MeterComponent.h
        src/components/metering/MeterComponent.cpp
        src/components/metering/ChannelBar.h
        src/components/metering/ChannelBar.cpp
        src/components/metering/DiscreteLevelLabels.h
        src/components/metering/DiscreteLevelLabels.cpp
        src/components/metering/ClippingIndicator.h
        src/components/metering/ClippingIndicator.cpp
        src/components/SidebarContent.h
        src/components/SidebarContent.cpp
        src/components/SidebarHeader.h
        src/components/SidebarHeader.cpp
        src/components/SidebarFooter.h
        src/components/SidebarFooter.cpp
        src/WithJuceEventLoop.h)

juce_add_plugin(
        ${BINARY}
        PLUGIN_MANUFACTURER_CODE
        Zone
        PLUGIN_CODE
        Zone
        FORMATS
        AU
        VST3
        Standalone
        COPY_PLUGIN_AFTER_BUILD
        TRUE
        PRODUCT_NAME
        Zones
        COMPANY_NAME
        Zones
        BUNDLE_ID
        com.zones.zones
        VERSION
        0.0.1)

target_sources(${BINARY} PRIVATE ${SOURCES})

target_link_libraries(
        ${BINARY}
        PRIVATE ${LINK_JUCE} ${LINK_COMMON}
        PUBLIC ${MODULE_LINK_CONF})

target_compile_options(${BINARY} INTERFACE -Wall -Wextra -Werror)

embed_resources(${BINARY} translations/en.translate)