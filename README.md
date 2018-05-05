# commutator

This is draft version of the CNP protocol descriptionand of the Commutator description

The target of the project it is a create a cross-platfrom client-server communication solution

Linux:
On the linux platfrom the communication with the server will be proceed thought the kernel module.
The kernel module will be creating the device in the /dev that allows to open, read and write to it
The configuration of the server now is under discussion

Windows:
On the Windows platform the communication with the server is also will be implemented thought the user-mode driver
Also planned to implement the communication using WMI (WMI provider) and PowerShell (PowerShell-module)
The configuration of the server will be implemented as MMC snap-in

CNP - Custom network protocol

Request content:
CNP vXX.XX - where vXX.XX is the version of the protocol, this field is mandatory
Operation: XXX - where XXX - is the operation in the string representation, this field is mandatory
Data: XXX - where XXX - the data of the operation, this field is optional

Response content:
CNP vXX.XX - the same as in the request
Operation: XXX - the same as in the request
Status: XXX - the status of the operation execution
Data: XXX - the data of the operation execution

All fields in the response must be present