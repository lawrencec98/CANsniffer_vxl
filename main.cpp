// to compile and link:  
// 1. g++ -c main.cpp -o main.o -Ibin (Compile main.cpp to an object file and include the bin directory for header files)
// 2. g++ main.o -o main -Lbin -lvxlapi (Link main.o with vxlapi.lib to create the executable)

#include <Windows.h>
#include <iostream>
#include <string>

#include "bin\vxlapi.h"
#include "driver_init.cpp"


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

    InitDriver();
    xlCanSetChannelBitrate(xlPortHandle, accessMask, 500000);

    
}