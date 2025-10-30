#pragma once
#include <QGraphicsScene>
#include <QMap>
#include "VertexItem.h"

class Graph_2;
class Vertex_2;
class Edge_2;
class EdgeItem;

class GraphScene : public QGraphicsScene {
    Q_OBJECT
    public:
    explicit GraphScene(QObject* parent = nullptr);
    ~GraphScene();

    void setGraph(Graph_2* graph);
    void clearScene();
    VertexItem* addVertex(Vertex_2* vertex);
    EdgeItem* addEdge(Edge_2* edge);
    void removeVertex(Vertex_2* vertex);
    void removeEdge(Edge_2* edge);

    VertexItem* getVertexItem(Vertex_2* vertex) const;
    EdgeItem* getEdgeItem(Edge_2* edge) const;

    void resetAllColors();
    void updateEdgeWeights();

private:
    Graph_2* m_graph;
    QMap<Vertex_2*, VertexItem*> m_vertexItems;
    QMap<Edge_2*, EdgeItem*> m_edgeItems;
    QPointF m_nextNodePos;
    int m_nodesInRow;
    static constexpr int MAX_NODES_PER_ROW = 5;
    static constexpr qreal NODE_SPACING = 50.0;
};