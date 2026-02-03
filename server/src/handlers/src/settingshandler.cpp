#include "include/settingshandler.h"
#include "../../lib/common/config.h"
#include <QJsonObject>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QDir>
RequestHandler::Response SettingsHandler::handleRequest(const RequestHandler::Request& request)
{
    if (request.method == "GET") {
        return handleGet(request);
    }
    
    return errorResponse("Method not allowed", 405);
}

QString dirPath(QString p)
{
    if (p.isEmpty()) return p;
    QChar sep = QDir::separator();
    if (p.endsWith(sep)) return p;
    return p + sep;
}

RequestHandler::Response SettingsHandler::handleGet(const RequestHandler::Request& request)
{
    Q_UNUSED(request);
    
    Config& config = Config::instance();
    
    QString iniPath = config.stringValue("path.ini", "");

    QJsonObject response;
    response["iniPath"] = iniPath;
    response["exists"] = QFile::exists(iniPath);

    if (QFile::exists(iniPath)) {
        QSettings settings(iniPath, QSettings::IniFormat);
        settings.beginGroup("path");

        response["objectsPath"] = settings.value("objectsPath").toString();
        response["subServPath"] = settings.value("subServPath").toString();
        response["toolbarsPath"] = settings.value("toolbarsPath").toString();
        response["lineTemplatePath"] = settings.value("lineTemplatePath").toString();

        response["iconsPath"] = dirPath(settings.value("iconsPath").toString());
        response["objImagesPath"] = dirPath(settings.value("objImagesPath").toString());
        response["subImagesPath"] = dirPath(settings.value("subImagesPath").toString());
    }
    
    return successResponse(response);
}
