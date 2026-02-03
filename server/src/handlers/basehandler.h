#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include "../../lib/requesthandler.h"
#include "../../lib/common/jsonutils.h"

class BaseHandler : public RequestHandler
{
public:
    BaseHandler() = default;
    
protected:
    Response jsonResponse(const QJsonObject& obj, int statusCode = 200) {
        Response response(statusCode);
        response.body = JsonUtils::stringify(obj);
        return response;
    }
    
    Response successResponse(const QJsonValue& data = QJsonValue()) {
        return jsonResponse(JsonUtils::createSuccessResponse(data), 200);
    }
    
    Response errorResponse(const QString& message, int code = 500) {
        return jsonResponse(JsonUtils::createErrorResponse(message, code), code);
    }
    
    QJsonObject parseRequestBody(const QByteArray& body) {
        return JsonUtils::parseObject(body);
    }
};

#endif // BASEHANDLER_H
