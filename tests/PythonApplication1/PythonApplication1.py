import sys
import ctypes
from ctypes import cdll

# Minimum Python 3.3 required
assert sys.version_info >= (3,3)


# Load Indradrive API DLL into memory (use absolute or relative path for 'libpath')
libpath = "C:\\AE\\Alternator_Testbench\\Software\\API-Hardware\\Indradrive\\Lib\\IndradriveWrapper.dll"
indralib = cdll.LoadLibrary(libpath)

# Error-specific class
class ERR(ctypes.Structure):
    _fields_ = [("code", ctypes.c_int32),("msg", ctypes.c_char * 2048)]
    
    def get_msg_str(self):
        return str(self.msg, "UTF-8")

indra_error = ERR(0)
indra_error_call = ctypes.byref(indra_error)
indra_error_out = indra_error.get_msg_str



# MAIN ENTRY POINT
def main():
    # Getting API reference
    indraref = indralib.init()

    # Opening communication channel
    result = indralib.open(indraref, b"COM1", 19200, indra_error_call)
    if result: 
        print("Error occurred: " + indra_error_out())
        return result



    ans_yn = input("Do you have to explicitly switch to Velocity Control mode? (Y/N):")
    if (ans_yn.lower() == "y"):
        input("Please make sure to DISABLE the drive release before continue (stand-by mode)!\n(Press any key to continue...)")

        # Activate Speed Control
        result = indralib.speedcontrol_activate(indraref, indra_error_call)
        if result: 
            print("Error occurred: " + indra_error_out())
            return result

    input("Please make sure to RELEASE before continue (torque-controlled operation mode)!\n(Press any key to continue...)")
    

    # Set limits
    result = indralib.speedcontrol_init(indraref, 10000, 1000, indra_error_call)
    if result: 
        print("Error occurred: " + indra_error_out())
        return result

    # Set speed
    result = indralib.speedcontrol_write(indraref, 100, 10000, indra_error_call)
    if result: 
        print("Error occurred: " + indra_error_out())
        return result

    input("(Press any key to stop the drive...)")

    # Clear speed
    result = indralib.speedcontrol_write(indraref, 0, 10000, indra_error_call)
    if result: 
        print("Error occurred: " + indra_error_out())
        return result



    # Closing communication channel
    result = indralib.close(indraref, indra_error_call)
    if result: 
        print("Error occurred: " + indra_error_out())
        return result

    return 0


if __name__ == "__main__":
    sys.exit(int(main() or 0))