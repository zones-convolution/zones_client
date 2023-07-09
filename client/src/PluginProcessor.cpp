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
    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources ()
{
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

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels ();
    auto totalNumOutputChannels = getTotalNumOutputChannels ();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples ());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto * channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);
    }
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
    juce::ignoreUnused (destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void * data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

juce::AudioProcessor * JUCE_CALLTYPE createPluginFilter ()
{
    return new AudioPluginAudioProcessor ();
}