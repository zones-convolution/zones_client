#include "RefreshUserIrsEffect.h"

#include "ir_format/io/IrReader.h"

void RefreshUserIrsEffect (const RefreshUserIrsAction & action, const IrRepositoryContext & context)
{
    context.loop ().async (
        [action, context]
        {
            immer::flex_vector<std::filesystem::path> search_paths {
                "/Users/leonps/Documents/development/zones_client/modules/ir_format/test_data"};
            RefreshUserIrsResultAction result;
            IrReader ir_reader;

            for (auto & search_path : search_paths)
            {
                auto search_directory = juce::File (search_path.string ());
                auto ir_paths =
                    search_directory.findChildFiles (juce::File::TypesOfFileToFind::findFiles,
                                                     true,
                                                     "*.json",
                                                     juce::File::FollowSymlinks::no);

                for (const auto & ir_path : ir_paths)
                {
                    try
                    {
                        std::filesystem::path absolute_ir_path =
                            ir_path.getFullPathName ().toStdString ();
                        auto ir_metadata = ir_reader.ReadIrMetadata (absolute_ir_path);
                        ir_metadata.path_attribute = absolute_ir_path.remove_filename ();
                        result.user_irs = result.user_irs.push_back (ir_metadata);
                    }
                    catch (...)
                    {
                    }
                }
            }

            context.dispatch (result);
        });
}
