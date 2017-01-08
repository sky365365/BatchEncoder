///////////////////////////////////////////////////////////////////////////////
// USINGS
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;

///////////////////////////////////////////////////////////////////////////////
// ARGUMENTS
///////////////////////////////////////////////////////////////////////////////

var target = Argument("target", "Default");

///////////////////////////////////////////////////////////////////////////////
// SETTINGS
///////////////////////////////////////////////////////////////////////////////

var platforms = new [] { "Win32", "x64" }.ToList();
var configurations = new [] { "Debug", "Release" }.ToList();
var solution = "./BatchEncoder.sln";
var versionHeaderPath = (FilePath)File("./src/version.h");
var installerScript = MakeAbsolute((FilePath)File("SetupScript.iss"));
var installerScriptAMD64 = MakeAbsolute((FilePath)File("SetupScriptAMD64.iss"));
var iscc = @"C:/Program Files (x86)/Inno Setup 5/ISCC.exe";
var artifactsDir = (DirectoryPath)Directory("./artifacts");
var binDir = (DirectoryPath)Directory("./src/bin");
var objDir = (DirectoryPath)Directory("./src/obj");

///////////////////////////////////////////////////////////////////////////////
// VERSION
///////////////////////////////////////////////////////////////////////////////

var text = System.IO.File.ReadAllText(versionHeaderPath.FullPath);
var split = text.Split(new char [] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
var major = split[0].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var minor = split[1].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var revision = split[2].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var build = split[3].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var version = major + "." + minor;

Information("Defined Version: {0}.{1}.{2}.{3}", major, minor, revision, build);
Information("Release Version: {0}", version);

///////////////////////////////////////////////////////////////////////////////
// TASKS
///////////////////////////////////////////////////////////////////////////////

Task("Clean")
    .Does(() =>
{
    CleanDirectory(artifactsDir);
    CleanDirectory(binDir);
    CleanDirectory(objDir);
});

Task("Build")
    .IsDependentOn("Clean")
    .Does(() =>
{
    configurations.ForEach(configuration => {
        platforms.ForEach(platform => {
            Information("Building: {0}, {1} / {2}", solution, configuration, platform);
            MSBuild(solution, settings => {
                settings.SetConfiguration(configuration);
                settings.WithProperty("Platform", "\"" + platform + "\"");
                settings.SetVerbosity(Verbosity.Minimal);
            });
        });
    });
});

Task("Package-Binaries-x86")
    .IsDependentOn("Build")
    .Does(() =>
{
    var output = "BatchEncoder-" + version;
    var outputDir = artifactsDir.Combine(output);
    var outputZip = artifactsDir.CombineWithFilePath(output + ".zip");
    var exeFile = File("./src/bin/Release/Win32/BatchEncoder.exe");
    var enginesFile = File("./engines/unicode/Win32/BatchEncoder.engines");
    var portableFile = File("BatchEncoder.portable");

    CleanDirectory(outputDir);

    CopyFileToDirectory(File("README.md"), outputDir);
    CopyFileToDirectory(File("CHANGELOG.md"), outputDir);
    CopyFileToDirectory(File("COPYING.TXT"), outputDir);
    CopyFileToDirectory(exeFile, outputDir);
    CopyFileToDirectory(enginesFile, outputDir);
    CopyFileToDirectory(portableFile, outputDir);

    CleanDirectory(langDir);
    CopyFiles("./lang/*.txt", langDir);

    Zip(outputDir, outputZip);
});

Task("Package-Binaries-AMD64")
    .IsDependentOn("Build")
    .Does(() =>
{
    var output = "BatchEncoder-" + version + "-AMD64";
    var outputDir = artifactsDir.Combine(output);
    var outputZip = artifactsDir.CombineWithFilePath(output + ".zip");
    var exeFile = File("./src/bin/Release/x64/BatchEncoder.exe");
    var enginesFile = File("./engines/unicode/Win64/BatchEncoder.engines");
    var portableFile = File("BatchEncoder.portable");

    CleanDirectory(outputDir);

    CopyFileToDirectory(File("README.md"), outputDir);
    CopyFileToDirectory(File("CHANGELOG.md"), outputDir);
    CopyFileToDirectory(File("COPYING.TXT"), outputDir);
    CopyFileToDirectory(exeFile, outputDir);
    CopyFileToDirectory(enginesFile, outputDir);
    CopyFileToDirectory(portableFile, outputDir);

    CleanDirectory(langDir);
    CopyFiles("./lang/*.txt", langDir);

    Zip(outputDir, outputZip);
});

Task("Package-Installer-x86")
    .IsDependentOn("Build")
    .Does(() =>
{
    StartProcess(iscc, new ProcessSettings { 
        Arguments = "\"" + installerScript.FullPath + "\"" + " " + "/DVERSION=" + version, 
        WorkingDirectory = MakeAbsolute(artifactsDir) });
});

Task("Package-Installer-AMD64")
    .IsDependentOn("Build")
    .Does(() =>
{
    StartProcess(iscc, new ProcessSettings { 
        Arguments = "\"" + installerScriptAMD64.FullPath + "\"" + " " + "/DVERSION=" + version, 
        WorkingDirectory = MakeAbsolute(artifactsDir) });
});

///////////////////////////////////////////////////////////////////////////////
// TARGETS
///////////////////////////////////////////////////////////////////////////////

Task("Package")
  .IsDependentOn("Package-Binaries-x86")
  .IsDependentOn("Package-Binaries-AMD64")
  .IsDependentOn("Package-Installer-x86")
  .IsDependentOn("Package-Installer-AMD64");

Task("Default")
  .IsDependentOn("Package");

///////////////////////////////////////////////////////////////////////////////
// EXECUTE
///////////////////////////////////////////////////////////////////////////////

RunTarget(target);
