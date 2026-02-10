#ifndef AUTHMIDDLEWARE_H
#define AUTHMIDDLEWARE_H

#include "../../lib/requesthandler.hpp"
#include "../../server/src/httpserver.hpp"
#include <QSet>

class AuthMiddleware : public Middleware
{
public:
    AuthMiddleware();
    
    bool process(RequestHandler::Request& request) override;
    void addProtectedPath(const QString& path);
    void addApiKey(const QString& apiKey);
    
private:
    QSet<QString> protectedPaths_;
    QSet<QString> apiKeys_;
};

#endif // AUTHMIDDLEWARE_H
