#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include "basehandler.h"

class SettingsHandler : public BaseHandler
{
public:
    Response handleRequest(const Request& request) override;
    
private:
    Response handleGet(const Request& request);
};

#endif 
