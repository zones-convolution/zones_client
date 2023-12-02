zones_add_module(
        MODULE
        account
        SOURCES
        src/account/AccountController.h
        src/account/AccountController.cpp
        src/account/AccountModel.h
        src/account/AccountAction.h
        src/account/AccountAction.cpp
        src/account/AccountUtils.h
        src/account/AccountUtils.cpp
        src/account/ui/AccountComponent.h
        src/account/ui/AccountComponent.cpp
        DEPS
        layout
        api
        juce::juce_core
        juce::juce_events
        lager
        keychain
        jwt-cpp)

zones_add_test(MODULE account SOURCES test/AccountControllerTests.cpp test/AccountActionTests.cpp test/AccountUtilsTests.cpp DEPS account)
