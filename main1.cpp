// to compile and link  (Compile main.cpp to an object file and include the bin directory for header files)
// 1. g++ -c main.cpp -o main.o -Ibin (# Link main.o with vxlapi.lib to create the executable)
// 2. g++ main.o -o main -Lbin -lvxlapi

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

char appName[] = "vxl_CAN_cpp";
unsigned int hwType = 0;
unsigned int hwIndex = 0;
unsigned int hwChannel = 0;
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
    std::cout << "xlOpenDriver\n";

    // Get hardware configuration
    if (XL_SUCCESS == xlstatus) {
        xlstatus = xlGetDriverConfig(&xlDriverConfig);
        std::cout << "xlGetDriverConfig\n";
    }

    // Get application configuration
    if (XL_SUCCESS == xlstatus) {
        if (xlGetApplConfig(appName, appChannel, &hwType, &hwIndex, &hwChannel, busType) != XL_SUCCESS
        || xlGetApplConfig(appName, appChannel, &hwType, &hwIndex, &hwChannel, busType) != XL_SUCCESS) {

            xlstatus = xlSetApplConfig(appName, appChannel, hwType, hwIndex, hwChannel, busType);
            xlstatus = xlSetApplConfig(appName, appChannel, hwType, hwIndex, hwChannel, busType);
            std::cout << "xlSetApplConfig\n";        
        }
    }

    // Get channel mask 
    if (XL_SUCCESS == xlstatus) {
        accessMask == xlGetChannelMask(hwType,hwIndex,hwChannel);
        std::cout << "xlGetChannelMask\n";
    }

    // Open Port
    xlPermissionMask = accessMask;
    if (XL_SUCCESS == xlstatus) {
        xlstatus == xlOpenPort(&xlPortHandle, appName, accessMask, &xlPermissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, busType);
        std::cout << "xlOpenPort\n";
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


int main() {

    XLstatus xlstatus;
    int fChannelActivated;
    int outputMode = XL_OUTPUT_MODE_NORMAL;

    xlstatus = InitDriver();

    if (XL_SUCCESS == xlstatus) {
        xlstatus = xlActivateChannel(xlPortHandle, accessMask, busType,XL_ACTIVATE_RESET_CLOCK);
        if (XL_SUCCESS == xlstatus) fChannelActivated = 1;
    }
}