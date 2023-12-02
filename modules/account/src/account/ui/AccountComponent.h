#pragma once

#include "../AccountAction.h"
#include "../AccountModel.h";
#include "layout/tabs/TabsComponent.h"
#include "layout/tabs/TabsController.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>
#include <lager/reader.hpp>

class UnauthenticatedComponent : public juce::Component
{
public:
    UnauthenticatedComponent (lager::reader<AccountModel> account_reader,
                              lager::context<AccountAction> account_context);
    void resized () override;

private:
    juce::TextButton login_button_ {juce::translate ("login")};
    lager::reader<AccountModel> account_reader_;
    lager::context<AccountAction> account_context_;
};

class DeviceFlowComponent : public juce::Component
{
public:
    DeviceFlowComponent (lager::reader<AccountModel> account_reader,
                         lager::context<AccountAction> account_context);
    void resized () override;

private:
    lager::reader<AccountModel> account_reader_;
    lager::context<AccountAction> account_context_;

    juce::Label user_code_label_;
};

class ProfileComponent : public juce::Component
{
public:
    ProfileComponent (lager::reader<AccountModel> account_reader,
                      lager::context<AccountAction> account_context);
    void resized () override;

private:
    lager::reader<AccountModel> account_reader_;
    lager::context<AccountAction> account_context_;

    juce::Label email_label_;
    juce::TextButton logout_button_ {juce::translate ("logout")};
};

class AccountComponent : public juce::Component
{
public:
    AccountComponent (lager::reader<AccountModel> account_reader,
                      lager::context<AccountAction> account_context);
    void resized () override;

private:
    void UpdateDisplay (const AccountModel & account_model);

    lager::reader<AccountModel> account_reader_;
    lager::context<AccountAction> account_context_;

    TabsComponent tabs_component_;
    TabsController tabs_controller_ {tabs_component_};

    UnauthenticatedComponent unauthenticated_component_ {account_reader_, account_context_};
    DeviceFlowComponent device_flow_component_ {account_reader_, account_context_};
    ProfileComponent profile_component_ {account_reader_, account_context_};
};
