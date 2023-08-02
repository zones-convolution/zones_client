#pragma once

#include "PluginProcessor.h"
#include "browser/BrowserComponent.h"
#include "components/SidebarContent.h"
#include "components/SidebarHeader.h"
#include "editor/EditorComponent.h"
#include "layout/sidebar/SidebarComponent.h"
#include "layout/tabs/TabsAction.h"
#include "layout/tabs/TabsComponent.h"
#include "layout/tabs/TabsController.h"
#include "layout/tabs/TabsModel.h"
#include "model/Model.h"
#include "zones_look_and_feel/LookAndFeel.h"
#include "zones_look_and_feel/components/PanelComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor & processor,
                                              const lager::reader<Model> & model,
                                              lager::context<Action> & context);
    ~AudioPluginAudioProcessorEditor () override = default;

private:
    static constexpr float kPreferredAspectRatio = 3.f / 4.f;
    static constexpr int kWindowMinimumWidth = 600;
    static constexpr int kWindowMaxWidth = 2000;

    AudioPluginAudioProcessor & processor_;

    const lager::reader<Model> model_;
    lager::context<Action> context_;
    lager::context<ProjectIrRepositoryAction> project_ir_repository_context_ {context_};
    lager::context<ParameterAction> parameter_context_ {context_};

    LookAndFeel look_and_feel_;

    EditorComponent editor_;
    PanelComponent editor_panel_ {editor_};

    BrowserComponent browser_;
    PanelComponent browser_panel_ {browser_};

    juce::TextButton settings_ {"SETTINGS"};
    PanelComponent settings_panel_ {settings_};

    TabsComponent tabs_component_;
    TabsController tabs_controller_ {tabs_component_};

    lager::store<TabsAction, TabsModel> store_ = lager::make_store<TabsAction> (
        TabsModel {},
        lager::with_manual_event_loop {},
        lager::with_deps (std::reference_wrapper<TabsControllerDelegate> (tabs_controller_)),
        lager::with_reducer (UpdateTabs));

    SidebarHeader sidebar_header_;
    PanelComponent sidebar_header_panel_ {sidebar_header_};

    SidebarContent sidebar_content_ {store_};
    PanelComponent sidebar_content_panel_ {sidebar_content_};

    juce::TextButton sidebar_footer_button_ {"footer"};
    PanelComponent sidebar_footer_panel_ {sidebar_footer_button_};

    SidebarComponent sidebar_component_ {sidebar_header_panel_,
                                         sidebar_content_panel_,
                                         sidebar_footer_panel_};

    void resized () override;
    void paint (juce::Graphics & graphics) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};