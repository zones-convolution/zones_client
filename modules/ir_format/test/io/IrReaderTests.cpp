#include "ir_format/io/IrReader.h"
#include "ir_format/io/IrWriter.h"

#include <catch2/catch_test_macros.hpp>
static std::filesystem::path kTestDataDirectory = TEST_DATA_DIRECTORY;

static const std::string kValidIrIdentifier = "valid_ir";
static const std::string kNonExistentIr = "this_does_not_exist";
static const std::string kInvalidIrFile = "invalid_ir";

TEST_CASE ("ir_reader can read from disk", "[IrReader]")
{
    IrReader ir_reader;

    SECTION ("reads valid metadata")
    {
        auto ir_metadata = ir_reader.ReadIrMetadata (kTestDataDirectory / kValidIrIdentifier);

        REQUIRE (ir_metadata.name == "valid_name");
        REQUIRE (ir_metadata.description == "valid_description");
        REQUIRE (ir_metadata.channel_format == ChannelFormat::kStereo);

        REQUIRE (ir_metadata.position_map->centre == "centre");
        REQUIRE (ir_metadata.position_map->left == "left");
        REQUIRE (ir_metadata.position_map->right == "right");
    }

    SECTION ("throws NoMetadataFileException when metadata file does not exist")
    {
        REQUIRE_THROWS_AS (ir_reader.ReadIrMetadata (kTestDataDirectory / kNonExistentIr),
                           IrReader::NoMetadataFileException);
    }

    SECTION ("reads valid ir")
    {
        IrData ir_data;
        ir_reader.ReadIrData (kTestDataDirectory, kValidIrIdentifier, ir_data);

        REQUIRE (ir_data.buffer.getNumChannels () > 0);
        REQUIRE (ir_data.buffer.getNumSamples () == 288000);

        REQUIRE (ir_data.bit_depth == 24);
        REQUIRE (ir_data.sample_rate == 96000);
    }

    SECTION ("throws NoIrFileException when Ir file does not exist")
    {
        IrData ir_data;
        REQUIRE_THROWS_AS (ir_reader.ReadIrData (kTestDataDirectory, kNonExistentIr, ir_data),
                           IrReader::NoIrFileException);
    }

    SECTION ("throws FailedToReadIrException when Wav file is invalid")
    {
        IrData ir_data;
        REQUIRE_THROWS_AS (ir_reader.ReadIrData (kTestDataDirectory, kInvalidIrFile, ir_data),
                           IrReader::FailedToReadIrException);
    }
}

// SCENARIO ("finds irs from project directory")
//{
//    GIVEN ("a project directory with multiple irmetadata files")
//    {
//        IrWriter ir_writer;
//        auto temp_dir =
//            juce::File::getSpecialLocation (juce::File::SpecialLocationType::tempDirectory)
//                .getFullPathName ()
//                .toStdString ();
//
//        static constexpr int kNumberOfIrs = 10;
//        std::vector<IrMetadata> ir_metadatas;
//        for (auto i = 0; i < kNumberOfIrs; ++i)
//        {
//            auto ir_identifier = juce::Uuid ().toString ().toStdString ();
//            IrMetadata ir_metadata {.name = ir_identifier, .description = std::to_string (i)};
//            ir_metadatas.push_back (ir_metadata);
//            ir_writer.WriteIrMetadata (temp_dir, ir_identifier, ir_metadata);
//        }
//
//        WHEN ("getting irs in the project directory")
//        {
//            IrReader ir_reader;
//            auto project_data = ir_reader.GetIrsInPath (temp_dir);
//
//            THEN ("the correct number of irs are found")
//            {
//                REQUIRE (project_data.size () == kNumberOfIrs);
//            }
//
//            THEN ("project data matches")
//            {
//                for (const auto & [identifier, data] : project_data)
//                {
//                    auto corresponding_data =
//                        std::find_if (ir_metadatas.begin (),
//                                      ir_metadatas.end (),
//                                      [&identifier = identifier] (const auto & ir_meta)
//                                      { return ir_meta.name == identifier; });
//
//                    REQUIRE (corresponding_data != ir_metadatas.end ());
//                    REQUIRE (corresponding_data->description == data.description);
//                }
//            }
//        }
//
//        juce::File (temp_dir).moveToTrash ();
//    }
//}