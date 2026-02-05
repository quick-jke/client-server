#include "config.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Config& Config::instance() {
    static Config instance;
    return instance;
}

Config::Config() {
}

bool Config::loadFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        return false;
    }
    
    if (!doc.isObject()) {
        return false;
    }
    
    m_data = doc.object().toVariantMap();
    return true;
}

void Config::saveToFile(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    
    QJsonObject obj = QJsonObject::fromVariantMap(m_data);
    QJsonDocument doc(obj);
    
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

QVariant Config::value(const QString& key, const QVariant& defaultValue) const {
    QStringList keys = key.split('.');
    QVariant current = m_data;
    
    for (const QString& k : keys) {
        if (current.canConvert<QVariantMap>()) {
            QVariantMap map = current.toMap();
            if (map.contains(k)) {
                current = map.value(k);
            } else {
                return defaultValue;
            }
        } else {
            return defaultValue;
        }
    }
    
    return current.isValid() ? current : defaultValue;
}

QString Config::stringValue(const QString& key, const QString& defaultValue) const {
    return value(key, defaultValue).toString();
}

int Config::intValue(const QString& key, int defaultValue) const {
    return value(key, defaultValue).toInt();
}

bool Config::boolValue(const QString& key, bool defaultValue) const {
    return value(key, defaultValue).toBool();
}

double Config::doubleValue(const QString& key, double defaultValue) const {
    return value(key, defaultValue).toDouble();
}

void Config::setValue(const QString& key, const QVariant& value) {
    QStringList keys = key.split('.');
    if (keys.isEmpty()) return;
    
    std::function<QVariantMap(QVariantMap, int)> updateMap;
    updateMap = [&](QVariantMap map, int index) -> QVariantMap {
        if (index == keys.size() - 1) {
            map[keys[index]] = value;
            return map;
        }
        
        const QString& currentKey = keys[index];
        QVariantMap nestedMap;
        
        if (map.contains(currentKey)) {
            QVariant existingValue = map.value(currentKey);
            if (existingValue.canConvert<QVariantMap>()) {
                nestedMap = existingValue.toMap();
            }
        }
        
        nestedMap = updateMap(nestedMap, index + 1);
        
        map[currentKey] = nestedMap;
        return map;
    };
    
    m_data = updateMap(m_data, 0);
}

bool Config::contains(const QString& key) const {
    return value(key).isValid();
}
