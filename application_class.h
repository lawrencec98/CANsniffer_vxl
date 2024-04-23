// Application class
class Application {

public:
    XLstatus GetAppConfig();
    XLstatus SetAppConfig();
    Application(char *appName, unsigned int appChannel,
                unsigned int hwType,
                unsigned int hwIndex,
                unsigned int hwChannel,
                unsigned int busType) 
                
            : appName(appName),
            appChannel(appChannel),
            hwType(hwType),
            hwIndex(hwIndex),
            hwChannel(hwChannel),
            busType(busType) {};

    char * get_appName() {return appName;}
    unsigned int get_appChannel() const {return appChannel;}
    unsigned int get_hwType() const {return hwType;}
    unsigned int get_hwIndex() const {return hwIndex;}
    unsigned int get_hwChannel() const {return hwChannel;}
    unsigned int get_busType() const {return busType;}

private:
    char *appName;
    unsigned int appChannel;
    unsigned int hwType;
    unsigned int hwIndex;
    unsigned int hwChannel;
    unsigned int busType;
};


XLstatus Application::SetAppConfig() {
    return xlSetApplConfig(appName, appChannel, hwType, hwIndex, hwChannel, busType);
}