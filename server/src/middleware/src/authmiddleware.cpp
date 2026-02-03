#include "include/authmiddleware.h"
#include "../../lib/common/logger.h"  

AuthMiddleware::AuthMiddleware()
{
    addApiKey("test-api-key-12345");
}

bool AuthMiddleware::process(RequestHandler::Request& request)
{
    bool isProtected = false;
    for (const QString& protectedPath : m_protectedPaths) {
        if (request.path.startsWith(protectedPath)) {
            isProtected = true;
            break;
        }
    }
    
    if (!isProtected) {
        return true; 
    }
    
    QString apiKey = request.headers.value("X-API-Key", "");
    
    if (apiKey.isEmpty()) {
        apiKey = request.queryParams.value("api_key", "");
    }
    
    if (apiKey.isEmpty() || !m_apiKeys.contains(apiKey)) {
        LOG_WARNING(QString("Unauthorized access attempt from %1 to %2")
            .arg(request.clientAddress, request.path));
        return false; 
    }
    
    LOG_DEBUG(QString("Authorized access from %1").arg(request.clientAddress));
    return true;
}

void AuthMiddleware::addProtectedPath(const QString& path)
{
    m_protectedPaths.insert(path);
}

void AuthMiddleware::addApiKey(const QString& apiKey)
{
    m_apiKeys.insert(apiKey);
}
