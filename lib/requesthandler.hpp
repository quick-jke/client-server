#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include <QByteArray>
#include <QString>
#include <QMap>
#include <QUrl>
#include <memory>

class RequestHandler
{
public:
    struct Request {
        QString method;
        QString path;
        QMap<QString, QString> headers;
        QByteArray body;
        QMap<QString, QString> queryParams;
        QString clientAddress;
    };

    struct Response {
        int statusCode;
        QMap<QString, QString> headers;
        QByteArray body;

        Response(int code = 200) : statusCode(code) {
            headers["Content-Type"] = "application/json; charset=utf-8";
            headers["Server"] = "QtHttpServer/1.0";
        }

        static Response ok(const QByteArray& body) {
            Response resp(200);
            resp.body = body;
            return resp;
        }

        static Response notFound(const QString& message = "Not Found") {
            Response resp(404);
            resp.body = QString("{\"error\": \"%1\"}").arg(message).toUtf8();
            return resp;
        }

        static Response badRequest(const QString& message = "Bad Request") {
            Response resp(400);
            resp.body = QString("{\"error\": \"%1\"}").arg(message).toUtf8();
            return resp;
        }

        static Response internalError(const QString& message = "Internal Server Error") {
            Response resp(500);
            resp.body = QString("{\"error\": \"%1\"}").arg(message).toUtf8();
            return resp;
        }
    };

    virtual Response handleRequest(const Request& request) = 0;
    virtual ~RequestHandler() = default;

    static QMap<QString, QString> parseQueryParams(const QString& queryString) {
        QMap<QString, QString> params;
        if (queryString.isEmpty()) return params;

        QStringList pairs = queryString.split('&', Qt::SkipEmptyParts);
        for (const QString& pair : pairs) {
            int idx = pair.indexOf('=');
            if (idx > 0) {
                QString key = pair.left(idx);
                QString value = pair.mid(idx + 1);
                params[key] = QUrl::fromPercentEncoding(value.toUtf8());
            } else if (!pair.isEmpty()) {
                params[pair] = QString();
            }
        }
        return params;
    }
};

#endif
