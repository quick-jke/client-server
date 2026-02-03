#ifndef LOGGERMIDDLEWARE_H
#define LOGGERMIDDLEWARE_H

#include "../../lib/requesthandler.h"
#include "../../server/src/httpserver.h"

class LoggerMiddleware : public Middleware
{
public:
    bool process(RequestHandler::Request& request) override;
};

#endif // LOGGERMIDDLEWARE_H
