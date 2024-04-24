#include "driver_init.h"

/*
The process for initializing the driver is:
    xlopenDriver()
    xlGetDriverConfig()
    Do stuff to Application:
        -get application config
        -set application config
        -get channel index
    xlGetChannelMask()
    xlOpenPort()
*/


XLstatus InitDriver() {
    XLstatus xlstatus;

    xlOpenDriver();
    xlGetDriverConfig(&xlDriverConfig);

    Application myApp("CANSniffer_App", APP_CHANNEL, HW_TYPE, HW_INDEX, HW_CHANNEL, BUS_TYPE);
    myApp.SetAppConfig();
    

    accessMask = (myApp.get_hwType(), myApp.get_hwIndex(), myApp.get_hwChannel());
    xlOpenPort(&xlPortHandle, myApp.get_appName(), accessMask, &permissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, myApp.get_busType());
}