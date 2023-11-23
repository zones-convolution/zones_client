#include "Translations.h"

extern "C" const char translations_en_translate [];
extern "C" const unsigned translations_en_translate_size;

static const std::string kENTranslations{translations_en_translate};

Translations::Translations ()
{
    juce::LocalisedStrings::setCurrentMappings (new juce::LocalisedStrings{kENTranslations, true});
}
