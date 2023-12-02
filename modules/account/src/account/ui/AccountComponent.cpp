#include "AccountComponent.h"

UnauthenticatedComponent::UnauthenticatedComponent (lager::reader<AccountModel> account_reader,
                                                    lager::context<AccountAction> account_context)
    : account_reader_ (account_reader)
    , account_context_ (account_context)
{
    addAndMakeVisible (login_button_);
    login_button_.onClick = [&] () { account_context_.dispatch (DeviceCodeAction {}); };
}

void UnauthenticatedComponent::resized ()
{
    login_button_.setBounds (getLocalBounds ());
}

DeviceFlowComponent::DeviceFlowComponent (lager::reader<AccountModel> account_reader,
                                          lager::context<AccountAction> account_context)
    : account_reader_ (account_reader)
    , account_context_ (account_context)
{
    addAndMakeVisible (user_code_label_);
    lager::watch (account_reader_,
                  [&] (const AccountModel & model)
                  {
                      if (model.device_flow.has_value ())
                          user_code_label_.setText ("User Code: " + model.device_flow->user_code,
                                                    juce::dontSendNotification);
                      else
                          user_code_label_.setText ("loading", juce::dontSendNotification);
                  });
}

void DeviceFlowComponent::resized ()
{
    user_code_label_.setBounds (getLocalBounds ());
}

ProfileComponent::ProfileComponent (lager::reader<AccountModel> account_reader,
                                    lager::context<AccountAction> account_context)
    : account_reader_ (account_reader)
    , account_context_ (account_context)
{
    addAndMakeVisible (email_label_);
    addAndMakeVisible (logout_button_);

    logout_button_.onClick = [&] () { account_context_.dispatch (ClearSessionAction {}); };

    lager::watch (account_reader_,
                  [&] (const AccountModel & model)
                  {
                      if (model.session.has_value ())
                      {
                          auto profile = model.session->profile;
                          email_label_.setText ("Profile Email: " + profile.email,
                                                juce::dontSendNotification);
                      }
                  });
}

void ProfileComponent::resized ()
{
    juce::FlexBox layout;
    layout.flexDirection = juce::FlexBox::Direction::column;

    layout.items.add (juce::FlexItem (email_label_).withFlex (1.f));
    layout.items.add (juce::FlexItem (logout_button_).withFlex (1.f));

    layout.performLayout (getLocalBounds ().toFloat ());
}

AccountComponent::AccountComponent (lager::reader<AccountModel> account_reader,
                                    lager::context<AccountAction> account_context)
    : account_reader_ (account_reader)
    , account_context_ (account_context)
{
    addAndMakeVisible (tabs_component_);
    tabs_controller_.AddTab ("profile", profile_component_);
    tabs_controller_.AddTab ("device_flow", device_flow_component_);
    tabs_controller_.AddTab ("unauthenticated", unauthenticated_component_);

    UpdateDisplay (*account_reader_);
    lager::watch (account_reader_, [&] (const AccountModel & model) { UpdateDisplay (model); });

    account_context_.dispatch (LoadSessionFromKeychainAction {});
}

void AccountComponent::resized ()
{
    juce::FlexBox flex_box;
    flex_box.flexDirection = juce::FlexBox::Direction::column;
    flex_box.alignContent = juce::FlexBox::AlignContent::center;

    flex_box.items.add (juce::FlexItem (tabs_component_).withHeight (100.f));
    flex_box.performLayout (getLocalBounds ().toFloat ());
}

void AccountComponent::UpdateDisplay (const AccountModel & account_model)
{
    if (account_model.session.has_value ())
        tabs_controller_.LoadTab ("profile", false);
    else if (account_model.device_flow_status == AccountModel::DeviceFlowStatus::kIdle)
        tabs_controller_.LoadTab ("unauthenticated", false);
    else
        tabs_controller_.LoadTab ("device_flow", false);
}
