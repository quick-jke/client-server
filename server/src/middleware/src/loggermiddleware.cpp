#include "include/loggermiddleware.hpp"
#include "../../lib/common/logger.hpp"  // ← Добавлен заголовок логгера
#include <QDateTime>

bool LoggerMiddleware::process(RequestHandler::Request& request)
{
    LOG_INFO(QString("[%1] %2 %3").arg(
        request.clientAddress,
        request.method,
        request.path
    ));
    
    return true; // Продолжаем обработку
}
