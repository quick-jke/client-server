#include "loggermiddleware.h"
#include "../../lib/common/logger.h"  // ← Добавлен заголовок логгера
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
