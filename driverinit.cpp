#include "vxlapi.h"

#define RX_QUEUE_SIZE           1024

// Globals
XLdriverConfig xlDriverConfig;
XLaccess accessMask;
XLportHandle xlPortHandle;
XLaccess xlPermissionMask;

//application config
char *appName = "xl_cpp";
unsigned int hwType;
unsigned int hwIndex;
unsigned int hwChannel;

// Functions for Driver Init
XLstatus xlOpenDriver(void);
XLstatus xlCloseDriver(void);
XLstatus xlGetDriverConfig(XLdriverConfig *pDriverConfig);
XLstatus xlGetApplConfig (
    char            *appName,
    unsigned int    appChannel,
    unsigned int    *pHwType,
    unsigned int    *pHwIndex,
    unsigned int    *pHwChannel,
    unsigned int    busType
);
XLstatus xlSetApplConfig (
    char            *appName,
    unsigned int    appChannel,
    unsigned int    hwType,
    unsigned int    hwIndex,
    unsigned int    hwChannel,
    unsigned int    busType
);
int xlGetChannelIndex (
    int             hwType,
    int             hwIndex,
    int             hwChannel
);
XLaccess xlGetChannelMask (
    int             hwType,
    int             hwIndex,
    int             hwChannel
);
XLstatus xlOpenPort (
    XLportHandle    *portHandle,
    char            *userName,
    XLaccess        accessMask,
    XLaccess        *permissionMask,
    unsigned int    rxQueueSize,
    unsigned int    xlInterfaceVersion,
    unsigned int    busType
);


////////////////////////////////////////////////
////////          InitDriver()          ////////         
////////////////////////////////////////////////

int InitDriver() {
    //define variables
    XLstatus xlstatus;

    xlstatus = xlOpenDriver();

    if (XL_SUCCESS == xlstatus) {
        xlstatus = xlGetDriverConfig(&xlDriverConfig);
    }

    if (XL_SUCCESS == xlstatus) {
        if (xlGetApplConfig(appName, 0, &hwType, &hwIndex, &hwChannel, XL_BUS_TYPE_CAN) != XL_SUCCESS
        || xlGetApplConfig(appName, 1, &hwType, &hwIndex, &hwChannel, XL_BUS_TYPE_CAN) != XL_SUCCESS) {

            xlstatus = xlSetApplConfig(appName, 0, XL_HWTYPE_VN1630, hwIndex, hwChannel, XL_BUS_TYPE_CAN);
            xlstatus = xlSetApplConfig(appName, 1, XL_HWTYPE_VN1630, hwIndex, hwChannel, XL_BUS_TYPE_CAN);        
        }
    }

    if (XL_SUCCESS == xlstatus) {
        xlstatus = xlGetChannelMask(hwType, hwIndex, hwChannel);
    }

    if (XL_SUCCESS == xlstatus) {
        accessMask == xlGetChannelMask(hwType,hwIndex,hwChannel); 
    }

    xlPermissionMask = accessMask;
    xlstatus == xlOpenPort(&xlPortHandle, appName, accessMask, &xlPermissionMask, RX_QUEUE_SIZE, XL_INTERFACE_VERSION, XL_BUS_TYPE_CAN);
}