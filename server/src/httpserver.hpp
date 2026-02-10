#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QMap>
#include <memory>
#include <vector>
#include "../../lib/requesthandler.hpp"

class Middleware {
public:
    virtual bool process(RequestHandler::Request& request) = 0;
    virtual ~Middleware() = default;
};

class HttpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit HttpServer(quint16 port = 8080, QObject *parent = nullptr);
    ~HttpServer() override;

    bool start();
    void stop();
    void setRequestHandler(std::unique_ptr<RequestHandler> handler);
    void addMiddleware(std::unique_ptr<Middleware> middleware);

signals:
    void requestReceived(const QString& method, const QString& path);
    void errorOccurred(const QString& error);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    struct ClientConnection {
        QTcpSocket* socket;
        QByteArray buffer;
    };

    QMap<QTcpSocket*, ClientConnection> connections_;
    std::unique_ptr<RequestHandler> requestHandler_;
    std::vector<std::unique_ptr<Middleware>> middlewares_;
    quint16 port_;

    RequestHandler::Request parseRequest(const QByteArray& data, QTcpSocket* socket) const;
    void sendResponse(QTcpSocket* socket, const RequestHandler::Response& response);
    QByteArray buildHttpResponse(const RequestHandler::Response& response) const;
    bool processMiddlewares(RequestHandler::Request& request);
};

#endif 
