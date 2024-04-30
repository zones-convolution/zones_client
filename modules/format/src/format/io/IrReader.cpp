#include "IrReader.h"

void IrReader::ReadIrData (const std::filesystem::path & load_path,
                           const std::string & ir_identifier,
                           IrData & ir_data)
{
    ReadAudioFileToIrData (load_path / ir_identifier, ir_data);
}

void IrReader::ReadAudioFileToIrData (const std::filesystem::path & audio_path, IrData & ir_data)
{
    juce::AudioFormatManager audio_format_manager;
    audio_format_manager.registerBasicFormats ();

    auto ir_file = juce::File (audio_path.string ());

    if (! ir_file.existsAsFile ())
        throw NoIrFileException {};

    std::unique_ptr<juce::AudioFormatReader> reader (
        audio_format_manager.createReaderFor (ir_file));
    if (reader.get () == nullptr)
        throw FailedToReadIrException {};

    ir_data.sample_rate = reader->sampleRate;
    ir_data.bit_depth = reader->bitsPerSample;

    ir_data.buffer.setSize (reader->numChannels, reader->lengthInSamples);
    reader->read (&ir_data.buffer, 0, reader->lengthInSamples, 0, true, true);
}
