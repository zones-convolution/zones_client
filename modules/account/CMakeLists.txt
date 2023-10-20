zones_add_module(
        MODULE
        account
        SOURCES
        src/account/AccountController.h
        src/account/AccountController.cpp
        src/account/AccountModel.h
        src/account/AccountAction.h
        src/account/AccountAction.cpp
        DEPS
        api
        juce::juce_core
        juce::juce_events
        lager
        keychain)

zones_add_test(MODULE account SOURCES test/AccountControllerTests.cpp DEPS account)
