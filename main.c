// to compile and link:  
// 1. g++ -c main.c -o main.o -Ibin (Compile main.cpp to an object file and include the bin directory for header files)
// 2. g++ main.o -o main -Lbin -lvxlapi (Link main.o with vxlapi.lib to create the executable)

// system includes
#include <Windows.h>
#include <stdio.h>
// third-party includes
#include "bin\vxlapi.h"

#define RX_QUEUE_SIZE 1024
#define HW_TYPE     XL_HWTYPE_VN1630
#define BUS_TYPE    XL_BUS_TYPE_CAN

// global variables
char *appName = "CANSniffer_App";
unsigned int appChannel = 0;
unsigned int hwIndex = 0;
unsigned int hwChannel = 2;
unsigned int channelIndex;
unsigned int channelMask;

XLdriverConfig g_driverConfig;
XLportHandle g_portHandle;
XLaccess g_accessMask;
XLaccess g_permissionMask;


XLstatus InitDriver() {

    xlOpenDriver();
    xlGetDriverConfig(&g_driverConfig);
    xlSetApplConfig(appName, appChannel, HW_TYPE, hwIndex, hwChannel, BUS_TYPE);
    
    channelIndex = xlGetChannelIndex(HW_TYPE, hwIndex, hwChannel);
    
    channelMask = xlGetChannelMask(HW_TYPE, hwIndex, hwChannel);
    g_accessMask = channelMask; // only works if ONE channel only, will break if more than one.
    
    return xlOpenPort(&g_portHandle, appName, g_accessMask, &g_permissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, BUS_TYPE);
    
}


void printVariables() {
    printf("g_driverConfig = %d\n", g_driverConfig);
    printf("g_portHandle = %d\n", g_portHandle);
    printf("g_accessMask = %d\n", g_accessMask);
    printf("g_permissionMask = %d\n", g_permissionMask);

    return;
}


int main() {

    XLstatus xlstatus;

    xlstatus = InitDriver();
    printf("InitDriverStatus = %d\n", xlstatus);
    xlCanSetChannelBitrate(g_portHandle, g_accessMask, 500000);

    
}