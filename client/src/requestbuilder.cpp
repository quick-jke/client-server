#include "requestbuilder.h"
#include <QJsonDocument>  

RequestBuilder::RequestBuilder(HttpClient* client)
    : m_client(client)
{
}

RequestBuilder& RequestBuilder::baseUrl(const QString& url)
{
    m_baseUrl = url;
    return *this;
}

RequestBuilder& RequestBuilder::path(const QString& path)
{
    m_path = path;
    return *this;
}

RequestBuilder& RequestBuilder::param(const QString& key, const QVariant& value)
{
    m_params[key] = value;
    return *this;
}

RequestBuilder& RequestBuilder::params(const QVariantMap& params)
{
    m_params = params;
    return *this;
}

RequestBuilder& RequestBuilder::header(const QString& key, const QString& value)
{
    m_headers[key] = value;
    return *this;
}

RequestBuilder& RequestBuilder::body(const QJsonObject& json)
{
    QJsonDocument doc(json);
    m_body = doc.toJson(QJsonDocument::Compact);
    return *this;
}

RequestBuilder& RequestBuilder::body(const QByteArray& data)
{
    m_body = data;
    return *this;
}

RequestBuilder& RequestBuilder::apiKey(const QString& key)
{
    m_headers["X-API-Key"] = key;
    return *this;
}

QNetworkReply* RequestBuilder::get()
{
    QString url = m_baseUrl + m_path;
    
    if (m_client) {
        for (auto it = m_headers.begin(); it != m_headers.end(); ++it) {
            m_client->setDefaultHeader(it.key(), it.value().toString());
        }
        return m_client->get(url, m_params);
    }
    
    return nullptr;
}

QNetworkReply* RequestBuilder::post()
{
    QString url = m_baseUrl + m_path;
    
    if (m_client) {
        for (auto it = m_headers.begin(); it != m_headers.end(); ++it) {
            m_client->setDefaultHeader(it.key(), it.value().toString());
        }
        return m_client->post(url, m_body);
    }
    
    return nullptr;
}

QNetworkReply* RequestBuilder::put()
{
    QString url = m_baseUrl + m_path;
    
    if (m_client) {
        for (auto it = m_headers.begin(); it != m_headers.end(); ++it) {
            m_client->setDefaultHeader(it.key(), it.value().toString());
        }
        return m_client->put(url, m_body);
    }
    
    return nullptr;
}

QNetworkReply* RequestBuilder::del()
{
    QString url = m_baseUrl + m_path;
    
    if (m_client) {
        for (auto it = m_headers.begin(); it != m_headers.end(); ++it) {
            m_client->setDefaultHeader(it.key(), it.value().toString());
        }
        return m_client->del(url, m_params);
    }
    
    return nullptr;
}

void RequestBuilder::reset()
{
    m_path.clear();
    m_params.clear();
    m_headers.clear();
    m_body.clear();
}
