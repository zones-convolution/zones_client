#include "StoryboardComponent.h"
#include "zones_look_and_feel/LookAndFeel.h"

#include <juce_gui_basics/juce_gui_basics.h>

class StoryboardApplication : public juce::JUCEApplication
{
public:
    StoryboardApplication ()
    {
        juce::LookAndFeel::setDefaultLookAndFeel (&look_and_feel_);
    }

    const juce::String getApplicationName () override
    {
        return "Storyboard";
    }

    const juce::String getApplicationVersion () override
    {
        return "1.0.0";
    }

    bool moreThanOneInstanceAllowed () override
    {
        return true;
    }

    void initialise (const juce::String & commandLine) override
    {
        juce::ignoreUnused (commandLine);
        mainWindow.reset (new MainWindow (getApplicationName ()));
    }

    void shutdown () override
    {
        mainWindow = nullptr;
    }

    void systemRequestedQuit () override
    {
        quit ();
    }

    void anotherInstanceStarted (const juce::String & commandLine) override
    {
        juce::ignoreUnused (commandLine);
    }

    class MainWindow : public juce::DocumentWindow
    {
    public:
        explicit MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance ().getDefaultLookAndFeel ().findColour (
                                  ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new StoryboardComponent (), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
#else
            setResizable (true, true);
            centreWithSize (getWidth (), getHeight ());
#endif

            setVisible (true);
        }

        void closeButtonPressed () override
        {
            JUCEApplication::getInstance ()->systemRequestedQuit ();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    LookAndFeel look_and_feel_;
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION (StoryboardApplication)