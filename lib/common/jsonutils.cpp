#include "jsonutils.hpp"
#include <QJsonParseError>

static QString lastError;

namespace JsonUtils {

QJsonObject parseObject(const QByteArray& data) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        lastError = error.errorString();
        return QJsonObject();
    }
    
    if (!doc.isObject()) {
        lastError = "JSON is not an object";
        return QJsonObject();
    }
    
    return doc.object();
}

QJsonArray parseArray(const QByteArray& data) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        lastError = error.errorString();
        return QJsonArray();
    }
    
    if (!doc.isArray()) {
        lastError = "JSON is not an array";
        return QJsonArray();
    }
    
    return doc.array();
}

QByteArray stringify(const QJsonObject& obj) {
    QJsonDocument doc(obj);
    return doc.toJson(QJsonDocument::Compact);
}

QByteArray stringify(const QJsonArray& arr) {
    QJsonDocument doc(arr);
    return doc.toJson(QJsonDocument::Compact);
}

QByteArray stringify(const QJsonValue& value) {
    QJsonDocument doc(value.toObject()); 
    return doc.toJson(QJsonDocument::Compact);
}

bool isValidJson(const QByteArray& data) {
    QJsonParseError error;
    QJsonDocument::fromJson(data, &error);
    return error.error == QJsonParseError::NoError;
}

QString getErrorMessage() {
    return lastError;
}

QJsonObject createSuccessResponse(const QJsonValue& data) {
    QJsonObject response;
    response["success"] = true;
    if (!data.isNull()) {
        response["data"] = data;
    }
    response["timestamp"] = QDateTime::currentMSecsSinceEpoch();
    return response;
}

QJsonObject createErrorResponse(const QString& message, int code) {
    QJsonObject response;
    response["success"] = false;
    response["error"] = message;
    response["code"] = code;
    response["timestamp"] = QDateTime::currentMSecsSinceEpoch();
    return response;
}

}
