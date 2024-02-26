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
unsigned int hwType;
unsigned int hwIndex;
unsigned int hwChannel;


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
        if (xlGetApplConfig(appName, 0, &hwType, &hwIndex, &hwChannel, XL_BUS_TYPE_CAN) != XL_SUCCESS
        || xlGetApplConfig(appName, 1, &hwType, &hwIndex, &hwChannel, XL_BUS_TYPE_CAN) != XL_SUCCESS) {

            xlstatus = xlSetApplConfig(appName, 0, XL_HWTYPE_VN1630, hwIndex, hwChannel, XL_BUS_TYPE_CAN);
            xlstatus = xlSetApplConfig(appName, 1, XL_HWTYPE_VN1630, hwIndex, hwChannel, XL_BUS_TYPE_CAN);
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
        xlstatus == xlOpenPort(&xlPortHandle, appName, accessMask, &xlPermissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);
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
XLstatus StartStopChannel(int fChannelActivated) {
    XLstatus xlstatus;

    if (fChannelActivated) {
        xlstatus = xlDeactivateChannel(xlPortHandle, accessMask);
    }
    else {
        xlstatus = xlActivateChannel(xlPortHandle, accessMask, XL_BUS_TYPE_CAN, XL_ACTIVATE_NONE);
    }
    return xlstatus;
}


int main() {

    XLstatus xlstatus;
    int fChannelActivated;
    int outputMode = XL_OUTPUT_MODE_NORMAL;

    xlstatus = InitDriver();

    if (XL_SUCCESS == xlstatus) {
        xlstatus = xlActivateChannel(xlPortHandle, accessMask, XL_BUS_TYPE_CAN,XL_ACTIVATE_RESET_CLOCK);
    }

}