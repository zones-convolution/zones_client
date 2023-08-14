#include "Translations.h"

extern "C" const char translations_en_translate [];
extern "C" const unsigned translations_en_translate_size;

static const std::string kENTranslations {translations_en_translate};
static juce::LocalisedStrings kENLocalisedStrings {kENTranslations, true};

Translations::Translations ()
{
    juce::LocalisedStrings::setCurrentMappings (&kENLocalisedStrings);
}
