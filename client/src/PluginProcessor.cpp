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
    processor_container_.graph_.prepare (
        juce::dsp::ProcessSpec {sampleRate,
                                static_cast<juce::uint32> (samplesPerBlock),
                                static_cast<juce::uint32> (getTotalNumOutputChannels ())});
}

void AudioPluginAudioProcessor::releaseResources ()
{
    processor_container_.graph_.reset ();
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

    processor_container_.command_queue_.Service ();
    processor_container_.graph_.process (process_context);
}

bool AudioPluginAudioProcessor::hasEditor () const
{
    return true;
}

juce::AudioProcessorEditor * AudioPluginAudioProcessor::createEditor ()
{
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