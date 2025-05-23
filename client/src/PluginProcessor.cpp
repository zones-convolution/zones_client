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
    processor_container_.graph_.Reset ();
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout & layouts) const
{
    auto main_output_channel_set = layouts.getMainOutputChannelSet ();
    auto main_input_channel_set = layouts.getMainInputChannelSet ();

    if (main_output_channel_set != juce::AudioChannelSet::mono () &&
        main_output_channel_set != juce::AudioChannelSet::stereo () &&
        main_output_channel_set != juce::AudioChannelSet::ambisonic (1) &&
        main_output_channel_set != juce::AudioChannelSet::quadraphonic ())
        return false;

    if (main_output_channel_set != main_input_channel_set &&
        main_input_channel_set != juce::AudioChannelSet::mono ())
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
        buffer.addFrom (i, 0, buffer, 0, 0, buffer.getNumSamples ());

    const auto num_channels = juce::jmax (total_num_input_channels, total_num_output_channels);

    auto process_block =
        juce::dsp::AudioBlock<float> (buffer).getSubsetChannelBlock (0, (size_t) num_channels);
    juce::dsp::ProcessContextReplacing<float> process_context (process_block);

    processor_container_.command_queue_.Service ();
    processor_container_.graph_.Process (process_context);
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
    auto & ir_selection = processor_container_.load_controller_.GetCurrentIr ();
    json data = ir_selection;
    auto dump = data.dump ();

    auto & parameters = processor_container_.parameter_tree_state_;
    auto state = parameters.copyState ();
    std::unique_ptr<juce::XmlElement> xml (state.createXml ());
    xml->setAttribute ("selection", dump);
    copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void * data, int sizeInBytes)
{
    auto & parameters = processor_container_.parameter_tree_state_;
    std::unique_ptr<juce::XmlElement> xml_state (getXmlFromBinary (data, sizeInBytes));
    if (xml_state.get () != nullptr)
        if (xml_state->hasTagName (parameters.state.getType ()))
            parameters.replaceState (juce::ValueTree::fromXml (*xml_state));

    auto selection_data = xml_state->getStringAttribute ("selection");
    std::optional<IrSelection> ir_selection;
    try
    {
        json::parse (selection_data.toStdString ()).get_to (ir_selection);
    }
    catch (...)
    {
        jassertfalse;
    }

    if (ir_selection.has_value ())
        processor_container_.load_controller_.Load (*ir_selection, [] (bool on_load) {});
}

juce::AudioProcessor * JUCE_CALLTYPE createPluginFilter ()
{
    return new AudioPluginAudioProcessor ();
}