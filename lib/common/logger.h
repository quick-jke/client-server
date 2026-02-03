#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QDateTime>
#include <memory>

class Logger
{
public:
    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

    static Logger& instance();
    
    void setLevel(Level level);
    void setLogFile(const QString& filePath);
    void enableConsoleOutput(bool enable);
    
    void debug(const QString& message);
    void info(const QString& message);
    void warning(const QString& message);
    void error(const QString& message);
    void critical(const QString& message);
    
    void log(Level level, const QString& message);
    
private:
    Logger();
    ~Logger();
    
    void writeLog(Level level, const QString& message);
    QString levelToString(Level level) const;
    QString timestamp() const;
    
    Level m_level;
    QString m_logFile;
    bool m_consoleOutput;
};

#define LOG_DEBUG(msg) Logger::instance().debug(msg)
#define LOG_INFO(msg) Logger::instance().info(msg)
#define LOG_WARNING(msg) Logger::instance().warning(msg)
#define LOG_ERROR(msg) Logger::instance().error(msg)
#define LOG_CRITICAL(msg) Logger::instance().critical(msg)

#endif 
