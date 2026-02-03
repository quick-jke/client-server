#include "include/objects.h"
#include <QDateTime>

RequestHandler::Response ObjectsHandler::handleRequest(const RequestHandler::Request& request)
{
    if (request.method == "GET") {
        return handleGet(request);
    }
    
    return errorResponse("Method not allowed", 405);
}

RequestHandler::Response ObjectsHandler::handleGet(const RequestHandler::Request& request)
{
    QString oldRect = request.queryParams.value("oldRect");
    QString oldScale = request.queryParams.value("oldScale");
    QString newRect = request.queryParams.value("newRect");
    QString newScale = request.queryParams.value("newScale");
    
    QJsonObject data;
    data["oldRect"] = oldRect;
    data["oldScale"] = oldRect;
    data["newRect"] = oldRect;
    data["newScale"] = oldRect;
    data["timestamp"] = QDateTime::currentMSecsSinceEpoch();
    data["client"] = request.clientAddress;
    
    return successResponse(data);
}
