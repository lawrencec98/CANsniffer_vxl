#include "application_class.h"

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


#define RX_QUEUE_SIZE 1024
#define APP_CHANNEL 0
#define HW_INDEX 0
#define HW_CHANNEL 2
#define HW_TYPE     XL_HWTYPE_VN1630
#define BUS_TYPE    XL_BUS_TYPE_CAN


XLdriverConfig xlDriverConfig;
XLportHandle xlPortHandle;
XLaccess accessMask;
XLaccess permissionMask;


XLstatus InitDriver() {
    XLstatus xlstatus;

    xlOpenDriver();
    xlGetDriverConfig(&xlDriverConfig);

    Application myApp("CANSniffer_App", APP_CHANNEL, HW_TYPE, HW_INDEX, HW_CHANNEL, BUS_TYPE);
    myApp.SetAppConfig();
    

    accessMask = (myApp.get_hwType(), myApp.get_hwIndex(), myApp.get_hwChannel());
    xlOpenPort(&xlPortHandle, myApp.get_appName(), accessMask, &permissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, myApp.get_busType());
}