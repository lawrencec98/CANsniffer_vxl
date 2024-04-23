// Hardcoded Application paramters
char appName[] = "CANsniffer_app";
unsigned int hwType = XL_HWTYPE_VN1630;
unsigned int hwIndex = 0;
unsigned int hwChannel = 3;
unsigned int appChannel = 0;
unsigned int busType = XL_BUS_TYPE_CAN;

// Application class
class Application {

public:
    XLstatus xlGetApplConfig();
    XLstatus xlSetApplConfig();
    Application(char *appName, unsigned int appChannel,
                unsigned int *pHwType,
                unsigned int *pHwIndex,
                unsigned int *pHwChannel,
                unsigned int busType) 
                
                : appName(appName),
                appChannel(appChannel),
                pHwType(pHwType),
                pHwIndex(pHwIndex),
                pHwChannel(pHwChannel),
                busType(busType) {};

private:
    char *appName;
    unsigned int appChannel;
    unsigned int *pHwType;
    unsigned int *pHwIndex;
    unsigned int *pHwChannel;
    unsigned int busType;
};