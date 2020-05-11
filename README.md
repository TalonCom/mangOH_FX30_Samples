# mangOH_FX30_Samples
This repository contains sample applications to verify the connectivity between different Talon IoT exansion modules and Octave platform, using either mangOH or FX30 based devices.

Each sample application provides in its respective folder, the folder structure to add the sample application, containing: the application definition (.adef), component definition (.cdef) files and the source code of the component in C languaje. It is also suggested the way to include it into the system definition file (.sdef) and its respective dependencies.

# How to use a sample application.

Each sample application have a readme file inside of each application folder which indicates the hardware needed to test it and how to perform the test to verify the connectivity with the IoT expansion and proof the concept of bridging data from your IoT expansion to the cloud in Octave platform.

In general, all samples provided in this repo already have the folder structure needed to be compiled as an application. Just add the folder of the IoT card being tested into your development environment, if you do not have a development environment yet, please follow the respective tutorial of your target device (mangOH platform have specific resources for your platform in mangoh.io, while FX30 has their own resources in source.sierrawireless.com and legato.io).

Consider only adding the applications needed to your project as the sample applications are demos and does not check for other applications coexistence (they may use the same serial interface in the IoT Expansion pinout).
