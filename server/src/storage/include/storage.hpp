#ifndef STORE_HPP
#define STORE_HPP

#include <QByteArray>
#include <QRect>
#include <QDataStream>
#include <QIODevice>

#include "position_list.hpp"
#include "result_code.hpp"
#include "user_chanal.hpp"
#include "space_tree.hpp"
// #include <QTextCodec>
class MapObjectStorage {
public:
    void doLoad(QByteArray data)
    {
        int user;
        QRect newRect, oldRect;
        int newScale, oldScale;
        QDataStream(&data, QIODevice::ReadOnly) >> user >> newRect >> newScale >> oldRect >> oldScale;
        auto userChanal = m_UserChanalHash[user];
        auto posList = m_SpaceIndex.selectObjects(oldRect, oldScale, newRect, newScale, &userChanal.m_ClassHash, &userChanal.m_NodeHash);

        // emit dataSizeReady(posList.size());
        load(posList, newScale);
    }

    void load(PositionList& posList, int newScale)
    {
        if (posList.isEmpty())
        {
            exit(0);
            return;
        }

        int const BUF_SIZE = 0x40000;

        QByteArray dst;
        dst.resize(BUF_SIZE);
        qint64 wPos = 0;
        for (auto ps = posList.cbegin(), pe = posList.cend(); ps != pe; ++ps)
        {
            if (ps->m_Size > BUF_SIZE)
            {
                continue;
            }

            if (wPos + ps->m_Size > BUF_SIZE)
            {
                dst.resize(wPos);
                emit dataReady(dst, newScale);
                dst.resize(BUF_SIZE);
                wPos = 0;
            }

            auto r = readData(ps->m_Pos, dst.data() + wPos, ps->m_Size, &m_dataFile);
            wPos += ps->m_Size;

            if (r != R_OK)
            {
                exit(r);
                return;
            }
        }
        if (wPos != 0)
        {
            dst.resize(wPos);
            emit dataReady(dst, newScale);
        }
        exit(0);
    }

    void open(QString iniPath, bool bIndexing)
    {
        // m_bIsOpen = false;
        // m_IniPath = iniPath;

        // QSettings settings( iniPath, QSettings::IniFormat );
        // settings.beginGroup( QStringLiteral( "path" ) );
        // m_DataPath = settings.value( QStringLiteral( "projectPath" ) ).toString();
        // settings.endGroup();
        // settings.beginGroup( QStringLiteral( "projectSet" ) );
        // m_MapNumber = settings.value( QStringLiteral( "mapnumber" ) ).toInt();
        // auto t0 = settings.value( QStringLiteral( "mapName" ) ).toString();
        // auto t1 = t0.toLatin1();
        // m_MapName = t1.isEmpty() ? "?" : t1[0] == '?' ? t0 : QTextCodec::codecForName( "Windows-1251" )->toUnicode( t1 );
        // settings.endGroup();

        // int iop = m_DataPath.lastIndexOf('.');
        // QString path = iop >= 0 ? m_DataPath.left(iop) : m_DataPath;
        // m_TmpPath = path + ".tmp";
        // m_BakPath = path + ".bak";
        // m_OldPath = path + ".old";

        // ResultCode r = R_OK;

        // m_dataFile.setFileName(m_DataPath);
        // bool bNew = !m_dataFile.exists();

        // QFileInfo fi(m_DataPath);
        // QString sf = fi.suffix();
        
        // if(bNew) {
        //     if( reinterpret_cast<MapObject*>( this )->oldVersion() ){
        //         m_pHR = &m_HR113;
        //     } else {
        //         m_pHR = &m_HR201;
        //     }
        //     m_pHR->init();
        //     m_dataFile.open(QIODevice::WriteOnly);
        //     r = saveHR();
        //     m_dataFile.close();
        //     if( r != R_OK ){
        //         return trace(R_WRITEERROR, m_DataPath);
        //     }
        // }

        // if(!m_dataFile.open(QIODevice::ReadWrite)){
        //     return trace(R_OPENERROR, m_DataPath);
        // }

        // if( !defineVersion(&m_dataFile)) {
        //     return trace(R_BADFILESTRUCT, m_DataPath);
        // }

        // if(m_bOldVersion) {
        //     QTextStream badStr( &m_logFile );
        //     reinterpret_cast<MapObject*>( this )->upgrateObjects( badStr );
        //     reinterpret_cast<MapObject*>(this)->upgrateTemplates( badStr );
        // }

        // if( bIndexing )
        // {
        //     bool bRestart = !m_ObjectIndex.isEmpty();
        //     m_ObjectIndex.clear();
        //     m_SpaceIndex.setObjects( &m_Objects );
        //     r = createIndexes( bRestart );
        //     trace( R_INFORMATION, "indexed " + m_DataPath );
        // }

        // m_bIsOpen = true;
        // trace( R_INFORMATION, "opened " + m_DataPath );

        // return r;
    }
    void createIndexes(bool bRestart)
    {

    }
    
private:
    ResultCode readData(qint64 pos, char* buf, int size, QIODevice* pSrc)
    {
        ResultCode r = pSrc->seek(pos) && pSrc->read(buf, size) == size ? R_OK : R_READERROR;
        return r == R_OK ? r : trace(r, pSrc->inherits("QFileDevice") ? ((QFileDevice*)pSrc)->fileName() : "", pos, 2);
    }

    ResultCode trace(ResultCode r, QString txt, qint64 p0, long p1)
    {
        return r;
    }
    
    QMap<int, UserChanal> m_UserChanalHash;
    SpaceTree m_SpaceIndex;
    QFile   m_dataFile;

    bool    m_bIsOpen;
    QString m_IniPath;
    QString m_DataPath;
    int     m_MapNumber;
    QString m_MapName;
    QString m_TmpPath;  
    QString m_BakPath;
    QString m_OldPath;



signals:
    void dataReady(QByteArray data, int newScale = 0);
    void dataSizeReady(int size);
    void dataComplete();
    void dataError(int code); 
};

#endif
