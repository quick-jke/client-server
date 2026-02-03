#include "graph.h"

GraphNode::GraphNode()
    : m_id(0)
{
}

GraphNode::GraphNode(int id, const QString& label)
    : m_id(id)
    , m_label(label)
{
}

QJsonObject GraphNode::toJson() const
{
    QJsonObject obj;
    obj["id"] = m_id;
    obj["label"] = m_label;
    
    QJsonObject props;
    for (auto it = m_properties.begin(); it != m_properties.end(); ++it) {
        props[it.key()] = it.value();
    }
    obj["properties"] = props;
    
    return obj;
}

GraphNode GraphNode::fromJson(const QJsonObject& json)
{
    GraphNode node;
    node.m_id = json["id"].toInt();
    node.m_label = json["label"].toString();
    
    if (json.contains("properties") && json["properties"].isObject()) {
        QJsonObject props = json["properties"].toObject();
        for (auto it = props.begin(); it != props.end(); ++it) {
            node.m_properties[it.key()] = it.value();
        }
    }
    
    return node;
}

GraphEdge::GraphEdge()
    : m_id(0)
    , m_from(0)
    , m_to(0)
{
}

GraphEdge::GraphEdge(int id, int from, int to, const QString& label)
    : m_id(id)
    , m_from(from)
    , m_to(to)
    , m_label(label)
{
}

QJsonObject GraphEdge::toJson() const
{
    QJsonObject obj;
    obj["id"] = m_id;
    obj["from"] = m_from;
    obj["to"] = m_to;
    obj["label"] = m_label;
    
    QJsonObject props;
    for (auto it = m_properties.begin(); it != m_properties.end(); ++it) {
        props[it.key()] = it.value();
    }
    obj["properties"] = props;
    
    return obj;
}

GraphEdge GraphEdge::fromJson(const QJsonObject& json)
{
    GraphEdge edge;
    edge.m_id = json["id"].toInt();
    edge.m_from = json["from"].toInt();
    edge.m_to = json["to"].toInt();
    edge.m_label = json["label"].toString();
    
    if (json.contains("properties") && json["properties"].isObject()) {
        QJsonObject props = json["properties"].toObject();
        for (auto it = props.begin(); it != props.end(); ++it) {
            edge.m_properties[it.key()] = it.value();
        }
    }
    
    return edge;
}

Graph::Graph()
{
}

void Graph::addNode(const GraphNode& node)
{
    m_nodes.append(node);
}

void Graph::addEdge(const GraphEdge& edge)
{
    m_edges.append(edge);
}

QJsonObject Graph::toJson() const
{
    QJsonObject obj;
    
    QJsonArray nodesArray;
    for (const GraphNode& node : m_nodes) {
        nodesArray.append(node.toJson());
    }
    obj["nodes"] = nodesArray;
    
    QJsonArray edgesArray;
    for (const GraphEdge& edge : m_edges) {
        edgesArray.append(edge.toJson());
    }
    obj["edges"] = edgesArray;
    
    return obj;
}

Graph Graph::fromJson(const QJsonObject& json)
{
    Graph graph;
    
    if (json.contains("nodes") && json["nodes"].isArray()) {
        QJsonArray nodesArray = json["nodes"].toArray();
        for (const QJsonValue& value : nodesArray) {
            if (value.isObject()) {
                graph.addNode(GraphNode::fromJson(value.toObject()));
            }
        }
    }
    
    if (json.contains("edges") && json["edges"].isArray()) {
        QJsonArray edgesArray = json["edges"].toArray();
        for (const QJsonValue& value : edgesArray) {
            if (value.isObject()) {
                graph.addEdge(GraphEdge::fromJson(value.toObject()));
            }
        }
    }
    
    return graph;
}
