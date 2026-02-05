#ifndef REQUESTBUILDER_H
#define REQUESTBUILDER_H

#include <QString>
#include <QVariantMap>
#include <QJsonObject>
#include "httpclient.hpp"

class RequestBuilder
{
public:
    explicit RequestBuilder(HttpClient* client = nullptr);
    
    // Цепочка методов
    RequestBuilder& baseUrl(const QString& url);
    RequestBuilder& path(const QString& path);
    RequestBuilder& param(const QString& key, const QVariant& value);
    RequestBuilder& params(const QVariantMap& params);
    RequestBuilder& header(const QString& key, const QString& value);
    RequestBuilder& body(const QJsonObject& json);
    RequestBuilder& body(const QByteArray& data);
    RequestBuilder& apiKey(const QString& key);
    
    // Выполнение запроса
    QNetworkReply* get();
    QNetworkReply* post();
    QNetworkReply* put();
    QNetworkReply* del();
    
    // Сброс
    void reset();
    
private:
    HttpClient* m_client;
    QString m_baseUrl;
    QString m_path;
    QVariantMap m_params;
    QVariantMap m_headers;
    QByteArray m_body;
};

#endif // REQUESTBUILDER_H
