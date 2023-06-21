#include "IrReader.h"

const std::string IrReader::kMetadataExtension = ".zone_meta";
const std::string IrReader::kImpulseResponseExtension = ".wav";

std::filesystem::path IrReader::GetMetadataFileNameForIdentifier (const std::string & ir_identifier)
{
    return ir_identifier + kMetadataExtension;
}

std::filesystem::path
IrReader::GetImpulseResponseFileNameForIdentifier (const std::string & ir_identifier)
{
    return ir_identifier + kImpulseResponseExtension;
}

void IrReader::ReadIrData (const std::filesystem::path & load_path,
                           const std::string & ir_identifier,
                           IrData & ir_data)
{
    ReadIr (load_path, ir_identifier, ir_data.impulse_response);
}
void IrReader::ReadIr (const std::filesystem::path & load_path,
                       const std::string & ir_identifier,
                       juce::AudioBuffer<float> & ir_buffer)
{
    ReadAudioFileToBuffer (load_path / GetImpulseResponseFileNameForIdentifier (ir_identifier),
                           ir_buffer);
}
IrMetadata IrReader::ReadIrMetadata (const std::filesystem::path & load_path,
                                     const std::string & ir_identifier)
{
    IrMetadata irMetadata;
    return irMetadata;
}
void IrReader::ReadAudioFileToBuffer (const std::filesystem::path & audio_path,
                                      juce::AudioBuffer<float> & audio_buffer)
{
    juce::AudioFormatManager audioFormatManager;
    audioFormatManager.registerBasicFormats ();

    std::unique_ptr<juce::AudioFormatReader> reader (
        audioFormatManager.createReaderFor (juce::File (audio_path.string ())));

    audio_buffer.setSize (reader->numChannels, reader->lengthInSamples);
    reader->read (&audio_buffer, 0, reader->lengthInSamples, 0, true, true);
}
