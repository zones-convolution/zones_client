zones_add_module(
        MODULE
        layout
        SOURCES
        src/layout/tabs/TabsAction.h
        src/layout/tabs/TabsAction.cpp
        src/layout/tabs/TabsComponent.h
        src/layout/tabs/TabsComponent.cpp
        src/layout/tabs/TabsController.h
        src/layout/tabs/TabsController.cpp
        src/layout/tabs/TabsModel.h
        src/layout/WithJuceEventLoop.h
        DEPS
        juce::juce_core
        juce::juce_gui_basics
        lager)

zones_add_test(MODULE layout SOURCES test/TabsActionTests.cpp
        DEPS layout)
