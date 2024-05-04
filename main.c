// to compile and link:  
// 1. g++ -c main.c -o main.o -Ibin (Compile main.cpp to an object file and include the bin directory for header files)
// 2. g++ main.o -o main -Lbin -lvxlapi (Link main.o with vxlapi.lib to create the executable)

// system includes
#include <Windows.h>
#include <stdio.h>
#define UNUSED_PARAM(a) { a=a; }
// third-party includes
#include "bin\vxlapi.h"
// personal libraries

#define RX_QUEUE_SIZE 1024
#define HW_TYPE     XL_HWTYPE_VN1630
#define BUS_TYPE    XL_BUS_TYPE_CAN
#define QUEUE_LEVEL 1
#define TIMER_RATE 100

// global variables
char *appName = "CANSniffer_App";
unsigned int appChannel = 0;
unsigned int hwIndex = 0;
unsigned int hwChannel = 2;
unsigned int channelIndex;
unsigned int channelMask;
unsigned long baudrate = 500000;

XLdriverConfig g_driverConfig;
XLportHandle g_portHandle;
XLaccess g_accessMask = 0x04;
XLaccess g_permissionMask;
XLhandle g_threadHandle;
XLhandle g_msgEvent;

HANDLE g_RXthread;


XLstatus InitDriver() {
    XLstatus xlstatus;

    xlOpenDriver(); 
    xlSetApplConfig(appName, appChannel, HW_TYPE, hwIndex, hwChannel, BUS_TYPE);
    // xlGetDriverConfig(&g_driverConfig);
    
    // channelIndex = xlGetChannelIndex(HW_TYPE, hwIndex, hwChannel);
    
    channelMask = xlGetChannelMask(HW_TYPE, hwIndex, hwChannel);
    g_accessMask = channelMask; // only works if ONE channel only, will break if more than one.
    
    xlstatus = xlOpenPort(&g_portHandle, appName, g_accessMask, &g_permissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, BUS_TYPE);
    if (g_portHandle == XL_INVALID_PORTHANDLE) {
        printf("INVALID PORTHANDLE: %d\n", g_portHandle);
    }
    if (xlstatus != XL_SUCCESS) {
        printf("InitDriver() could not open port\n");
    }
}


XLstatus OnBus() {
    XLstatus xlstatus;

    xlResetClock(g_portHandle);
    xlstatus = xlSetTimerRate(g_portHandle, TIMER_RATE);
    xlCanSetChannelBitrate(g_portHandle, g_accessMask, baudrate);
    return xlstatus;
}


DWORD WINAPI RxThread(LPVOID par) {
  XLstatus        xlStatus;
  
  unsigned int    msgsrx = QUEUE_LEVEL;
  XLevent         xlEvent; 
  
  UNUSED_PARAM(par); 
  

  while (1) { 
   
    WaitForSingleObject(g_msgEvent,10);

    xlStatus = XL_SUCCESS;
    
    while (!xlStatus) {
      
      msgsrx = QUEUE_LEVEL;

      xlStatus = xlReceive(g_portHandle, &msgsrx, &xlEvent);      
      if ( xlStatus!=XL_ERR_QUEUE_IS_EMPTY ) {
        printf("%s\n", xlGetEventString(&xlEvent));
      }  
    }
          
  }
  return NO_ERROR; 
}


XLstatus CreateRXThread() {
    XLstatus xlstatus = XL_ERROR;
    DWORD ThreadId=0;

    if (g_portHandle != XL_INVALID_PORTHANDLE) {
        xlstatus = xlSetNotification(g_portHandle, &g_msgEvent, QUEUE_LEVEL);
        printf("SetNotification = %d\n", xlstatus);
        g_RXthread = CreateThread(0, 0x1000, RxThread, (LPVOID) 0, 0, &ThreadId);
    }
    return xlstatus;
}


int main() {
    XLstatus xlstatus;

    xlstatus = InitDriver();
    printf("InitDriver = %d\n", xlstatus);

    xlstatus = OnBus();
    printf("OnBus = %d\n", xlstatus);

    xlstatus = CreateRXThread();
    printf("CreateRXThread = %d\n", xlstatus);

    xlstatus = OnBus();
    printf("OnBus = %d\n", xlstatus);

    xlstatus = xlActivateChannel(g_portHandle, g_accessMask, BUS_TYPE, XL_ACTIVATE_RESET_CLOCK);
    printf("ActivateChannel = %d\n", xlstatus);
}