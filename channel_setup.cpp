/*
    Enter this stage once hardware has been initialised with InitDriver()
*/

#include "channel_setup.h"
#include "driver_init.h"

XLstatus SetupChannel() {

    Channel myChannel;
    myChannel.setChannelBitrate(500000);

}