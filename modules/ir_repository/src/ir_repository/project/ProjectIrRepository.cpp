#include "ProjectIrRepository.h"
ProjectIrRepository::ProjectIrRepository (lager::reader<ProjectIrRepositoryModel> reader,
                                          lager::context<ProjectIrRepositoryAction> context)
{
}

void ProjectIrRepository::LinkProjectPath (const std::filesystem::path & project_path)
{
}

std::filesystem::path ProjectIrRepository::GetProjectPath ()
{
    return std::filesystem::path ();
}

void ProjectIrRepository::LoadNewProjectIr (const IrMetadata & ir_metadata,
                                            const IrData & ir_data,
                                            ProjectIrRepository::LoadNewProjectIrCallback callback)
{
}

IrMetadata ProjectIrRepository::LoadIrMetaData (const std::string & ir_identifier)
{
    return IrMetadata ();
}

void ProjectIrRepository::LoadIrData (const std::string & ir_identifier, IrData & ir_data)
{
}

bool ProjectIrRepository::IsPathValidDirectory (const std::filesystem::path & path)
{
    return false;
}
