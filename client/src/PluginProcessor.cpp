#include "PluginProcessor.h"

#include "PluginEditor.h"

AudioPluginAudioProcessor::AudioPluginAudioProcessor ()
    : AudioProcessor (BusesProperties ()
#if ! JucePlugin_IsMidiEffect
    #if ! JucePlugin_IsSynth
                          .withInput ("Input", juce::AudioChannelSet::stereo (), true)
    #endif
                          .withOutput ("Output", juce::AudioChannelSet::stereo (), true)
#endif
                          )
    , audio_engine_ (command_queue_,
                     project_ir_load_controller_,
                     project_ir_store_.zoom (
                         lager::lenses::attr (&ProjectIrRepositoryModel::current_project_ir)))
{
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor ()
{
}

const juce::String AudioPluginAudioProcessor::getName () const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi () const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::producesMidi () const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool AudioPluginAudioProcessor::isMidiEffect () const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds () const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms ()
{
    return 1;
}

int AudioPluginAudioProcessor::getCurrentProgram ()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String & newName)
{
    juce::ignoreUnused (index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    graph_.prepare (
        juce::dsp::ProcessSpec {sampleRate,
                                static_cast<juce::uint32> (samplesPerBlock),
                                static_cast<juce::uint32> (getTotalNumOutputChannels ())});
}

void AudioPluginAudioProcessor::releaseResources ()
{
    graph_.reset ();
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout & layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else

    if (layouts.getMainOutputChannelSet () != juce::AudioChannelSet::mono () &&
        layouts.getMainOutputChannelSet () != juce::AudioChannelSet::stereo ())
        return false;

    #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet () != layouts.getMainInputChannelSet ())
        return false;
    #endif

    return true;
#endif
}

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float> & buffer,
                                              juce::MidiBuffer & midiMessages)
{
    juce::ignoreUnused (midiMessages);
    juce::ScopedNoDenormals no_denormals;

    auto total_num_input_channels = getTotalNumInputChannels ();
    auto total_num_output_channels = getTotalNumOutputChannels ();

    for (auto i = total_num_input_channels; i < total_num_output_channels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples ());

    const auto num_channels = juce::jmax (total_num_input_channels, total_num_output_channels);

    auto process_block =
        juce::dsp::AudioBlock<float> (buffer).getSubsetChannelBlock (0, (size_t) num_channels);
    juce::dsp::ProcessContextReplacing<float> process_context (process_block);

    command_queue_.RTService ();
    graph_.process (process_context);
}

bool AudioPluginAudioProcessor::hasEditor () const
{
    return true;
}

juce::AudioProcessorEditor * AudioPluginAudioProcessor::createEditor ()
{
    return new AudioPluginAudioProcessorEditor (*this, project_ir_store_, project_ir_store_);
}

void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock & destData)
{
    if (! project_ir_store_->project_paths.empty ())
    {
        auto state = std::make_unique<juce::XmlElement> ("Zones");
        state->setAttribute (juce::Identifier {"project_path"},
                             project_ir_store_->project_paths.front ().string ());
        copyXmlToBinary (*state, destData);
    }
}

void AudioPluginAudioProcessor::setStateInformation (const void * data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> state (getXmlFromBinary (data, sizeInBytes));
    auto project_path = state->getStringAttribute ("project_path", "");
    if (project_path.isNotEmpty ())
        project_ir_store_.dispatch (
            AddProjectPathAction {.project_path = project_path.toStdString ()});
}

juce::AudioProcessor * JUCE_CALLTYPE createPluginFilter ()
{
    return new AudioPluginAudioProcessor ();
}