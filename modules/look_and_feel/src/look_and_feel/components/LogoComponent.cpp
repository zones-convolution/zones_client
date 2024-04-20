#include "LogoComponent.h"

extern "C" const char assets_zones_light_svg [];
extern "C" const unsigned assets_zones_light_svg_size;

LogoComponent::LogoComponent ()
{
    zones_logo_drawable_ =
        juce::Drawable::createFromImageData (assets_zones_light_svg, assets_zones_light_svg_size);
}

void LogoComponent::paint (juce::Graphics & g)
{
    zones_logo_drawable_->setTransformToFit (getLocalBounds ().toFloat (),
                                             juce::RectanglePlacement::xLeft |
                                                 juce::RectanglePlacement::centred);

    zones_logo_drawable_->draw (g, 1.f);
}
