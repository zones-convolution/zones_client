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

// kept for browser component
IrReader::ProjectData IrReader::GetIrsInPath (const std::filesystem::path & load_path)
{
    auto project_directory = juce::File (load_path.string ());
    auto project_files = project_directory.findChildFiles (
        juce::File::TypesOfFileToFind::findFiles | juce::File::TypesOfFileToFind::ignoreHiddenFiles,
        false,
        "*.json",
        juce::File::FollowSymlinks::no);

    ProjectData project_data;
    for (const auto & file : project_files)
    {
        std::filesystem::path file_path = file.getFullPathName ().toStdString ();
        auto ir_identifier = file_path.stem ();
        project_data = project_data.push_back (
            std::make_pair (ir_identifier, ReadIrMetadata (load_path, ir_identifier)));
    }

    return project_data;
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
