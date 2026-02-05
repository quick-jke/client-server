#ifndef HELLOHANDLER_H
#define HELLOHANDLER_H

#include "basehandler.hpp"

class HelloHandler : public BaseHandler
{
public:
    Response handleRequest(const Request& request) override;
    
private:
    Response handleGet(const Request& request);
};

#endif // HELLOHANDLER_H
