#include <QCoreApplication>
#include <QTimer>
#include <QDir>
#include <vector>
#include <memory>
#include "httpserver.h"
#include "handlers/hellohandler.h"
#include "handlers/usershandler.h"
#include "handlers/graphhandler.h"
#include "handlers/settingshandler.h"
#include "middleware/loggermiddleware.h"
#include "middleware/authmiddleware.h"
#include "../../lib/common/logger.h"
#include "../../lib/common/config.h"

// Комбинированный обработчик для маршрутизации (безопасная реализация с вектором)
class RouterHandler : public RequestHandler
{
public:
    void addRoute(const QString& pathPrefix, std::unique_ptr<RequestHandler> handler) {
        // Сохраняем пару (префикс, обработчик) с перемещением уникального указателя
        m_routes.emplace_back(pathPrefix, std::move(handler));
    }
    
    Response handleRequest(const Request& request) override {
        // Ищем от более длинных префиксов к коротким для правильного сопоставления
        // Сортируем временно по длине префикса в обратном порядке
        std::vector<std::pair<QString, RequestHandler*>> sortedRoutes;
        for (auto& route : m_routes) {
            sortedRoutes.emplace_back(route.first, route.second.get());
        }
        
        // Сортировка по длине префикса (от длинного к короткому)
        std::sort(sortedRoutes.begin(), sortedRoutes.end(), 
            [](const auto& a, const auto& b) {
                return a.first.length() > b.first.length();
            });
        
        // Поиск подходящего маршрута
        for (const auto& [prefix, handler] : sortedRoutes) {
            if (request.path.startsWith(prefix)) {
                return handler->handleRequest(request);
            }
        }
        
        return Response::notFound("Route not found");
    }
    
private:
    // Вектор пар: префикс + уникальный указатель на обработчик
    std::vector<std::pair<QString, std::unique_ptr<RequestHandler>>> m_routes;
};

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    // Настройка логгера
    Logger::instance().setLevel(Logger::DEBUG);
    Logger::instance().setLogFile("server.log");
    
    LOG_INFO("=== HTTP Server Starting ===");
    
    // Загрузка конфигурации
    Config& config = Config::instance();
    QString configPath = "config.json";
    
    if (QFile::exists(configPath)) {
        if (config.loadFromFile(configPath)) {
            LOG_INFO("Configuration loaded successfully");
        } else {
            LOG_WARNING("Failed to load configuration, using defaults");
        }
    } else {
        LOG_WARNING(QString("Config file not found: %1").arg(configPath));
    }
    
    // Создание сервера
    quint16 port = config.intValue("server.port", 8080);
    HttpServer server(port);
    
    // Создание маршрутизатора
    auto router = std::make_unique<RouterHandler>();
    router->addRoute("/api/hello", std::make_unique<HelloHandler>());
    router->addRoute("/api/users", std::make_unique<UsersHandler>());
    router->addRoute("/api/graph", std::make_unique<GraphHandler>());
    router->addRoute("/api/settings", std::make_unique<SettingsHandler>());
    
    server.setRequestHandler(std::move(router));
    
    // Добавление мидлвар
    server.addMiddleware(std::make_unique<LoggerMiddleware>());
    
    auto authMiddleware = std::make_unique<AuthMiddleware>();
    authMiddleware->addProtectedPath("/api/graph");
    server.addMiddleware(std::move(authMiddleware));
    
    // Обработка ошибок
    QObject::connect(&server, &HttpServer::errorOccurred, [](const QString& error) {
        LOG_CRITICAL(error);
    });
    
    // Запуск сервера
    if (!server.start()) {
        LOG_CRITICAL("Failed to start server");
        return 1;
    }
    
    LOG_INFO(QString("Server is running on port %1").arg(port));
    LOG_INFO("Press Ctrl+C to stop");
    
    // Graceful shutdown
    QObject::connect(&app, &QCoreApplication::aboutToQuit, [&server]() {
        server.stop();
        LOG_INFO("=== HTTP Server Stopped ===");
    });
    
    return app.exec();
}
