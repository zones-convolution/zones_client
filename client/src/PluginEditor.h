#pragma once

#include "PluginProcessor.h"
#include "ProcessorContainer.h"
#include "browser/BrowserNavigationComponent.h"
#include "browser/import/ImportComponent.h"
#include "components/SidebarContent.h"
#include "components/SidebarFooter.h"
#include "components/SidebarHeader.h"
#include "editor/EditorComponent.h"
#include "layout/sidebar/SidebarComponent.h"
#include "layout/tabs/TabsAction.h"
#include "layout/tabs/TabsComponent.h"
#include "layout/tabs/TabsController.h"
#include "layout/tabs/TabsModel.h"
#include "look_and_feel/components/PanelComponent.h"
#include "model/Model.h"
#include "preferences/PreferencesComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor & processor,
                                              ProcessorContainer & processor_container);
    ~AudioPluginAudioProcessorEditor () override;

private:
    static constexpr float kPreferredAspectRatio = 3.f / 4.f;
    static constexpr int kWindowMinimumWidth = 600;
    static constexpr int kWindowMaxWidth = 2000;

    AudioPluginAudioProcessor & processor_;
    ProcessorContainer & processor_container_;

    lager::reader<Model> model_;
    lager::context<Action> context_;

    EditorComponent editor_;

    lager::store<BrowserAction, BrowserModel> browser_store_ =
        lager::make_store<BrowserAction> (BrowserModel {},
                                          WithJuceEventLoop {processor_container_.thread_pool_},
                                          lager::with_reducer (UpdateBrowser));

    lager::store<TabsAction, TabsModel> store_ = lager::make_store<TabsAction> (
        TabsModel {},
        WithJuceEventLoop {processor_container_.thread_pool_},
        lager::with_deps (std::reference_wrapper<TabsControllerDelegate> (tabs_controller_)),
        lager::with_reducer (UpdateTabs));

    BrowserNavigationComponent browser_ {browser_store_, model_, context_};

    ImportComponent import_component_;
    PanelComponent import_panel_ {import_component_};

    PreferencesComponent preferences_component_;
    PanelComponent settings_panel_ {preferences_component_};

    TabsComponent tabs_component_;
    TabsController tabs_controller_ {tabs_component_};

    SidebarHeader sidebar_header_;
    PanelComponent sidebar_header_panel_ {sidebar_header_};

    SidebarContent sidebar_content_ {store_};
    PanelComponent sidebar_content_panel_ {sidebar_content_};

    SidebarFooter sidebar_footer_;
    PanelComponent sidebar_footer_panel_ {sidebar_footer_};

    SidebarComponent sidebar_component_ {sidebar_header_panel_,
                                         sidebar_content_panel_,
                                         sidebar_footer_panel_};

    IrEngine & ir_engine_;

    void resized () override;
    void paint (juce::Graphics & graphics) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
