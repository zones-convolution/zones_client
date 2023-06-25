#include "IrReader.h"

void IrReader::ReadIrData (const std::filesystem::path & load_path,
                           const std::string & ir_identifier,
                           IrData & ir_data)
{
    ReadAudioFileToIrData (
        load_path / IrDataFormat::GetImpulseResponseFileNameForIdentifier (ir_identifier), ir_data);
}

static juce::DynamicObject ReadJsonFileToDynamic (const juce::File & json_file)
{
    auto metadata_string = json_file.loadFileAsString ();
    auto metadata_var = juce::JSON::parse (metadata_string);
    return *metadata_var.getDynamicObject ();
}

IrMetadata IrReader::ReadIrMetadata (const std::filesystem::path & load_path,
                                     const std::string & ir_identifier)
{
    auto absolute_path = load_path / IrDataFormat::GetMetadataFileNameForIdentifier (ir_identifier);
    juce::File metadata_file (absolute_path.string ());

    if (! metadata_file.existsAsFile ())
        throw NoMetadataFileException {};

    return IrMetadata::FromDynamic (ReadJsonFileToDynamic (metadata_file));
}

void IrReader::ReadAudioFileToIrData (const std::filesystem::path & audio_path, IrData & ir_data)
{
    juce::AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats ();

    auto ir_file = juce::File (audio_path.string ());

    if (! ir_file.existsAsFile ())
        throw NoIrFileException {};

    std::unique_ptr<juce::AudioFormatReader> reader (audioFormatManager.createReaderFor (ir_file));
    if (reader.get () == nullptr)
        throw FailedToReadIrException {};

    ir_data.sample_rate = reader->sampleRate;
    ir_data.bit_depth = reader->bitsPerSample;

    ir_data.buffer.setSize (reader->numChannels, reader->lengthInSamples);
    reader->read (&ir_data.buffer, 0, reader->lengthInSamples, 0, true, true);
}
