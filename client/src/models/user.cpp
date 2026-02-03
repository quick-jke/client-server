#include "user.h"

User::User()
    : m_id(0)
{
}

User::User(int id, const QString& name, const QString& email)
    : m_id(id)
    , m_name(name)
    , m_email(email)
{
}

QJsonObject User::toJson() const
{
    QJsonObject obj;
    obj["id"] = m_id;
    obj["name"] = m_name;
    obj["email"] = m_email;
    return obj;
}

User User::fromJson(const QJsonObject& json)
{
    User user;
    user.m_id = json["id"].toInt();
    user.m_name = json["name"].toString();
    user.m_email = json["email"].toString();
    return user;
}
