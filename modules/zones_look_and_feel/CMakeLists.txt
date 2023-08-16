zones_add_module(
        MODULE
        zones_look_and_feel
        SOURCES
        src/zones_look_and_feel/LookAndFeel.cpp
        src/zones_look_and_feel/LookAndFeel.h
        src/zones_look_and_feel/components/PanelComponent.h
        src/zones_look_and_feel/components/PanelComponent.cpp
        src/zones_look_and_feel/components/LogoComponent.h
        src/zones_look_and_feel/components/LogoComponent.cpp
        src/zones_look_and_feel/components/DividerComponent.h
        src/zones_look_and_feel/components/DividerComponent.cpp
        src/zones_look_and_feel/components/IconComponent.h
        src/zones_look_and_feel/components/IconComponent.cpp
        src/zones_look_and_feel/components/IconTextButton.h
        src/zones_look_and_feel/components/IconTextButton.cpp
        src/zones_look_and_feel/BoxIcons.h
        src/zones_look_and_feel/BoxIcons.cpp
        DEPS
        juce::juce_gui_basics)

embed_resources(zones_look_and_feel assets/zones_light.svg assets/boxicons.ttf)

juce_add_gui_app(storyboard
        PRODUCT_NAME "storyboard")

target_sources(storyboard
        PRIVATE
        storyboard/Main.cpp
        storyboard/StoryboardComponent.h
        storyboard/StoryboardComponent.cpp
        storyboard/Buttons.h
        storyboard/Buttons.cpp
        storyboard/Combos.h
        storyboard/Combos.cpp
        storyboard/Visualisers.h
        storyboard/Visualisers.cpp)

target_compile_definitions(storyboard
        PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0)

target_link_libraries(storyboard
        PRIVATE
        juce::juce_core
        juce::juce_gui_basics
        zones_look_and_feel
        visualiser
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)