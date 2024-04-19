#include "RefreshUserIrsEffect.h"

#include "ir_format/io/IrReader.h"

void RefreshUserIrsEffect (const RefreshUserIrsAction & action, const IrRepositoryContext & context)
{
    if (action.search_paths.empty ())
    {
        context.dispatch (RefreshUserIrsResultAction {});
        return;
    }

    context.loop ().async (
        [action, context]
        {
            RefreshUserIrsResultAction result;
            IrReader ir_reader;

            for (auto & search_path : action.search_paths)
            {
                auto search_directory = juce::File (search_path.string ());

                auto ir_directories = search_directory.findChildFiles (
                    juce::File::TypesOfFileToFind::findDirectories, false);

                for (const auto & ir_directory : ir_directories)
                {
                    auto ir_path =
                        std::filesystem::path (ir_directory.getFullPathName ().toStdString ());

                    try
                    {
                        auto ir_metadata = ir_reader.ReadIrMetadata (ir_path);
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
