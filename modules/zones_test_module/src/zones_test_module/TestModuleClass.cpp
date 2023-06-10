#include "TestModuleClass.h"

TestModuleClass::TestModuleClass ()
{
    m_printMe = "TESTMODULECLASS";
}

juce::String TestModuleClass::getPrintMe ()
{
    return m_printMe;
}
