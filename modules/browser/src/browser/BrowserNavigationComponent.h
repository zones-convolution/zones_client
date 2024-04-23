#include "BrowserAction.h"
#include "BrowserModel.h"
#include "look_and_feel/BoxIcons.h"
#include "look_and_feel/components/IconButton.h"
#include "look_and_feel/components/PanelComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include <lager/context.hpp>
#include <lager/reader.hpp>
#include <lager/store.hpp>

class BrowserHistoryComponent : public juce::Component
{
public:
    BrowserHistoryComponent ();
    ~BrowserHistoryComponent () override = default;

    void resized () override;

private:
    IconButton navigate_back_ {"Go Back", BoxIcons::kBxChevronLeft};
    IconButton navigate_forward_ {"Go Forward", BoxIcons::kBxChevronRight};
    juce::Label current_view_;
};

class BrowserNavigationComponent : public juce::Component
{
public:
    BrowserNavigationComponent (lager::store<BrowserAction, BrowserModel> & browser_store);
    ~BrowserNavigationComponent () override = default;

    void resized () override;

private:
    lager::reader<BrowserModel> browser_model_;
    lager::context<BrowserAction> browser_context_;

    BrowserHistoryComponent history_component_;
    PanelComponent history_panel_ {history_component_};
};
