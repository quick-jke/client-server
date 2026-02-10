#ifndef OBJECTSHANDLER_H
#define OBJECTSHANDLER_H
#include "basehandler.hpp"
#include "storage/include/storage.hpp"

class ObjectsHandler : public BaseHandler
{
public:
    Response handleRequest(const Request& request) override;
    
private:
    Response handleGet(const Request& request);

    MapObjectStorage storage_;
};
#endif
