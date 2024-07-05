zones_add_module(
        MODULE
        look_and_feel
        SOURCES
        src/look_and_feel/LookAndFeel.cpp
        src/look_and_feel/LookAndFeel.h
        src/look_and_feel/components/PanelComponent.h
        src/look_and_feel/components/PanelComponent.cpp
        src/look_and_feel/components/LogoComponent.h
        src/look_and_feel/components/LogoComponent.cpp
        src/look_and_feel/components/DividerComponent.h
        src/look_and_feel/components/DividerComponent.cpp
        src/look_and_feel/components/IconComponent.h
        src/look_and_feel/components/IconComponent.cpp
        src/look_and_feel/components/IconTextButton.h
        src/look_and_feel/components/IconTextButton.cpp
        src/look_and_feel/components/IconButton.h
        src/look_and_feel/components/IconButton.cpp
        src/look_and_feel/BoxIcons.h
        src/look_and_feel/BoxIcons.cpp
        DEPS
        juce::juce_gui_basics)

embed_resources(look_and_feel assets/zones_light.svg assets/boxicons.ttf)

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
        look_and_feel
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)