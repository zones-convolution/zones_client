#pragma once
#include <juce_core/juce_core.h>

class TestModuleClass
{
public:
    TestModuleClass();

    juce::String getPrintMe();
private:
    juce::String m_printMe;
};