#ifndef STORE_SETTINGS_HPP
#define STORE_SETTINGS_HPP
#include <QSettings>
#include <QString>
#include <QDir>
struct StoreSettings
{
    StoreSettings(QSettings settings)
    {
        objectsPath_ = settings.value("objectsPath").toString();
        subServPath_ = settings.value("subServPath").toString();
        toolbarsPath_ = settings.value("toolbarsPath").toString();
        lineTemplatePath_ = settings.value("lineTemplatePath").toString();

        iconsPath_ = dirPath(settings.value("iconsPath").toString());
        objImagesPath_ = dirPath(settings.value("objImagesPath").toString());
        subImagesPath_ = dirPath(settings.value("subImagesPath").toString());
    }
    QString dirPath(QString p)
    {
        if (p.isEmpty()) return p;
        QChar sep = QDir::separator();
        if (p.endsWith(sep)) return p;
        return p + sep;
    }
    QString objectsPath_;
    QString subServPath_;
    QString toolbarsPath_;
    QString lineTemplatePath_;
    QString iconsPath_;
    QString objImagesPath_;
    QString subImagesPath_;
};

#endif
