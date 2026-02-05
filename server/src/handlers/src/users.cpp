#include "include/users.hpp"
#include <QJsonArray>

UsersHandler::UsersHandler()
    : m_nextId(1)
{
    // Инициализация тестовыми данными
    User user1;
    user1.id = m_nextId++;
    user1.name = "John Doe";
    user1.email = "john@example.com";
    m_users.append(user1);
    
    User user2;
    user2.id = m_nextId++;
    user2.name = "Jane Smith";
    user2.email = "jane@example.com";
    m_users.append(user2);
}

RequestHandler::Response UsersHandler::handleRequest(const RequestHandler::Request& request)
{
    if (request.path == "/api/users") {
        if (request.method == "GET") {
            return handleGet(request);
        } else if (request.method == "POST") {
            return handlePost(request);
        }
    } else if (request.path.startsWith("/api/users/")) {
        // /api/users/123
        QString idStr = request.path.mid(QString("/api/users/").length());
        bool ok;
        int userId = idStr.toInt(&ok);
        
        if (ok && userId > 0) {
            if (request.method == "GET") {
                return handleGetById(request, userId);
            }
        }
    }
    
    return errorResponse("Endpoint not found", 404);
}

RequestHandler::Response UsersHandler::handleGet(const RequestHandler::Request& request)
{
    QJsonArray usersArray;
    
    QMutexLocker locker(&m_mutex);
    for (const User& user : m_users) {
        usersArray.append(user.toJson());
    }
    
    return successResponse(usersArray);
}

RequestHandler::Response UsersHandler::handlePost(const RequestHandler::Request& request)
{
    QJsonObject userData = parseRequestBody(request.body);
    
    if (userData["name"].toString().isEmpty()) {
        return errorResponse("Name is required", 400);
    }
    
    if (userData["email"].toString().isEmpty()) {
        return errorResponse("Email is required", 400);
    }
    
    User user;
    {
        QMutexLocker locker(&m_mutex);
        user.id = m_nextId++;
        user.name = userData["name"].toString();
        user.email = userData["email"].toString();
        m_users.append(user);
    }
    
    return successResponse(user.toJson());
}

RequestHandler::Response UsersHandler::handleGetById(const RequestHandler::Request& request, int userId)
{
    QMutexLocker locker(&m_mutex);
    
    for (const User& user : m_users) {
        if (user.id == userId) {
            return successResponse(user.toJson());
        }
    }
    
    return errorResponse("User not found", 404);
}
