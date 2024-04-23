#include "bin/vxlapi.h"
#include "application_config.h"

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

XLdriverConfig xlDriverConfig;
XLportHandle xlPortHandle;
XLaccess accessMask;
XLaccess permissionMask;


XLstatus InitDriver() {
    XLstatus xlstatus;

    xlOpenDriver();
    xlGetDriverConfig(&xlDriverConfig);

    if(xlGetApplConfig(appName, appChannel, &hwType, &hwIndex, &hwChannel, busType)) {
        xlSetApplConfig(appName, appChannel, hwType, hwIndex, hwChannel, busType);
    }

    accessMask = (hwType, hwIndex, hwChannel);
    xlOpenPort(&xlPortHandle, appName, accessMask, &permissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, busType);
}