#define Version Trim(FileRead(FileOpen("..\VERSION")))
#define ProjectName GetEnv('PROJECT_NAME')
#define ProductName GetEnv('PRODUCT_NAME')
#define Publisher GetEnv('COMPANY_NAME')
#define Year GetDateTimeString("yyyy","","")

[Components]
Name: "vst3"; Description: "VST3 plugin";

[Setup]
ArchitecturesInstallIn64BitMode=x64compatible
ArchitecturesAllowed=x64compatible
AppName={#ProductName}
OutputBaseFilename={#ProductName}-{#Version}-Windows
AppCopyright=Copyright (C) {#Year} {#Publisher}
AppPublisher={#Publisher}
AppVersion={#Version}
DefaultDirName="{commoncf64}\VST3\{#ProductName}.vst3"
DisableDirPage=yes

LicenseFile="EULA"
UninstallFilesDir="{commonappdata}\{#ProductName}\uninstall"

[UninstallDelete]
Type: filesandordirs; Name: "{commoncf64}\VST3\{#ProductName}Data"

[Files]
Source: "builds\x64-msvc\client\{#ProjectName}_artefacts\Release\VST3\{#ProductName}.vst3\*"; DestDir: "{commoncf64}\VST3\{#ProductName}.vst3\"; Excludes: *.ilk; Flags: ignoreversion recursesubdirs; Components: vst3

[Icons]
Name: "{autoprograms}\Uninstall {#ProductName}"; Filename: "{uninstallexe}"

[Run]
Filename: "{cmd}"; \
    WorkingDir: "{commoncf64}\VST3"; \
    Parameters: "/C mklink /D ""{commoncf64}\VST3\{#ProductName}Data"" ""{commonappdata}\{#ProductName}"""; \
    Flags: runascurrentuser; Components: vst3