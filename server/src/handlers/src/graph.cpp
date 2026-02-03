#include "include/graph.h"
#include <QJsonArray>

GraphHandler::GraphHandler()
    : m_nextNodeId(1)
    , m_nextEdgeId(1)
{
}

RequestHandler::Response GraphHandler::handleRequest(const RequestHandler::Request& request)
{
    if (request.path == "/api/graph/nodes") {
        if (request.method == "GET") {
            return handleGetNodes(request);
        } else if (request.method == "POST") {
            return handlePostNode(request);
        }
    } else if (request.path.startsWith("/api/graph/nodes/")) {
        QString idStr = request.path.mid(QString("/api/graph/nodes/").length());
        bool ok;
        int nodeId = idStr.toInt(&ok);
        
        if (ok && nodeId > 0 && request.method == "GET") {
            return handleGetNode(request, nodeId);
        }
    } else if (request.path == "/api/graph/edges") {
        if (request.method == "GET") {
            return handleGetEdges(request);
        } else if (request.method == "POST") {
            return handlePostEdge(request);
        }
    }
    
    return errorResponse("Endpoint not found", 404);
}

RequestHandler::Response GraphHandler::handleGetNodes(const RequestHandler::Request& request)
{
    QJsonArray nodesArray;
    
    QMutexLocker locker(&m_mutex);
    for (const Node& node : m_nodes) {
        nodesArray.append(node.toJson());
    }
    
    return successResponse(nodesArray);
}

RequestHandler::Response GraphHandler::handleGetNode(const RequestHandler::Request& request, int nodeId)
{
    QMutexLocker locker(&m_mutex);
    
    for (const Node& node : m_nodes) {
        if (node.id == nodeId) {
            return successResponse(node.toJson());
        }
    }
    
    return errorResponse("Node not found", 404);
}

RequestHandler::Response GraphHandler::handlePostNode(const RequestHandler::Request& request)
{
    QJsonObject nodeData = parseRequestBody(request.body);
    
    if (nodeData["label"].toString().isEmpty()) {
        return errorResponse("Label is required", 400);
    }
    
    Node node;
    {
        QMutexLocker locker(&m_mutex);
        node.id = m_nextNodeId++;
        node.label = nodeData["label"].toString();
        
        if (nodeData.contains("properties") && nodeData["properties"].isObject()) {
            QJsonObject props = nodeData["properties"].toObject();
            for (auto it = props.begin(); it != props.end(); ++it) {
                node.properties[it.key()] = it.value();
            }
        }
        
        m_nodes.append(node);
    }
    
    return successResponse(node.toJson());
}

RequestHandler::Response GraphHandler::handleGetEdges(const RequestHandler::Request& request)
{
    QJsonArray edgesArray;
    
    QMutexLocker locker(&m_mutex);
    for (const Edge& edge : m_edges) {
        edgesArray.append(edge.toJson());
    }
    
    return successResponse(edgesArray);
}

RequestHandler::Response GraphHandler::handlePostEdge(const RequestHandler::Request& request)
{
    QJsonObject edgeData = parseRequestBody(request.body);
    
    int from = edgeData["from"].toInt();
    int to = edgeData["to"].toInt();
    
    if (from <= 0 || to <= 0) {
        return errorResponse("Invalid from/to node IDs", 400);
    }
    
    Edge edge;
    {
        QMutexLocker locker(&m_mutex);
        edge.id = m_nextEdgeId++;
        edge.from = from;
        edge.to = to;
        edge.label = edgeData["label"].toString();
        
        if (edgeData.contains("properties") && edgeData["properties"].isObject()) {
            QJsonObject props = edgeData["properties"].toObject();
            for (auto it = props.begin(); it != props.end(); ++it) {
                edge.properties[it.key()] = it.value();
            }
        }
        
        m_edges.append(edge);
    }
    
    return successResponse(edge.toJson());
}
