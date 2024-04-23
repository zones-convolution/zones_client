#pragma once
#include "../BrowserModel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class ZoneCardComponent : public juce::Component
{
public:
    ZoneCardComponent (const std::string & image_path, const std::string & name);
    void paint (juce::Graphics & g) override;
    void resized () override;

private:
    juce::Label name_label_ {"Zone Name"};
    juce::Image background_image_;
};

class Top10ContentComponent : public juce::Component
{
public:
    Top10ContentComponent ();
    void resized () override;
    float GetRequiredHeightForLayout ();

private:
    std::array<ZoneCardComponent, 16> zone_cards_ {
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/maple.jpg", "Maple"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/forest.jpg", "Forest"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/cave.jpg", "Cave"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/cave1.jpg", "Cave"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/castle.jpg", "Castle"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/canyon.jpg", "Canyon"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/church.jpg", "Church"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/room.jpg", "Room"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/maple.jpg", "Maple"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/forest.jpg", "Forest"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/cave.jpg", "Cave"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/cave1.jpg", "Cave"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/castle.jpg", "Castle"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/canyon.jpg", "Canyon"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/church.jpg", "Church"},
        ZoneCardComponent {"/Users/leonps/Downloads/zones_images/room.jpg", "Room"}};
};

class Top10ViewComponent : public juce::Component
{
public:
    Top10ViewComponent ();
    void Update (const Top10View & top_10_view);
    void resized () override;

private:
    void PerformLayout ();

    Top10ContentComponent top_10_content_component_;
    juce::Viewport viewport_;
};
