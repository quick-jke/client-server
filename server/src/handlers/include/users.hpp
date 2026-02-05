#ifndef USERSHANDLER_H
#define USERSHANDLER_H

#include "basehandler.hpp"
#include <QVector>
#include <QMutex>

class User {
public:
    int id;
    QString name;
    QString email;
    
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["id"] = id;
        obj["name"] = name;
        obj["email"] = email;
        return obj;
    }
    
    static User fromJson(const QJsonObject& obj) {
        User user;
        user.id = obj["id"].toInt();
        user.name = obj["name"].toString();
        user.email = obj["email"].toString();
        return user;
    }
};

class UsersHandler : public BaseHandler
{
public:
    UsersHandler();
    Response handleRequest(const Request& request) override;
    
private:
    Response handleGet(const Request& request);
    Response handlePost(const Request& request);
    Response handleGetById(const Request& request, int userId);
    
    QVector<User> m_users;
    QMutex m_mutex;
    int m_nextId;
};

#endif // USERSHANDLER_H
