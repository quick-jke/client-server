#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QByteArray>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>

namespace JsonUtils {

QJsonObject parseObject(const QByteArray& data);
QJsonArray parseArray(const QByteArray& data);
QByteArray stringify(const QJsonObject& obj);
QByteArray stringify(const QJsonArray& arr);
QByteArray stringify(const QJsonValue& value);

bool isValidJson(const QByteArray& data);
QString getErrorMessage();

QJsonObject createSuccessResponse(const QJsonValue& data = QJsonValue());
QJsonObject createErrorResponse(const QString& message, int code = 500);

} 

#endif 
