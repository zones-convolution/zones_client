#include "IrReader.h"

void IrReader::ReadIrData (const std::filesystem::path & load_path,
                           const std::string & ir_identifier,
                           IrData & ir_data)
{
    ReadIr (load_path, ir_identifier, ir_data.impulse_response);
    ir_data.metadata = ReadIrMetadata (load_path, ir_identifier);
}

void IrReader::ReadIr (const std::filesystem::path & load_path,
                       const std::string & ir_identifier,
                       juce::AudioBuffer<float> & ir_buffer)
{
    ReadAudioFileToBuffer (
        load_path / IrDataFormat::GetImpulseResponseFileNameForIdentifier (ir_identifier),
        ir_buffer);
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

void IrReader::ReadAudioFileToBuffer (const std::filesystem::path & audio_path,
                                      juce::AudioBuffer<float> & audio_buffer)
{
    juce::AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats ();

    auto ir_file = juce::File (audio_path.string ());

    if (! ir_file.existsAsFile ())
        throw NoIrFileException {};

    std::unique_ptr<juce::AudioFormatReader> reader (audioFormatManager.createReaderFor (ir_file));
    if (reader.get () == nullptr)
        throw FailedToReadIrException {};

    audio_buffer.setSize (reader->numChannels, reader->lengthInSamples);

    reader->read (&audio_buffer, 0, reader->lengthInSamples, 0, true, true);
}
