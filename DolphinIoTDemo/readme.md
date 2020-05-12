# Dolphin IoT Expansion Demo

## Introduction

[Dolphin IoT Expansion](https://media.digikey.com/pdf/Data%20Sheets/Talon%20PDFs/EnOcean_Module_PB_2019.pdf) connected to a mangOH/FX30 device through the IoT Expansion connection enables Self Powered Sensors connectivity to the cloud. In this very simple demo we get and count the serial frame notifications send by Dolphin module whenever an over the air packet frame is sent by a self powered sensor (compliant with EnOcean Radio Protocol). The serial frames from Dolphin are logged and the count is uploaded to the cloud (in Octave platform) to complete the proof of concept.
In this document we use a mangOH Yellow platform, but with small changes in platform configuration this can work on other mangOH platforms or FX30 devices.

*NOTE: This demo does not parses the incoming frames from the serial, neither configures the Dolphin module to commission any device, this is just for a proof of concept to demonstrate the connectivity between self powered devices up to the cloud*

## Requirements

### Software

1. Install Leaf (mangOHYellow Getting Started Guide, apendix A). If using a different platform, then change the target device in step 5. Ensure you can build and test the downloaded applications.
	- Manual installation can be done by looking into the following resources:
	Legato Application Framework environment installed (https://docs.legato.io/18_09/basicBuild.html). 
	Leaf Workspace Manager (https://docs.legato.io/latest/confLeaf.html).

2. Enable application access to UART interface (https://docs.legato.io/17_08/howToCustomizeUART.html). 
	- AT interface can be access via microcom in an SSH session with the target device as shown below.
`root@swi-mdm9x28-wp:~#  microcom -s 9600 /dev/ttyAT`

3. Copy the `DolphinIoTDemo` application folder in the same level as the other applications.
4. Add the application to your system file (.sdef) and add the dependencies to the datahub interface. *Note: For mangOH yellow, look for yellow.sdef*

    ```
    apps:
    {
        // Other applications
        $CURDIR/apps/DolphinIoTDemo/DolphinIoTDemo
    }

    bindings:
    {
        // Other bindings
        DolphinIoTDemo.dhubIO -> dataHub.io
    }
    ```

5. Build the package.
    If using mangOH yellow package use:
	```
    $ make yellow
	```
6. Load the updated application package.
	If using mangOH yellow package use:
    ```
    $ update $LEAF_WORKSPACE/mangOH/build/update_files/yellow.wp77xx.update 192.168.2.2
    ```
7. Once the update is complete, check octave.sierrawireless.io/device/resources and look in your device resource for **DolphinIoTDemo/ReceivedUARTFrames** resource. This may take a couple minutes to be updated.
8. Trigger your Self Powered Device to send a frame, this should increase the number of ReceivedUARTFrames counter.


## Serial log

If no connection to cloud can be seen, serial log can be check for serial frames send by Dolphin IoT Expansion. To see the serial log do the following steps:

1. Open the ssh session
	`ssh root@192.168.2.2`
2. Make sure DolphinIoTDemo is running
	`$ app status`
then look for DolphinIoTDemo
3. Look the serial log for any DolphinIoTDemo output.
	`$ logread -f | grep DolphinIoTDemo`
4. Trigger the Self Powered Device. A serial stream from Dolphin device must start with the start of frame byte 0x55 and must have at least 6 bytes (Start of Frame + Header + header CRC).
The serial output should look like:
```
May 12 02:24:56 swi-mdm9x28-wp user.info Legato:  INFO | DolphinIoTDemo[26606]/DolphinIoTDemoComponent T=main | main.c _DolphinIoTDemoComponent_COMPONENT_INIT() 82 | Buffer Len = 21  Received bytes from UART2 : 55:00:07:07:01:7A:F6:00:00:2F:1B:2C:A0:01:FF:FF:FF:FF:38:00:DB
May 12 02:24:56 swi-mdm9x28-wp user.info Legato:  INFO | DolphinIoTDemo[26606]/DolphinIoTDemoComponent T=main | main.c _DolphinIoTDemoComponent_COMPONENT_INIT() 83 | Number of counted UART frames 7 .
```
