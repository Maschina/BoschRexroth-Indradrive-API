# Indradrive API
###### A Multi-purpose Dynamic Library for **Bosch Rexroth Indradrive M** (MPB-04 Firmware) based on **SIS protocol**

## Introduction

The Indradrive API provides an universal programming interface to the Indradrive M devices. A dedicated DLL (IndradriveAPI.dll, or IndradriveAPI-LV.dll for LabVIEW) handles the user inputs and converts them to SIS protocol telegrams. These telegrams are transfered to the Indradrive device via RS232 interface (refer to Indradrive User's Manual for more information). The API uses the reply telegram to extract the required data or identifies potentials errors and provides it back to the user.


## Compilation

The API is built for native programming languages such as Python or C#. However, a dedicated LabView variant can be also built, which utilizes specific memory allocation methods needed when the library is used in LabView.

1. Install Visual Studio 2017, or later (alternatively, install Visual Studio 2015 Express for Desktop)
2. Fetch the source code repository
3.  If you have LabVIEW installed on your computer, paste the following code into your `UserDirectories.props` file (and adjust the cintools folder to your LabVIEW version) 
```
<?xml version="1.0" encoding="utf-8"?>
<Project ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
<ImportGroup Label="PropertySheets" />
<PropertyGroup Label="UserMacros" />
<PropertyGroup />
<ItemDefinitionGroup>
<ClCompile>
<AdditionalIncludeDirectories>C:\Program Files\National Instruments\LabVIEW 2015\cintools;sis;serial;..\..\sis;..\..\serial;..\..;..;.;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
</ClCompile>
<Link>
<AdditionalLibraryDirectories>C:\Program Files\National Instruments\LabVIEW 2015\cintools;serial;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
<AdditionalDependencies>%(AdditionalDependencies)</AdditionalDependencies>
</Link>
</ItemDefinitionGroup>
<ItemGroup />
</Project>
```
4. Open the Visual Studio solution called Indradrive.sln
5. Choose configuration "Release" or "ReleaseLabview" (for LabView specific build)
6. Build the solution (For "Release", the final DLLs are located in the bin/ folder, for "ReleaseLabview", the final DLL are located in the ../ folder)


## Installation 

The API package consists of:
* IndradriveAPI.dll, or IndradriveAPI-LV.dll (for LabVIEW)
* msvcp140.dll
* vcruntime140.dll

As soon as IndradriveAPI.dll or IndradriveAPI-LV.dll has been compiled, copy all DLLs stated above into your binary folder, where your target application will be started from.

Examples how the bind in the library are provided for both Python and C#.
