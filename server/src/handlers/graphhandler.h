#ifndef GRAPHHANDLER_H
#define GRAPHHANDLER_H

#include "basehandler.h"
#include <QVector>
#include <QMap>
#include <QMutex>

class GraphHandler : public BaseHandler
{
public:
    GraphHandler();
    Response handleRequest(const Request& request) override;
    
private:
    struct Node {
        int id;
        QString label;
        QMap<QString, QJsonValue> properties;
        
        QJsonObject toJson() const {
            QJsonObject obj;
            obj["id"] = id;
            obj["label"] = label;
            QJsonObject props;
            for (auto it = properties.begin(); it != properties.end(); ++it) {
                props[it.key()] = it.value();
            }
            obj["properties"] = props;
            return obj;
        }
    };
    
    struct Edge {
        int id;
        int from;
        int to;
        QString label;
        QMap<QString, QJsonValue> properties;
        
        QJsonObject toJson() const {
            QJsonObject obj;
            obj["id"] = id;
            obj["from"] = from;
            obj["to"] = to;
            obj["label"] = label;
            QJsonObject props;
            for (auto it = properties.begin(); it != properties.end(); ++it) {
                props[it.key()] = it.value();
            }
            obj["properties"] = props;
            return obj;
        }
    };
    
    Response handleGetNodes(const Request& request);
    Response handleGetNode(const Request& request, int nodeId);
    Response handlePostNode(const Request& request);
    Response handleGetEdges(const Request& request);
    Response handlePostEdge(const Request& request);
    
    QVector<Node> m_nodes;
    QVector<Edge> m_edges;
    QMutex m_mutex;
    int m_nextNodeId;
    int m_nextEdgeId;
};

#endif 
