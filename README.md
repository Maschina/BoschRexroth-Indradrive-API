# IndradriveAPI
-------------
API Interface for Bosch-Rexroth's Indradrive based on SIS protocol


The Indradrive API provides an universal programming interface to the Indradrive M devices. A dedicated DLL (IndradriveAPI.dll, or IndradriveAPI-LV.dll for LabVIEW) handles the user inputs and converts them to SIS protocol telegrams. These telegrams are transfered to the Indradrive device via RS232 interface (refer to Indradrive User's Manual for more information). The API uses the reply telegram to extract the required data or identifies potentials errors and provides it back to the user.

Please refer to doc/Indradrive - API.chm, or doc/Indradrive - API.pdf for more information.