#ifndef LOGGERMIDDLEWARE_H
#define LOGGERMIDDLEWARE_H

#include "../../lib/requesthandler.hpp"
#include "../../server/src/httpserver.hpp"

class LoggerMiddleware : public Middleware
{
public:
    bool process(RequestHandler::Request& request) override;
};

#endif // LOGGERMIDDLEWARE_H
