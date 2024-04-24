// to compile and link:  
// 1. g++ -c main.cpp -o main.o -Ibin (Compile main.cpp to an object file and include the bin directory for header files)
// 2. g++ main.o -o main -Lbin -lvxlapi (Link main.o with vxlapi.lib to create the executable)

#include <Windows.h>
#include <iostream>
#include <string>


#include "bin\vxlapi.h"
#include "driver_init.h"
#include "channel_setup.h"


int main() {

    XLstatus xlstatus;

    InitDriver();
    SetupChannel();

}