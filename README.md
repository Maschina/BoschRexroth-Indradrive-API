# **Bosch Rexroth Indradrive API**
###### A Multi-purpose Dynamic Library for Bosch Rexroth Indradrive M (MPB-04 Firmware) based on *SIS protocol*

# Introduction

The Indradrive API provides an universal programming interface to the Indradrive M devices. A dedicated DLL (IndradriveAPI.dll, or IndradriveAPI-LV.dll for LabVIEW) handles the user inputs and converts them to SIS protocol telegrams. These telegrams are transfered to the Indradrive device via RS232 interface (refer to Indradrive User's Manual for more information). The API uses the reply telegram to extract the required data or identifies potentials errors and provides it back to the user.

## Drive modes
The API is designed to support two dedicated drive modes:
* Speed Control
* Sequencer

### Speed Control
The principle of the Speed Control is depicted below: 

![Speed Control Principle](https://github.com/Hokyo/BoschRexroth-Indradrive-API/blob/develop/doc/compile/Indradrive-Drivemodes-SpeedControl.png?raw=true)

Basically, Speed Control offers non-realtime-capable way to quickly setup a new kinematic point (controlled via speed and acceleration).

Based on the requested speed and acceleration, the motor connected to the Indradrive system is cranking or down to the respective kinematic point.

The time between providing the data to the API and reaction of the motor depends on the Operating System (most likely Windows), calculation and creation of the SIS telegram and the baudrate to transfer the telegram. The time to go from the current kinematic point to the requested kinematic point can be determined as the following: 

![t=t_{i+1}-t_i=\frac{v_{target}-v_{current}}{a}](https://latex.codecogs.com/gif.latex?t%3Dt_%7Bi&plus;1%7D-t_i%3D%5Cfrac%7Bv_%7Btarget%7D-v_%7Bcurrent%7D%7D%7Ba%7D)

whereas ![a](https://latex.codecogs.com/gif.latex?%5Cinline%20a) is the acceleration and ![v_{target}-v_{current}](https://latex.codecogs.com/gif.latex?%5Cinline%20v_%7B%5Ctext%7Btarget%7D%7D-v_%7B%5Ctext%7Bcurrent%7D%7D) the difference between current and targeted speed.

##### Remarks
> The Speed Control drive mode cannot be used for real-time applications, since the jitter caused by OS and telegram transmission is unpredictable. Use the Sequencer drive mode for real-time applications instead.

The Speed Control drive mode is properly controlled in the following order:
1. Check the current drive mode by using `get_drivemode()`
   - If drive mode "Sequencer" is selected, proceed like this:
     1. Check, if Indradrive is in _bb_ operation state by using `get_opstate()`
	 2. Call `speedcontrol_activate()`
   - If drive mode "Speed Control" is selected, do not do anything and proceed with the next point
2. Initialize the right units by using `speedcontrol_init()`
3. Write the target kinematic point by using `speedcontrol_write()`

##### Attention
> Speed Control commands the Indradrive to control the next kinematic point. This kinematic operates continuously until the next kinematic point is given or the emergency brake has been used. There is no automatic or time-limited stop system implemented.


### Sequencer
The principle of the Sequencer is depicted below: 

![Sequencer Principle](https://github.com/Hokyo/BoschRexroth-Indradrive-API/blob/develop/doc/compile/Indradrive-Drivemodes-Sequencer.png?raw=true)

Sequencer offers real-time capable operation of a pre-programmed kinematic sequence upon receiving a trigger signal. Thus, Sequencer can be used if operations in a time-critical application is required.

The Sequencer routine is implemented into Indradrive's built-in PLC. If the routine is neither properly programmed nor running, the Sequencer operation mode is not working correctly.

In contrast to Speed Control, the Sequencer will be pre-programmed with a specific kinematic sequence (an example is shown in the figure above). Upon receiving an hardware or a software trigger, the Sequencer routine within the PLC immediately starts operating based on the first given kinematic point. After the pre-programmed elapsed time delay, the next kinematic point will be operated accordingly. As soon as the last kinematic point has been processed, the Indradrive motor goes back into standstill state (stop mode).

##### Attention
> If the PLC routine for the Sequencer is neither properly programmed nor running, the Sequencer drive mode cannot correctly operate.

Planning the kinematic sequence premises some calculations to be done for the jerk, if the delay, speed and acceleration is know for each sequence element. The following formula can be used for calculing the respective jerk, ![r](https://latex.codecogs.com/gif.latex?%5Cinline%20r):

![r_i=\frac{a_i^2}{a_i(t_i-t_{i-1})-v_i}](https://latex.codecogs.com/gif.latex?r_i%3D%5Cfrac%7Ba_i%5E2%7D%7Ba_i%28t_i-t_%7Bi-1%7D%29-v_i%7D)

whereas ![t_i-t_{i-1}](https://latex.codecogs.com/gif.latex?%5Cinline%20t_i-t_%7Bi-1%7D) is the Delay i to get from the previous kinematic point to the next requested kinematic point, ![a_i](https://latex.codecogs.com/gif.latex?%5Cinline%20a_i) is the acceleration and ![v_i](https://latex.codecogs.com/gif.latex?%5Cinline%20v_i) is the speed. 

The Sequencer drive mode is properly controlled in the following order:
1. Check the current drive mode by using `get_drivemode()`
   - If drive mode "Speed Control" is selected, proceed like this:
     1. Check, if Indradrive is in _"bb"_ operation state by using `get_opstate()`
     2. Call `sequencer_activate()`
   - If drive mode "Sequencer" is selected, do not do anything and proceed with the next point
2. Initialize the right units by using `sequencer_init()`
3. Write the whole kinematic sequence by using `sequencer_write()`
4. Trigger the operation by using `sequencer_softtrigger()`, or use the hardware trigger (refer to Indradrive's User's Manual)


## API Modules

Module | Description
------ | -----------
Fundamentals | Provides functions for communication establishment
Status | Get information for diagnostic, drive modes, operation states, or even actual speed information
Configuration | Setting up essential required configurations 
Sequencer | Programming functions for "Sequencer" drive mode 
Speed Control | Programming functions for "Speed Control" drive mode 


# Building

[![Build status](https://ci.appveyor.com/api/projects/status/vhys1a4jj7ilklwa?svg=true)](https://ci.appveyor.com/project/Hokyo/boschrexroth-indradrive-api)

The API is built for native programming languages such as Python or C#. However, a dedicated LabView variant can be also built, which utilizes specific memory allocation methods needed when the library is used in LabView.

1. Install Visual Studio 2017, or later (alternatively, install Visual Studio 2015 Express for Desktop)
2. Install Python 3.3 or higher, and make sure that python.exe is provided in the environment variables
3. Install git 1.9.5 or higher, and make sure that python.exe is provided in the environment variables
4. Fetch the source code repository
5. If you have LabVIEW installed on your computer, adjust the cintools folder to your LabVIEW version and adapt the `IndradriveAPIDefaults.props` as below:

```xml
		...
		<ClCompile>
			<AdditionalIncludeDirectories>C:\Program Files\National Instruments\LabVIEW 2015\cintools;sis;serial;..\..\sis;..\..\serial;..\..;..;.;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
		</ClCompile>
		...
		<Link>
			<AdditionalLibraryDirectories>C:\Program Files\National Instruments\LabVIEW 2015\cintools;serial;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
			<AdditionalDependencies>%(AdditionalDependencies)</AdditionalDependencies>
		</Link>
		...
```
6. Open the Visual Studio solution called Indradrive.sln
7. Choose configuration "Release" or "ReleaseLabview" (for LabView specific build)
8. Build the solution using the respective solution configuration
   - "Release": Final DLLs are located in the bin/ folder
   - "ReleaseLabview": Final DLLs are located in the ../ folder


# Installation 

First, download the [most recent release files](https://github.com/Hokyo/BoschRexroth-Indradrive-API/releases/latest), or building them by yourself as described above.

Second, install [Microsoft Visual C++ Redistributable](https://support.microsoft.com/de-de/help/2977003/the-latest-supported-visual-c-downloads), if Visual Studio is not installed or an Operating System lower than Windows 10 is used. 
In that case, please use *Microsoft Visual C++ Redistributable for Visual Studio 2017*.

Copy the IndradriveAPI.dll into your binary folder, where your target application will be started from.

Examples how the bind in the library are provided for both Python and C#.


# Usage

The following tables provides an overview of exported functions that can be accessed through the API DLL:


Module | API function | Brief description 
------ | ------------ | -----------------
Fundamentals | `init()` | Creates API reference.  
Fundamentals | `open()` | Opens the communication port to the Indradrive device.  
Fundamentals | `close()` | Closes the communication port at the Indradrive device.  
Sequencer | `sequencer_activate()` | Activates the drive mode "Sequencer".  
Sequencer | `sequencer_init()` | Initializes limits and sets the right scaling/unit factors for operation of "Sequencer" drive mode.  
Sequencer | `sequencer_write()` | Writes the whole run sequence into the device.  
Sequencer | `sequencer_softtrigger()` | Software-Trigger to start operation of the "Sequencer" drive mode.  
Sequencer | `sequencer_hardtrigger()` | Hardware-Trigger to start operation of the "Sequencer" drive mode.  
Sequencer | `sequencer_getstatus()` | Get the status of the "Sequencer" drive mode.  
Speed Control | `speedcontrol_activate()` | Activates the drive mode "Speed Control".  
Speed Control | `speedcontrol_init()` | Initializes limits and sets the right scaling/unit factors for operation of "Speed Control" drive mode.  
Speed Control | `speedcontrol_write()` | Writes the current kinematic (speed and acceleration) into the device.  
Configuration | `set_stdenvironment()` | Sets the proper unit and language environment.  
Status | `get_drivemode()` | Retrieve information about the drive mode: Speed Control or Sequencer.  
Status | `get_opstate()` | Retrieve information about the operation states: bb, Ab, or AF.  
Status | `get_speed()` | Gets the actual rotation speed.  
Status | `get_diagnostic_msg()` | Gets diagnostic message string of the current Indradrive status.  
Status | `get_diagnostic_num()` | Gets diagnostic number of the current Indradrive status.  
Status | `clear_error()` | Clears a latched error in the Indradrive device 


# Examples

This sections gives examples for C# and Python how to use to library. However, through the nature of DLL, the API can be also called by other programming languages and development environments, such as LabVIEW, Matlab, etc.


## C# Example

The following code defines a C# class than can be copied in into a seperated .cs file. The Indradrive is accessible within the WpfApplication1 namespace (or whatever namespace you are writing). 

```csharp
using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Controls;

namespace WpfApplication1
{
    public class Indradrive
    {
        [StructLayout(LayoutKind.Sequential)]
        public unsafe struct ErrHandle
        {
            [MarshalAs(UnmanagedType.U4)]
            public UInt32 code;
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 2048)]
            public byte[] msg;
        }

        private int idref;
        private const string dllpath = "..\\..\\..\\..\\bin\\IndradriveAPI.dll";

        private ErrHandle indraerr;
        private ListBox listboxerr;

        public Indradrive(ref ListBox listbox)
        {
            listboxerr = listbox;
            idref = init();
        }


        // Fundamentals

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int init();

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int open(int ID_ref, Byte[] ID_comport, UInt32 ID_combaudrate, ref ErrHandle ID_err);
        public int open(Byte[] ID_comport, UInt32 ID_combaudrate) { return CheckResult(open(idref, ID_comport, ID_combaudrate, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int close(int ID_ref, ref ErrHandle ID_err);
        public int close() { return CheckResult(close(idref, ref indraerr)); }
       
        
        // Speed Control

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int speedcontrol_activate(int ID_ref, ref ErrHandle ID_err);
        public int speedcontrol_activate() { return CheckResult(speedcontrol_activate(idref, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int speedcontrol_init(int ID_ref, Double ID_max_accel, Double ID_max_jerk, ref ErrHandle ID_err);
        public int speedcontrol_init(Double ID_max_accel, Double ID_max_jerk) { return CheckResult(speedcontrol_init(idref, ID_max_accel, ID_max_jerk, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int speedcontrol_write(int ID_ref, Double ID_speed, Double ID_accel, ref ErrHandle ID_err);
        public int speedcontrol_write(Double ID_speed, Double ID_accel) { return CheckResult(speedcontrol_write(idref, ID_speed, ID_accel, ref indraerr)); }


        // Sequencer

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int sequencer_activate(int ID_ref, ref ErrHandle ID_err);
        public int sequencer_activate() { return CheckResult(sequencer_activate(idref, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int sequencer_init(int ID_ref, Double ID_max_accel, Double ID_max_jerk, ref ErrHandle ID_err);
        public int sequencer_init(Double ID_max_accel, Double ID_max_jerk) { return CheckResult(sequencer_init(idref, ID_max_accel, ID_max_jerk, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int sequencer_write(int ID_ref, Double[] ID_speeds, Double[] ID_accels, Double[] ID_jerks, UInt32[] ID_delays, UInt16 ID_set_length, ref ErrHandle ID_err);
        public int sequencer_write(Double[] ID_speeds, Double[] ID_accels, Double[] ID_jerks, UInt32[] ID_delays, UInt16 ID_set_length) { return CheckResult(sequencer_write(idref, ID_speeds, ID_accels, ID_jerks, ID_delays, ID_set_length, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int sequencer_softtrigger(int ID_ref, ref ErrHandle ID_err);
        public int sequencer_softtrigger() { return CheckResult(sequencer_softtrigger(idref, ref indraerr)); }


        // Status

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_drivemode(int ID_ref, ref UInt32 mode, ref ErrHandle ID_err);
        public int get_drivemode(ref UInt32 mode) { return CheckResult(get_drivemode(idref, ref mode, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_opstate(int ID_ref, ref Byte state, ref ErrHandle ID_err);
        public int get_opstate(ref Byte state) { return CheckResult(get_opstate(idref, ref state, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_speed(int ID_ref, ref Double speed, ref ErrHandle ID_err);
        public int get_speed(ref Double speed) { return CheckResult(get_speed(idref, ref speed, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_diagnostic_msg(int ID_ref, Byte[] ID_diagnostic_msg, ref ErrHandle ID_err);
        public int get_diagnostic_msg(Byte[] ID_diagnostic_msg) { return CheckResult(get_diagnostic_msg(idref, ID_diagnostic_msg, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int get_diagnostic_num(int ID_ref, ref UInt32 ID_diagnostic_num, ref ErrHandle ID_err);
        public int get_diagnostic_num(ref UInt32 ID_diagnostic_num) { return CheckResult(get_diagnostic_num(idref, ref ID_diagnostic_num, ref indraerr)); }

        [DllImport(dllpath, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private static extern int clear_error(int ID_ref, ref ErrHandle ID_err);
        public int clear_error() { return CheckResult(clear_error(idref, ref indraerr)); }


        // Helpers

        public int CheckResult(int ret)
        {
            if (ret != 0)
            {
                String err = Encoding.ASCII.GetString(indraerr.msg).TrimEnd((Char)0);

                Console.WriteLine(err);
                listboxerr.Dispatcher.BeginInvoke((System.Windows.Forms.MethodInvoker)(() =>
                {
                    listboxerr.Items.Add(err);
                }));
            }

            return ret;
        }
    }
}
```

## Python Example

```python
import sys
import ctypes
from ctypes import cdll
import os

# Minimum Python 3.3 required
assert sys.version_info >= (3,3)


# Load Indradrive API DLL into memory (use absolute or relative path for 'libpath')
libpath = os.path.dirname(__file__) + "\\..\\..\\bin\\IndradriveAPI.dll"
indralib = cdll.LoadLibrary(libpath)

# Error-specific class
class ERR(ctypes.Structure):
    _fields_ = [("code", ctypes.c_int32),("msg", ctypes.c_char * 2048)]
    
    def get_msg_str(self):
        return str(self.msg, "UTF-8")

indra_error = ERR(0)


def check_result(result):
    if result: 
        print("Error occurred: " + indra_error.get_msg_str())
        sys.exit(result)

def get_bit(byteval, idx):
    return ((byteval&(1<<idx))!=0);


# MAIN ENTRY POINT
def main():
    # Getting API reference
    indraref = indralib.init()

    # Opening communication channel
    result = indralib.open(indraref, b"COM1", 19200, ctypes.byref(indra_error))
    check_result(result)

    # Set standard environment
    result = indralib.set_stdenvironment(indraref, ctypes.byref(indra_error))
    check_result(result)


    # 
    # Check Drive Mode
    # 
    drvmode = ctypes.c_uint32(0)
    result = indralib.get_drivemode(indraref, ctypes.byref(drvmode), ctypes.byref(indra_error))
    check_result(result)

    if drvmode.value != 2: # Drive Mode is not "Speed Control" -> Change it
        input("Please make sure to DISABLE the drive release before continue (stand-by mode)!\n(Press any key to continue...)")

        # Activate Speed Control
        result = indralib.speedcontrol_activate(indraref, ctypes.byref(indra_error))
        check_result(result) 
            
    # Diagnostic message
    diagmsg = ctypes.create_string_buffer(256)
    result = indralib.get_diagnostic_msg(indraref, diagmsg, ctypes.byref(indra_error))
    check_result(result)
    print("Current status:\n" + diagmsg.raw.decode('ascii'))


    # 
    # Check Operation State
    # 
    while True:
        opstate = ctypes.c_uint8(0)
        result = indralib.get_opstate(indraref, ctypes.byref(opstate), ctypes.byref(indra_error))
        check_result(result)

        if (opstate.value & 0b11) != 0b11:
            input("Please make sure to RELEASE before continue (torque-controlled operation mode)!\n(Press any key to continue...)")
        else:
            break


    # Set limits
    result = indralib.speedcontrol_init(indraref, ctypes.c_double(10000), ctypes.c_double(1000), ctypes.byref(indra_error))
    check_result(result)

    while True:
        speed_str = input("Speed [rpm] = ?")
        if (speed_str == ""): break

        # Set speed
        speed = int(speed_str)
        result = indralib.speedcontrol_write(indraref, ctypes.c_double(speed), ctypes.c_double(10), ctypes.byref(indra_error))
        check_result(result)



    # Closing communication channel
    result = indralib.close(indraref, ctypes.byref(indra_error))
    check_result(result)

    return 0


if __name__ == "__main__":
    sys.exit(int(main() or 0))
```
