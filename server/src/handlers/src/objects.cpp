#include "include/objects.hpp"
#include <QDateTime>

#include <QRect>

RequestHandler::Response ObjectsHandler::handleRequest(const RequestHandler::Request& request)
{
    if (request.method == "GET") {
        return handleGet(request);
    }
    
    return errorResponse("Method not allowed", 405);
}

RequestHandler::Response ObjectsHandler::handleGet(const RequestHandler::Request& request)
{


    QJsonObject data;
    data["oldRect"] = request.queryParams.value("oldRect");
    data["oldScale"] = request.queryParams.value("oldScale");
    data["newRect"] = request.queryParams.value("newRect");
    data["newScale"] = request.queryParams.value("newScale");
    data["timestamp"] = QDateTime::currentMSecsSinceEpoch();
    data["client"] = request.clientAddress;


    //hardcode
    // QRect oldRect = QRect(0, 0, 0, 0);
    // int oldScale = 0;
    // QRect newRect = QRect(12846028, 13995115, 12134, 7603);
    // int newScale = 245;

    // PositionList posList;
    // if( pNodeHash->empty() ) {
    //     pNodeHash = nullptr;
    // }

    // SelectParam sp( oldRect, newRect );

    // for( auto item = m_RootHash.begin(); item != m_RootHash.end(); ++item )
    // {
    //     if( item.key() == 0 || pClassHash->value( item.key(), false ) )
    //     {
    //         int minScale = item.value().m_MinScale;
    //         if( minScale && newScale < minScale ) {
    //             continue;
    //         }

    //         int maxScale = item.value().m_MaxScale;
    //         if( maxScale && newScale > maxScale ) {
    //             continue;
    //         }

    //         PositionList clsList;
    //         sp.m_pPosList = &clsList;
    //         sp.m_bSameScale = oldScale == newScale || ( ( !minScale || oldScale >= minScale ) && ( !maxScale || oldScale <= maxScale ) );
    //         item.value().selectObjects( &sp, pNodeHash );
    //         clsList.sort();
    //         posList.append( clsList );
    //     }
    // }
    return successResponse(data);
}


// RequestHandler::Response ObjectsHandler::handleGet(const RequestHandler::Request& request)
// {
//     QJsonObject data;
//     QRect oldRect = request.queryParams.value("oldRect");
//     QRect oldScale = request.queryParams.value("oldScale");
//     QRect newRect = request.queryParams.value("newRect");
//     QRect newScale = request.queryParams.value("newScale");
//     data["timestamp"] = QDateTime::currentMSecsSinceEpoch();
//     data["client"] = request.clientAddress;

//     data["objects"] = store_->selectObjectsByParams(oldRect, oldScale, newRect, newScale);

//     return successResponse(data);


// }
