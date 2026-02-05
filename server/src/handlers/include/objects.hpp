#ifndef OBJECTSHANDLER_H
#define OBJECTSHANDLER_H
#include "basehandler.hpp"

class ObjectsHandler : public BaseHandler
{
public:
    Response handleRequest(const Request& request) override;
    
private:
    Response handleGet(const Request& request);
};
#endif
