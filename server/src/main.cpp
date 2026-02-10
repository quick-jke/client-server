#include <QCoreApplication>
#include <QTimer>
#include <QDir>
#include <vector>
#include <memory>
#include "httpserver.hpp"
#include "hello.hpp"
#include "users.hpp"
#include "graph.hpp"
#include "settings.hpp"
#include "objects.hpp"
#include "loggermiddleware.hpp"
#include "authmiddleware.hpp"
#include "../../lib/common/logger.hpp"
#include "../../lib/common/config.hpp"

class RouterHandler : public RequestHandler
{
public:
    void addRoute(const QString& pathPrefix, std::unique_ptr<RequestHandler> handler) {
        routes_.emplace_back(pathPrefix, std::move(handler));
    }
    
    Response handleRequest(const Request& request) override {
        std::vector<std::pair<QString, RequestHandler*>> sortedRoutes;
        for (auto& route : routes_) {
            sortedRoutes.emplace_back(route.first, route.second.get());
        }
        
        std::sort(sortedRoutes.begin(), sortedRoutes.end(), 
            [](const auto& a, const auto& b) {
                return a.first.length() > b.first.length();
            });
        
        for (const auto& [prefix, handler] : sortedRoutes) {
            if (request.path.startsWith(prefix)) {
                return handler->handleRequest(request);
            }
        }
        
        return Response::notFound("Route not found");
    }
    
private:
    std::vector<std::pair<QString, std::unique_ptr<RequestHandler>>> routes_;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    Logger::instance().setLevel(Logger::DEBUG);
    Logger::instance().setLogFile("server.log");
    
    LOG_INFO("=== HTTP Server Starting ===");
    
    Config& config = Config::instance();
    QString configPath = "app.json";
    
    if (QFile::exists(configPath)) {
        if (config.loadFromFile(configPath)) {
            LOG_INFO("Configuration loaded successfully");
        } else {
            LOG_WARNING("Failed to load configuration, using defaults");
        }
    } else {
        LOG_WARNING(QString("Config file not found: %1").arg(configPath));
    }
    
    quint16 port = config.intValue("server.port", 8080);
    HttpServer server(port);
    
    auto router = std::make_unique<RouterHandler>();
    router->addRoute("/api/hello",      std::make_unique<HelloHandler>());
    router->addRoute("/api/users",      std::make_unique<UsersHandler>());
    router->addRoute("/api/graph",      std::make_unique<GraphHandler>());
    router->addRoute("/api/settings",   std::make_unique<SettingsHandler>());
    router->addRoute("/api/objects",    std::make_unique<ObjectsHandler>());
    
    server.setRequestHandler(std::move(router));
    
    server.addMiddleware(std::make_unique<LoggerMiddleware>());
    
    auto authMiddleware = std::make_unique<AuthMiddleware>();
    authMiddleware->addProtectedPath("/api/graph");
    server.addMiddleware(std::move(authMiddleware));
    
    QObject::connect(&server, &HttpServer::errorOccurred, [](const QString& error) {
        LOG_CRITICAL(error);
    });
    
    if (!server.start()) {
        LOG_CRITICAL("Failed to start server");
        return 1;
    }
    
    LOG_INFO(QString("Server is running on port %1").arg(port));
    LOG_INFO("Press Ctrl+C to stop");
    
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&server]() {
        server.stop();
        LOG_INFO("=== HTTP Server Stopped ===");
    });
    
    return app.exec();
}
