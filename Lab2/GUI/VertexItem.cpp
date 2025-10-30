#include "VertexItem.h"
#include "EdgeItem.h"
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>

VertexItem::VertexItem(Vertex_2* vertex, QGraphicsItem* parent)
    : QGraphicsEllipseItem(-Radius, -Radius, 2 * Radius, 2 * Radius, parent),
      m_vertex(vertex)
{
    m_defaultColor = Qt::cyan;
    setPen(QPen(Qt::black, 2));
    setBrush(m_defaultColor);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(ItemIsSelectable);

    setCacheMode(DeviceCoordinateCache);
    setZValue(1);

    m_label = new QGraphicsTextItem(QString::number(m_vertex->getId()), this);
    QFont font = m_label->font();
    font.setPointSize(10);
    m_label->setFont(font);

    QRectF textRect = m_label->boundingRect();
    m_label->setPos(-textRect.width() / 2, -textRect.height() / 2);
}

VertexItem::~VertexItem() {}

void VertexItem::setColor(const QColor& color) {
    setBrush(QBrush(color));
}

void VertexItem::resetColor() {
    setBrush(QBrush(m_defaultColor));
}

void VertexItem::addEdge(EdgeItem* edge) {
    if (edge && !m_edgeList.contains(edge)) {
        m_edgeList.append(edge);
    }
}

void VertexItem::removeEdge(EdgeItem* edge) {
    m_edgeList.removeAll(edge);
}

void VertexItem::removeEdges() {
    for (EdgeItem* edge : std::as_const(m_edgeList)) {
        edge->getSourceItem()->removeEdge(edge);
        edge->getDestItem()->removeEdge(edge);
        if (scene()) {
            scene()->removeItem(edge);
        }
        delete edge;
    }
    m_edgeList.clear();
}

QVariant VertexItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionHasChanged && scene()) {
        for (EdgeItem* edge : std::as_const(m_edgeList)) {
            edge->trackVertices();
        }
    }
    return QGraphicsEllipseItem::itemChange(change, value);
}