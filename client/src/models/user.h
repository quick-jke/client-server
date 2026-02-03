#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

class User
{
public:
    User();
    User(int id, const QString& name, const QString& email);
    
    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString email() const { return m_email; }
    
    void setId(int id) { m_id = id; }
    void setName(const QString& name) { m_name = name; }
    void setEmail(const QString& email) { m_email = email; }
    
    QJsonObject toJson() const;
    static User fromJson(const QJsonObject& json);
    
private:
    int m_id;
    QString m_name;
    QString m_email;
};

#endif // USER_H
