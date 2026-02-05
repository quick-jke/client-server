#include "httpclient.hpp"
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QTimer>
#include <QJsonDocument>

#include "../../lib/common/jsonutils.hpp"

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_timeout(30000) // 30 секунд по умолчанию
{
    connect(m_networkManager, &QNetworkAccessManager::finished, 
            this, &HttpClient::onReplyFinished);
}

HttpClient::~HttpClient()
{
}

QNetworkReply* HttpClient::get(const QString& url, const QVariantMap& params)
{
    QUrl requestUrl = buildUrl(url, params);
    QNetworkRequest request = createRequest(requestUrl);
    
    LOG_DEBUG(QString("GET %1").arg(requestUrl.toString()));
    QNetworkReply* reply = m_networkManager->get(request);
    setupTimeout(reply);
    return reply;
}

QNetworkReply* HttpClient::post(const QString& url, const QByteArray& data)
{
    QUrl requestUrl(url);
    QNetworkRequest request = createRequest(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    LOG_DEBUG(QString("POST %1").arg(url));
    QNetworkReply* reply = m_networkManager->post(request, data);
    setupTimeout(reply);
    return reply;
}

QNetworkReply* HttpClient::post(const QString& url, const QJsonObject& json)
{
    return post(url, JsonUtils::stringify(json));
}

QNetworkReply* HttpClient::put(const QString& url, const QByteArray& data)
{
    QUrl requestUrl(url);
    QNetworkRequest request = createRequest(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    LOG_DEBUG(QString("PUT %1").arg(url));
    QNetworkReply* reply = m_networkManager->put(request, data);
    setupTimeout(reply);
    return reply;
}

QNetworkReply* HttpClient::put(const QString& url, const QJsonObject& json)
{
    return put(url, JsonUtils::stringify(json));
}

QNetworkReply* HttpClient::del(const QString& url, const QVariantMap& params)
{
    QUrl requestUrl = buildUrl(url, params);
    QNetworkRequest request = createRequest(requestUrl);
    
    LOG_DEBUG(QString("DELETE %1").arg(requestUrl.toString()));
    QNetworkReply* reply = m_networkManager->deleteResource(request);
    setupTimeout(reply);
    return reply;
}

void HttpClient::setDefaultHeader(const QString& key, const QString& value)
{
    m_defaultHeaders[key] = value;
}

void HttpClient::setDefaultHeaders(const QVariantMap& headers)
{
    m_defaultHeaders = headers;
}

void HttpClient::setApiKey(const QString& apiKey)
{
    m_defaultHeaders["X-API-Key"] = apiKey;
}

void HttpClient::setTimeout(int milliseconds)
{
    m_timeout = milliseconds;
}

void HttpClient::onSuccess(std::function<void(const QByteArray&, int, QNetworkReply*)> callback)
{
    m_successCallback = callback;
}

void HttpClient::onError(std::function<void(const QString&, int, QNetworkReply*)> callback)
{
    m_errorCallback = callback;
}

void HttpClient::onReplyFinished(QNetworkReply* reply)
{
    if (!reply) return;

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    QByteArray responseData = reply->readAll();
    
    if (reply->error() == QNetworkReply::NoError) {
        LOG_INFO(QString("Request successful. Status: %1").arg(statusCode));
        emit requestFinished(responseData, statusCode, reply);
        
        if (m_successCallback) {
            m_successCallback(responseData, statusCode, reply);
        }
    } else {
        QString errorString = reply->errorString();
        LOG_ERROR(QString("Request failed: %1 (Status: %2)").arg(errorString).arg(statusCode));
        emit requestError(errorString, statusCode, reply);
        
        if (m_errorCallback) {
            m_errorCallback(errorString, statusCode, reply);
        }
    }
}

QUrl HttpClient::buildUrl(const QString& baseUrl, const QVariantMap& params) const
{
    QUrl url(baseUrl);
    if (!params.isEmpty()) {
        QUrlQuery query;
        for (auto it = params.begin(); it != params.end(); ++it) {
            query.addQueryItem(it.key(), it.value().toString());
        }
        url.setQuery(query);
    }
    return url;
}

QNetworkRequest HttpClient::createRequest(const QUrl& url)
{
    QNetworkRequest request(url);
    
    // Установка заголовков по умолчанию
    for (auto it = m_defaultHeaders.begin(); it != m_defaultHeaders.end(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toString().toUtf8());
    }
    
    return request;
}

void HttpClient::setupTimeout(QNetworkReply* reply)
{
    if (m_timeout > 0 && reply) {
        QTimer::singleShot(m_timeout, reply, [reply]() {
            if (!reply->isFinished()) {
                LOG_WARNING("Request timeout");
                reply->abort();
            }
        });
    }
}
