// to compile and link:  
// 1. g++ -c main.cpp -o main.o -Ibin (Compile main.cpp to an object file and include the bin directory for header files)
// 2. g++ main.o -o main -Lbin -lvxlapi (Link main.o with vxlapi.lib to create the executable)

#include <Windows.h>
#include <iostream>
#include <string>

#include "bin\vxlapi.h"

#define RX_QUEUE_SIZE           1024

////////////////////////////////////////////////////////////////////////////////////////
// Globals

XLdriverConfig xlDriverConfig;
XLaccess accessMask;
XLportHandle xlPortHandle;
XLaccess xlPermissionMask;


////////////////////////////////////////////////////////////////////////////////////////
// Application config variables

char appName[] = "vxlCANcpp";
unsigned int hwType = XL_HWTYPE_VN1630;
unsigned int hwIndex = 0;
unsigned int hwChannel = 2;
unsigned int appChannel = 0;
unsigned int busType = XL_BUS_TYPE_CAN;


////////////////////////////////////////////////
////              InitDriver()              ////         
////////////////////////////////////////////////
// Opens and configures driver and application

XLstatus InitDriver() {

    XLstatus xlstatus;

    // Open Driver 
    xlstatus = xlOpenDriver();
    std::cout << "xlOpenDriver status: " << xlstatus << '\n';
    if (XL_SUCCESS != xlstatus) {
        std::cout << "Error opening driver!\n";
        return xlstatus;
    }

    // Get hardware configuration
    if (XL_SUCCESS == xlstatus) {
        xlstatus = xlGetDriverConfig(&xlDriverConfig);
        std::cout << "xlGetDriverConfig status: " << xlstatus << '\n';
        if (XL_SUCCESS != xlstatus) {
            std::cout << "Error getting driver config!\n";
            return xlstatus;
        }
    }

    // Get application configuration
    if (XL_SUCCESS == xlstatus) {
        xlstatus = xlSetApplConfig(appName, appChannel, hwType, hwIndex, hwChannel, busType);
        std::cout << "xlSetApplConfig status: " << xlstatus << '\n';
        if (XL_SUCCESS != xlstatus) {
            std::cout << "Error setting application config!\n";
            return xlstatus;
        }
    }

    // Get channel mask 
    if (XL_SUCCESS == xlstatus) {
        accessMask == xlGetChannelMask(hwType,hwIndex,hwChannel);
        std::cout << "xlGetChannelMask status:\n";
        std::cout << "\thwType: " << hwType << '\n';
        std::cout << "\thwIndex: " << hwIndex << '\n';
        std::cout << "\thwChannel: " << hwChannel << '\n';
        std::cout << "\taccessMask: " << accessMask << '\n'; 
    }

    // Open Port
    xlPermissionMask = accessMask;
    if (XL_SUCCESS == xlstatus) {
        xlstatus == xlOpenPort(&xlPortHandle, appName, accessMask, &xlPermissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, busType);
        std::cout << "xlOpenPort status: " << xlstatus << '\n';
        if (XL_SUCCESS != xlstatus) {
            std::cout << "Error opening port!\n";
            return xlstatus;
        }
    }
    return xlstatus;
}


// USER CONTROL: FlipChannelOutput() flips ACK on or off
XLstatus FlipChannelOutputMode(int outputMode) {
    XLstatus xlstatus;

    // Set to normal mode (ACK ON)
    if (outputMode != XL_OUTPUT_MODE_NORMAL) {
        xlstatus = xlCanSetChannelOutput(xlPortHandle, accessMask, XL_OUTPUT_MODE_NORMAL);
    }
    if (XL_SUCCESS == xlstatus && outputMode != XL_OUTPUT_MODE_SILENT) {
        xlstatus = xlCanSetChannelOutput(xlPortHandle, accessMask, XL_OUTPUT_MODE_SILENT);
    }

    return xlstatus;
}


// USER CONTROL: StartStopChannel() activates and deactivates channel
XLstatus StartStopChannel(int& fChannelActivated) {
    XLstatus xlstatus;

    if (fChannelActivated) {
        xlstatus = xlDeactivateChannel(xlPortHandle, accessMask);
    }
    else {
        xlstatus = xlActivateChannel(xlPortHandle, accessMask, busType, XL_ACTIVATE_NONE);
    }
    return xlstatus;
}


XLstatus ReadRXMessages() {

    XLhandle xlHandle;
    XLstatus xlstatus;
    XLevent xlEvent;
    unsigned int xlEventCount = 1;
    bool RXThreadRun;

    while (RXThreadRun) {

        WaitForSingleObject(xlHandle, 10);
        xlstatus == XL_SUCCESS;

        while(!xlstatus) {
            xlEventCount = 1;
            xlstatus = xlReceive(xlPortHandle, &xlEventCount, &xlEvent);

            if (xlstatus != XL_ERR_QUEUE_IS_EMPTY) {
                std::string eventString = xlGetEventString(&xlEvent);
            }
        }
    }
}


int main() {

    XLstatus xlstatus;
    int fChannelActivated;
    int outputMode = XL_OUTPUT_MODE_NORMAL;

    xlstatus = InitDriver();
    xlCanSetChannelBitrate(xlPortHandle, accessMask, 500000);

    if (XL_SUCCESS == xlstatus) {
        xlstatus = xlActivateChannel(xlPortHandle, accessMask, busType,XL_ACTIVATE_RESET_CLOCK);
        if (XL_SUCCESS == xlstatus) fChannelActivated = 1;
    }

    while(fChannelActivated) {
        if (XL_SUCCESS == xlstatus) {
            xlstatus = ReadRXMessages();
        }
    }
}