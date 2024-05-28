#include "PluginProcessor.h"

#include "PluginEditor.h"

AudioPluginAudioProcessor::AudioPluginAudioProcessor ()
    : AudioProcessor (BusesProperties ()
                          .withInput ("Input", juce::AudioChannelSet::stereo (), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo (), true))
    , processor_container_ (*this)
{
}

const juce::String AudioPluginAudioProcessor::getName () const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi () const
{
    return false;
}

bool AudioPluginAudioProcessor::producesMidi () const
{
    return false;
}

bool AudioPluginAudioProcessor::isMidiEffect () const
{
    return false;
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
    processor_container_.Prepare (sampleRate, samplesPerBlock, getBusesLayout ());
}

void AudioPluginAudioProcessor::releaseResources ()
{
    processor_container_.graph_.reset ();
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout & layouts) const
{
    auto main_output_channel_set = layouts.getMainOutputChannelSet ();

    if (main_output_channel_set != juce::AudioChannelSet::mono () &&
        main_output_channel_set != juce::AudioChannelSet::stereo () &&
        main_output_channel_set != juce::AudioChannelSet::ambisonic (1) &&
        main_output_channel_set != juce::AudioChannelSet::quadraphonic ())
        return false;

    if (main_output_channel_set != layouts.getMainInputChannelSet ())
        return false;

    return true;
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

    processor_container_.command_queue_.Service ();
    processor_container_.graph_.process (process_context);
}

bool AudioPluginAudioProcessor::hasEditor () const
{
    return true;
}

juce::AudioProcessorEditor * AudioPluginAudioProcessor::createEditor ()
{
    LookAndFeel::setDefaultLookAndFeel (&look_and_feel_);
    return new AudioPluginAudioProcessorEditor (*this, processor_container_);
}

void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock & destData)
{
}

void AudioPluginAudioProcessor::setStateInformation (const void * data, int sizeInBytes)
{
}

juce::AudioProcessor * JUCE_CALLTYPE createPluginFilter ()
{
    return new AudioPluginAudioProcessor ();
}