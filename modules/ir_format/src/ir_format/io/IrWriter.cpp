#include "IrWriter.h"

void IrWriter::WriteIrMetadata (const std::filesystem::path & write_path,
                                const std::string & ir_identifier,
                                const IrMetadata & ir_metadata)
{
    auto file_path = write_path / IrDataFormat::GetMetadataFileNameForIdentifier (ir_identifier);
    auto write_file = juce::File (file_path.string ());
    if (! write_file.getParentDirectory ().exists ())
        throw DirectoryDoesNotExistException {};

    auto output_stream = write_file.createOutputStream ();
    IrMetadata::ToDynamic (ir_metadata).writeAsJSON (*output_stream, {});
}

void IrWriter::WriteIrData (const std::filesystem::path & write_path,
                            const std::string & ir_identifier,
                            const IrData & ir_data)
{
    auto file_path =
        write_path / IrDataFormat::GetImpulseResponseFileNameForIdentifier (ir_identifier);
    auto write_file = juce::File (file_path.string ());
    if (! write_file.getParentDirectory ().exists ())
        throw DirectoryDoesNotExistException {};

    juce::WavAudioFormat wav_format;
    std::unique_ptr<juce::AudioFormatWriter> writer;
    writer.reset (wav_format.createWriterFor (new juce::FileOutputStream (write_file),
                                              ir_data.sample_rate,
                                              ir_data.buffer.getNumChannels (),
                                              ir_data.bit_depth,
                                              {},
                                              0));

    if (writer != nullptr)
        writer->writeFromAudioSampleBuffer (ir_data.buffer, 0, ir_data.buffer.getNumSamples ());
}