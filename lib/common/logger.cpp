#include "logger.hpp"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : m_level(INFO)
    , m_consoleOutput(true)
{
}

Logger::~Logger() {
}

void Logger::setLevel(Level level) {
    m_level = level;
}

void Logger::setLogFile(const QString& filePath) {
    m_logFile = filePath;
    
    QFileInfo fileInfo(filePath);
    if (!fileInfo.dir().exists()) {
        fileInfo.dir().mkpath(".");
    }
}

void Logger::enableConsoleOutput(bool enable) {
    m_consoleOutput = enable;
}

void Logger::debug(const QString& message) {
    log(DEBUG, message);
}

void Logger::info(const QString& message) {
    log(INFO, message);
}

void Logger::warning(const QString& message) {
    log(WARNING, message);
}

void Logger::error(const QString& message) {
    log(ERROR, message);
}

void Logger::critical(const QString& message) {
    log(CRITICAL, message);
}

void Logger::log(Level level, const QString& message) {
    if (level < m_level) return;
    writeLog(level, message);
}

void Logger::writeLog(Level level, const QString& message) {
    QString logMessage = QString("[%1] [%2] %3")
        .arg(timestamp())
        .arg(levelToString(level))
        .arg(message);
    
    if (m_consoleOutput) {
        switch (level) {
        case DEBUG:
            qDebug().noquote() << logMessage;
            break;
        case INFO:
            qInfo().noquote() << logMessage;
            break;
        case WARNING:
            qWarning().noquote() << logMessage;
            break;
        case ERROR:
        case CRITICAL:
            qCritical().noquote() << logMessage;
            break;
        }
    }
    
    if (!m_logFile.isEmpty()) {
        QFile file(m_logFile);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << logMessage << "\n";
            file.close();
        }
    }
}

QString Logger::levelToString(Level level) const {
    switch (level) {
    case DEBUG: return "DEBUG";
    case INFO: return "INFO";
    case WARNING: return "WARNING";
    case ERROR: return "ERROR";
    case CRITICAL: return "CRITICAL";
    }
    return "UNKNOWN";
}

QString Logger::timestamp() const {
    return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
}
