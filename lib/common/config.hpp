#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QVariantMap>
#include <QVariant>

class Config
{
public:
    static Config& instance();
    
    bool loadFromFile(const QString& filePath);
    void saveToFile(const QString& filePath) const;
    
    QVariant value(const QString& key, const QVariant& defaultValue = QVariant()) const;
    QString stringValue(const QString& key, const QString& defaultValue = QString()) const;
    int intValue(const QString& key, int defaultValue = 0) const;
    bool boolValue(const QString& key, bool defaultValue = false) const;
    double doubleValue(const QString& key, double defaultValue = 0.0) const;
    
    void setValue(const QString& key, const QVariant& value);
    
    bool contains(const QString& key) const;
    
private:
    Config();
    QVariantMap m_data;
};

#endif 
