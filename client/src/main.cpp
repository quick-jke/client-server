#include <QCoreApplication>
#include <QTimer>
#include <QDebug>
#include "httpclient.h"
#include "requestbuilder.h"
#include "models/user.h"
#include "models/graph.h"
#include "../../lib/common/logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // Настройка логгера
    Logger::instance().setLevel(Logger::INFO);
    Logger::instance().enableConsoleOutput(true);
    
    LOG_INFO("=== HTTP Client Starting ===");
    
    HttpClient client;
    client.setTimeout(10000); // 10 секунд
    
    // Колбэки
    client.onSuccess([](const QByteArray& response, int statusCode, QNetworkReply*) {
        LOG_INFO(QString("Success! Status: %1").arg(statusCode));
        LOG_INFO(QString("Response: %1").arg(QString::fromUtf8(response)));
    });
    
    client.onError([](const QString& error, int statusCode, QNetworkReply*) {
        LOG_ERROR(QString("Error: %1 (Status: %2)").arg(error).arg(statusCode));
    });
    
    // Тестовые запросы
    QTimer::singleShot(1000, [&client]() {
        LOG_INFO("\n=== Testing API ===\n");
        
        // 1. Hello endpoint
        LOG_INFO("1. Testing /api/hello");
        client.get("http://localhost:8080/api/hello");

        LOG_INFO("2. Testing /api/settings");
        client.get("http://localhost:8080/api/settings");

        LOG_INFO("3. Testing /api/objects with params");
        QVariantMap params;
        params["oldRect"] = "0";
        params["oldScale"] = "0";
        params["newRect"] = "2";
        params["newScale"] = "4";
        client.get("http://localhost:8080/api/objects", params);
        // // 2. Hello with parameter
        // LOG_INFO("\n2. Testing /api/hello?name=Alice");
        // QVariantMap params;
        // params["name"] = "Alice";
        // client.get("http://localhost:8080/api/hello", params);
        
        // // 3. Get all users
        // LOG_INFO("\n3. Testing /api/users (GET)");
        // client.get("http://localhost:8080/api/users");
        
        // // 4. Create user
        // LOG_INFO("\n4. Testing /api/users (POST)");
        // QJsonObject newUser;
        // newUser["name"] = "Bob Johnson";
        // newUser["email"] = "bob@example.com";
        // client.post("http://localhost:8080/api/users", newUser);
        
        // // 5. Get graph nodes (без авторизации - должно быть 403)
        // LOG_INFO("\n5. Testing /api/graph/nodes without auth (should be 403)");
        // client.get("http://localhost:8080/api/graph/nodes");
        
        // // 6. Get graph nodes (с авторизацией)
        // LOG_INFO("\n6. Testing /api/graph/nodes with auth");
        // HttpClient clientWithAuth;
        // clientWithAuth.setApiKey("test-api-key-12345");
        // clientWithAuth.onSuccess([](const QByteArray& response, int statusCode, QNetworkReply*) {
        //     LOG_INFO(QString("Graph nodes: Status %1").arg(statusCode));
        //     LOG_INFO(QString("Response: %1").arg(QString::fromUtf8(response)));
        // });
        // clientWithAuth.onError([](const QString& error, int statusCode, QNetworkReply*) {
        //     LOG_ERROR(QString("Graph error: %1 (Status: %2)").arg(error).arg(statusCode));
        // });
        // clientWithAuth.get("http://localhost:8080/api/graph/nodes");
        
        // // 7. Create graph node
        // LOG_INFO("\n7. Testing /api/graph/nodes (POST)");
        // QJsonObject node;
        // node["label"] = "Test Node";
        // QJsonObject props;
        // props["type"] = "test";
        // props["value"] = 42;
        // node["properties"] = props;
        // clientWithAuth.post("http://localhost:8080/api/graph/nodes", node);
    });
    
    return app.exec();
}
