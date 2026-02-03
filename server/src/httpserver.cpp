#include "httpserver.h"
#include <QHostAddress>
#include <QUrlQuery>
#include <QDebug>
#include "../../lib/common/logger.h"

HttpServer::HttpServer(quint16 port, QObject *parent)
    : QTcpServer(parent)
    , m_port(port)
{
    connect(this, &QTcpServer::newConnection, this, &HttpServer::onNewConnection);
}

HttpServer::~HttpServer()
{
    stop();
}

bool HttpServer::start()
{
    if (!listen(QHostAddress::Any, m_port)) {
        emit errorOccurred(QString("Failed to start server: %1").arg(errorString()));
        LOG_ERROR(QString("Failed to start server: %1").arg(errorString()));
        return false;
    }
    
    LOG_INFO(QString("Server started on port %1").arg(m_port));
    return true;
}

void HttpServer::stop()
{
    for (auto it = m_connections.begin(); it != m_connections.end();) {
        QTcpSocket* socket = it->socket;
        socket->disconnectFromHost();
        socket->deleteLater();
        it = m_connections.erase(it);
    }
    
    if (isListening()) {
        close();
        LOG_INFO("Server stopped");
    }
}

void HttpServer::setRequestHandler(std::unique_ptr<RequestHandler> handler)
{
    m_requestHandler = std::move(handler);
}

void HttpServer::addMiddleware(std::unique_ptr<Middleware> middleware)
{
    m_middlewares.push_back(std::move(middleware));
}

void HttpServer::onNewConnection()
{
    while (hasPendingConnections()) {
        QTcpSocket* socket = nextPendingConnection();
        if (socket) {
            connect(socket, &QTcpSocket::readyRead, this, &HttpServer::onReadyRead);
            connect(socket, &QTcpSocket::disconnected, this, &HttpServer::onDisconnected);
            
            m_connections[socket] = {socket, QByteArray()};
            LOG_DEBUG(QString("New connection from %1").arg(socket->peerAddress().toString()));
        }
    }
}

void HttpServer::onReadyRead()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket || !m_connections.contains(socket)) return;

    ClientConnection& conn = m_connections[socket];
    conn.buffer.append(socket->readAll());

    if (conn.buffer.contains("\r\n\r\n")) {
        RequestHandler::Request request = parseRequest(conn.buffer, socket);
        
        if (!processMiddlewares(request)) {
            RequestHandler::Response response(403);
            response.body = "{\"error\": \"Forbidden\"}";
            sendResponse(socket, response);
            return;
        }

        emit requestReceived(request.method, request.path);
        LOG_INFO(QString("%1 %2 from %3").arg(request.method, request.path, request.clientAddress));

        RequestHandler::Response response;
        if (m_requestHandler) {
            response = m_requestHandler->handleRequest(request);
        } else {
            response = RequestHandler::Response::internalError("No request handler set");
        }

        sendResponse(socket, response);
    }
}

void HttpServer::onDisconnected()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    LOG_DEBUG(QString("Client disconnected: %1").arg(socket->peerAddress().toString()));
    m_connections.remove(socket);
    socket->deleteLater();
}

RequestHandler::Request HttpServer::parseRequest(const QByteArray& data, QTcpSocket* socket) const
{
    RequestHandler::Request request;
    request.clientAddress = socket->peerAddress().toString();
    
    QStringList lines = QString::fromUtf8(data).split("\r\n");
    if (lines.isEmpty()) return request;

    QStringList requestLine = lines[0].split(' ');
    if (requestLine.size() >= 2) {
        request.method = requestLine[0].trimmed();
        QString fullPath = requestLine[1].trimmed();
        
        int queryIdx = fullPath.indexOf('?');
        if (queryIdx >= 0) {
            request.path = fullPath.left(queryIdx);
            request.queryParams = RequestHandler::parseQueryParams(fullPath.mid(queryIdx + 1));
        } else {
            request.path = fullPath;
        }
    }

    bool headersSection = true;
    for (int i = 1; i < lines.size() && headersSection; ++i) {
        QString line = lines[i].trimmed();
        if (line.isEmpty()) {
            headersSection = false;
            continue;
        }
        
        int colonIdx = line.indexOf(':');
        if (colonIdx > 0) {
            QString key = line.left(colonIdx).trimmed();
            QString value = line.mid(colonIdx + 1).trimmed();
            request.headers[key] = value;
        }
    }

    int bodyStart = data.indexOf("\r\n\r\n");
    if (bodyStart >= 0) {
        request.body = data.mid(bodyStart + 4);
    }

    return request;
}

bool HttpServer::processMiddlewares(RequestHandler::Request& request)
{
    for (auto& middleware : m_middlewares) {
        if (!middleware->process(request)) {
            return false;
        }
    }
    return true;
}

void HttpServer::sendResponse(QTcpSocket* socket, const RequestHandler::Response& response)
{
    QByteArray responseData = buildHttpResponse(response);
    socket->write(responseData);
    socket->flush();
    socket->disconnectFromHost();
}

QByteArray HttpServer::buildHttpResponse(const RequestHandler::Response& response) const
{
    QString statusLine;
    switch (response.statusCode) {
        case 200: statusLine = "HTTP/1.1 200 OK\r\n"; break;
        case 201: statusLine = "HTTP/1.1 201 Created\r\n"; break;
        case 400: statusLine = "HTTP/1.1 400 Bad Request\r\n"; break;
        case 401: statusLine = "HTTP/1.1 401 Unauthorized\r\n"; break;
        case 403: statusLine = "HTTP/1.1 403 Forbidden\r\n"; break;
        case 404: statusLine = "HTTP/1.1 404 Not Found\r\n"; break;
        case 500: statusLine = "HTTP/1.1 500 Internal Server Error\r\n"; break;
        default: statusLine = QString("HTTP/1.1 %1 Unknown\r\n").arg(response.statusCode);
    }

    QString headers;
    for (auto it = response.headers.begin(); it != response.headers.end(); ++it) {
        headers += QString("%1: %2\r\n").arg(it.key(), it.value());
    }
    headers += QString("Content-Length: %1\r\n").arg(response.body.size());
    headers += "Connection: close\r\n";
    headers += "\r\n";

    return statusLine.toUtf8() + headers.toUtf8() + response.body;
}
