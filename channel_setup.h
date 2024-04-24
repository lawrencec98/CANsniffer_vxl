#ifndef CHANNEL_SETUP_H
#define CHANNEL_SETUP_H

#include "bin/vxlapi.h"
#include "driver_init.h"


XLhandle randomHandle;


class Channel {
    public:
        XLstatus setChannelBitrate(unsigned long bitrate);
        XLstatus setNotification();
        Channel() {};
    private:
        unsigned long bitrate;
};


XLstatus setChannelBitrate(unsigned long bitrate) {
    return xlCanSetChannelBitrate(xlPortHandle, accessMask, bitrate);
}


XLstatus setNotification() {
    return xlSetNotification(xlPortHandle, &randomHandle, 1);
}


XLstatus SetupChannel();

#endif