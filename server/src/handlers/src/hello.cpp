#include "include/hello.hpp"
#include <QDateTime>

RequestHandler::Response HelloHandler::handleRequest(const RequestHandler::Request& request)
{
    if (request.method == "GET") {
        return handleGet(request);
    }
    
    return errorResponse("Method not allowed", 405);
}

RequestHandler::Response HelloHandler::handleGet(const RequestHandler::Request& request)
{
    QString name = request.queryParams.value("name", "World");
    
    if (name.length() > 100) {
        return errorResponse("Name too long", 400);
    }
    
    QJsonObject data;
    data["message"] = QString("Hello, %1!").arg(name);
    data["timestamp"] = QDateTime::currentMSecsSinceEpoch();
    data["client"] = request.clientAddress;
    
    return successResponse(data);
}
