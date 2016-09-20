import sys
import ctypes


# Load Indradrive API DLL into memory.
libpath = "C:\\AE\\Alternator_Testbench\\Software\\API-Hardware\\Indradrive\\Lib\\IndradriveWrapper.dll"
hllDll = ctypes.WinDLL(libpath)


# MAIN ENTRY POINT
def main():
    print(hllDll)
    

if __name__ == "__main__":
    sys.exit(int(main() or 0))