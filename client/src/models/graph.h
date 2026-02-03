#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>

class GraphNode
{
public:
    GraphNode();
    GraphNode(int id, const QString& label);
    
    int id() const { return m_id; }
    QString label() const { return m_label; }
    QMap<QString, QJsonValue> properties() const { return m_properties; }
    
    void setId(int id) { m_id = id; }
    void setLabel(const QString& label) { m_label = label; }
    void setProperty(const QString& key, const QJsonValue& value) { m_properties[key] = value; }
    
    QJsonObject toJson() const;
    static GraphNode fromJson(const QJsonObject& json);
    
private:
    int m_id;
    QString m_label;
    QMap<QString, QJsonValue> m_properties;
};

class GraphEdge
{
public:
    GraphEdge();
    GraphEdge(int id, int from, int to, const QString& label = QString());
    
    int id() const { return m_id; }
    int from() const { return m_from; }
    int to() const { return m_to; }
    QString label() const { return m_label; }
    QMap<QString, QJsonValue> properties() const { return m_properties; }
    
    void setId(int id) { m_id = id; }
    void setFrom(int from) { m_from = from; }
    void setTo(int to) { m_to = to; }
    void setLabel(const QString& label) { m_label = label; }
    void setProperty(const QString& key, const QJsonValue& value) { m_properties[key] = value; }
    
    QJsonObject toJson() const;
    static GraphEdge fromJson(const QJsonObject& json);
    
private:
    int m_id;
    int m_from;
    int m_to;
    QString m_label;
    QMap<QString, QJsonValue> m_properties;
};

class Graph
{
public:
    Graph();
    
    void addNode(const GraphNode& node);
    void addEdge(const GraphEdge& edge);
    
    QVector<GraphNode> nodes() const { return m_nodes; }
    QVector<GraphEdge> edges() const { return m_edges; }
    
    QJsonObject toJson() const;
    static Graph fromJson(const QJsonObject& json);
    
private:
    QVector<GraphNode> m_nodes;
    QVector<GraphEdge> m_edges;
};

#endif // GRAPH_H
