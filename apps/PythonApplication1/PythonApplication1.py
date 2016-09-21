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
            
    for i in range(5):
        speed = ctypes.c_int32(0)
        result = indralib.get_speed(indraref, ctypes.byref(speed), ctypes.byref(indra_error))
        check_result(result)
        print(speed.value, "rpm")


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
    result = indralib.speedcontrol_init(indraref, 10000, 1000, ctypes.byref(indra_error))
    check_result(result)

    # Set speed
    result = indralib.speedcontrol_write(indraref, 100, 10000, ctypes.byref(indra_error))
    check_result(result)

    input("(Press any key to stop the drive...)")

    # Clear speed
    result = indralib.speedcontrol_write(indraref, 0, 10000, ctypes.byref(indra_error))
    check_result(result)



    # Closing communication channel
    result = indralib.close(indraref, ctypes.byref(indra_error))
    check_result(result)

    return 0


if __name__ == "__main__":
    sys.exit(int(main() or 0))