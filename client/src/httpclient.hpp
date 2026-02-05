#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QVariantMap>
#include <functional>
#include <memory>
#include "../../lib/common/logger.hpp"

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject *parent = nullptr);
    ~HttpClient() override;

    // GET запрос
    QNetworkReply* get(const QString& url, const QVariantMap& params = {});
    
    // POST запрос
    QNetworkReply* post(const QString& url, const QByteArray& data);
    QNetworkReply* post(const QString& url, const QJsonObject& json);
    
    // PUT запрос
    QNetworkReply* put(const QString& url, const QByteArray& data);
    QNetworkReply* put(const QString& url, const QJsonObject& json);
    
    // DELETE запрос
    QNetworkReply* del(const QString& url, const QVariantMap& params = {});
    
    // Установка заголовков
    void setDefaultHeader(const QString& key, const QString& value);
    void setDefaultHeaders(const QVariantMap& headers);
    void setApiKey(const QString& apiKey);
    
    // Таймаут
    void setTimeout(int milliseconds);
    
    // Callback'и
    void onSuccess(std::function<void(const QByteArray&, int, QNetworkReply*)> callback);
    void onError(std::function<void(const QString&, int, QNetworkReply*)> callback);

signals:
    void requestFinished(const QByteArray& response, int statusCode, QNetworkReply* reply);
    void requestError(const QString& error, int statusCode, QNetworkReply* reply);

private slots:
    void onReplyFinished(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_networkManager;
    QVariantMap m_defaultHeaders;
    int m_timeout;
    std::function<void(const QByteArray&, int, QNetworkReply*)> m_successCallback;
    std::function<void(const QString&, int, QNetworkReply*)> m_errorCallback;

    QUrl buildUrl(const QString& baseUrl, const QVariantMap& params) const;
    QNetworkRequest createRequest(const QUrl& url);
    void setupTimeout(QNetworkReply* reply);
};

#endif // HTTPCLIENT_H
